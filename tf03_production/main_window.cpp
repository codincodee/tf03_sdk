#include "main_window.h"
#include "ui_main_window.h"
#include <tf03_common/driver.h>
#include <QElapsedTimer>
#include <QComboBox>
#include <QDebug>
#include <tf03_common/command_echo_handler.h>
#include <tf03_common/command_echo_widgets_manager.h>
#include <QSerialPortInfo>
#include <QDesktopWidget>
#include <tf03_common/apd_page.h>
#include <tf03_common/measure_manifest.h>
#include <tf03_common/apd_page_wrapper.h>
#include <tf03_common/utils.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  if (!IsAPDExperimentPagUsed()) {
    return;
  }

  if (!IsDevelModeSupported()) {
    return;
  }

  ui->setupUi(this);

  this->setWindowIcon(QIcon(":/image/logo.png"));
  QRect desktop = QApplication::desktop()->availableGeometry();
  auto window = this->rect();
  this->move(desktop.topLeft() - window.topLeft());
  this->move(QPoint(desktop.width() / 2 - window.width() / 2, 0));

  driver_.reset(new Driver);
  driver_->Open();
  timer_id_ = startTimer(100);
//  timer_id_ = startTimer(0);
  frequency_timer_.start();

  command_echo_handler_.reset(new CommandEchoHandler);
  command_echo_handler_->SetDriver(driver_);

  connect_button_current_lingual_ = kConnectPushButtonText;
  auto baud_rates = Driver::BaudRates();
  for (auto& rate : baud_rates) {
    ui->BaudRateComboBox->addItem(QString::number(rate));
  }

  std::shared_ptr<APDPage> apd_core(new APDPage);
  apd_page_.reset(new APDPageWrapper);
  apd_page_->SetAPDPageCore(apd_core);

  ui->tabWidget->setFont(PageBase::GetCommonFont());

  if (apd_core) {
    apd_core->UsePageBaseSpecs(true);
    apd_core->SetLayout(ui->APDPagePlotVerticalLayout);
    apd_core->SetAPDDisplayLabel(ui->APDPageAPDVoltageLabel);
    apd_core->SetTemperatureDisplayLabel(ui->APDPageTemperatureLabel);
    apd_core->SetStartPushButton(ui->APDPageStartPushButton);
    apd_core->SetProgressBar(ui->APDPageProgressBar);
    apd_core->SetAPDFromLineEdit(ui->APDPageAPDFromLineEdit);
    apd_core->SetAPDToLineEdit(ui->APDPageAPDToLineEdit);
    apd_core->SetThresholdLineEdit(ui->APDPageThresholdLineEdit);
    apd_core->SetDriver(driver_);
    apd_core->SetCmdEchoHandler(command_echo_handler_);
    apd_core->SetStatusLabel(ui->APDPageStatusLabel);
    if (!apd_core->Initialize()) {
      apd_core.reset();
    }
  }

  apd_page_->SetWidgetFontCommon(ui->APDPageAPDFromTipLabel);
  apd_page_->SetWidgetFontCommon(ui->APDPageAPDToTipLabel);
  apd_page_->SetWidgetFontCommon(ui->APDPageThresholdTipLabel);
  apd_page_->SetWidgetFontCommon(ui->APDPageAPDVoltageTipLabel);
  apd_page_->SetWidgetFontCommon(ui->APDPageTemperatureTipLabel);

  ui->APDPageAPDFromLineEdit->setDisabled(true);
  ui->APDPageAPDToLineEdit->setDisabled(true);
  ui->APDPageThresholdLineEdit->setDisabled(true);

  ui->tabWidget->setCurrentIndex(0);
  SetupUIText();
}

MainWindow::~MainWindow()
{
  driver_->Close();
}


void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }

  if (apd_page_) {
    apd_page_->Update();
  }

  if (driver_->DetectAndAutoConnect()) {
    connect_button_current_lingual_ = kDisconnectPushButtonText;
    ui->ConnectPushButton->setText(
        which_lingual(connect_button_current_lingual_));
  }

  UpdatePortNameComboBox();

  MeasureBasic measure;
  auto measure_basic = driver_->LastMeasure();
  if (measure_basic) {
    measure = *measure_basic;
  }
  auto new_measure = (measure_basic != nullptr);
  auto elapse = frequency_timer_.elapsed();
  if (new_measure) {
    ui->DistanceDisplayLabel->setText(QString::number(measure.dist));
    if (elapse > 1000) {
      auto id_diff = measure.id - last_freq_measure_id_;
      float frequency = id_diff / (elapse / 1000.0f);
      ui->FrequencyDisplayLabel->setText(QString::number(std::ceil(frequency)));
      last_freq_measure_id_ = measure.id;
      frequency_timer_.restart();
    }
  }

  if (elapse > 2000) {
    ui->FrequencyDisplayLabel->clear();
    ui->DistanceDisplayLabel->clear();
  }

  command_echo_handler_->Probe();

  auto measure_devel = ToMeasureDevel(measure_basic);

  if (measure_devel) {
    auto apd_core = apd_page_->GetCore();
    if (apd_core) {
      apd_core->IncomingMeasure(*measure_devel);
    }
  }
}

void MainWindow::on_ChinesePushButton_clicked()
{
  set_current_language(Language::chinese);
  SetupUIText();
}

void MainWindow::on_EnglishPushButton_clicked()
{
  set_current_language(Language::english);
  SetupUIText();
}

void MainWindow::SetupUIText() {
  ui->DistanceHintLabel->setText(which_lingual(kDistanceLabelText));
  ui->FrequencyHintLabel->setText(which_lingual(kFrequencyLabelText));
  ui->PortNameLabel->setText(which_lingual(kPortNameLabelText));
  ui->BaudRateLabel->setText(which_lingual(kBaudRateLabelText));
  ui->ConnectPushButton->setText(
      which_lingual(connect_button_current_lingual_));
  ui->tabWidget->setTabText(0, which_lingual({"Setup", "配置"}));
  this->setWindowTitle(which_lingual(kWindowTitle) +  " v" + kVersion);
}

void MainWindow::on_ConnectPushButton_clicked()
{
  if (lingual_equal(ui->ConnectPushButton->text(), kDisconnectPushButtonText)) {
    driver_->Close();
    connect_button_current_lingual_ = kConnectPushButtonText;
    ui->ConnectPushButton->setText(
        which_lingual(connect_button_current_lingual_));
    return;
  }
  driver_->SetPortName(ui->SerialPortComboBox->currentText());
  bool ok;
  driver_->SetBaudRate(ui->BaudRateComboBox->currentText().toInt(&ok));
  if (!ok) {
    return;
  }
  driver_->Open();
  connect_button_current_lingual_ = kDisconnectPushButtonText;
  ui->ConnectPushButton->setText(
      which_lingual(connect_button_current_lingual_));
}

void MainWindow::UpdatePortNameComboBox() {
  auto ports = QSerialPortInfo::availablePorts();
  auto update = [this, ports](){
    ui->SerialPortComboBox->clear();
    for (auto& port : ports) {
      ui->SerialPortComboBox->addItem(port.portName());
    }
    port_combo_.clear();
    for (auto& port : ports) {
      port_combo_.push_back(port.portName());
    }
  };
  if (ports.size() != port_combo_.size()) {
    update();
    return;
  }
  bool should_update = false;
  for (auto& port : ports) {
    if (!port_combo_.contains(port.portName())) {
      should_update = true;
    }
  }
  if (should_update) {
    update();
  }
}
