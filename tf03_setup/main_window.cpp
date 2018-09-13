#include "main_window.h"
#include "ui_main_window.h"
#include "driver.h"
#include <QElapsedTimer>
#include <QComboBox>
#include <QDebug>
#include "command_echo_handler.h"
#include "command_echo_widgets_manager.h"
#include <QSerialPortInfo>
#include <QDesktopWidget>
#include "apd_page.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

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

  command_echo_widgets_manager_.reset(new CommandEchoWidgetsManager);
  command_echo_widgets_manager_->SetUIGrid(ui->CommandEchoGridLayout);
  command_echo_widgets_manager_->SetDriver(driver_);
  command_echo_widgets_manager_->SetEchoHandler(command_echo_handler_);
  command_echo_widgets_manager_->SetupFirmwareLayout(ui->FirmwareGridLayout);
  command_echo_widgets_manager_->SetParentWidget(this);
  command_echo_widgets_manager_->LoadWidgets();

  connect_button_current_lingual_ = kConnectPushButtonText;
  auto baud_rates = Driver::BaudRates();
  for (auto& rate : baud_rates) {
    ui->BaudRateComboBox->addItem(QString::number(rate));
  }

  SetupUIText();

#ifndef USE_APD_EXPERIMENT_PAGE
  ui->tabWidget->removeTab(1);
#else
  apd_page_.reset(new APDPage);
#endif
  if (apd_page_) {
    apd_page_->SetPlotLayout(ui->APDPagePlotVerticalLayout);
    apd_page_->SetAPDDisplayLabel(ui->APDPageAPDVoltageLabel);
    apd_page_->SetTemperatureDisplayLabel(ui->APDPageTemperatureLabel);
    apd_page_->SetStartPushButton(ui->APDPageStartPushButton);
    apd_page_->SetProgressBar(ui->APDPageProgressBar);
    apd_page_->SetAPDFromLineEdit(ui->APDPageAPDFromLineEdit);
    apd_page_->SetAPDToLineEdit(ui->APDPageAPDToLineEdit);
    apd_page_->SetThresholdLineEdit(ui->APDPageThresholdLineEdit);
    apd_page_->SetDriver(driver_);
    apd_page_->SetCmdEchoHandler(command_echo_handler_);
    apd_page_->SetStatusLabel(ui->APDPageStatusLabel);
    if (!apd_page_->Initialize()) {
      apd_page_.reset();
    }
  }

  ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
  driver_->Close();
}


void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }

#ifdef USE_APD_EXPERIMENT_PAGE
  if (apd_page_) {
    apd_page_->Update();
  }
#endif
#ifdef AUTOMATIC_SERIAL_CONNECTION
  if (driver_->DetectAndAutoConnect()) {
    connect_button_current_lingual_ = kDisconnectPushButtonText;
    ui->ConnectPushButton->setText(
        which_lingual(connect_button_current_lingual_));
  }
#endif
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
  command_echo_widgets_manager_->Update();

  if (apd_page_) {
    auto measure_devel = ToMeasureDevel(measure_basic);
    if (measure_devel) {
      apd_page_->IncomingMeasure(*measure_devel);
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
  ui->LanguageLabel->setText(which_lingual(kLanguageLabelText));
  ui->DistanceHintLabel->setText(which_lingual(kDistanceLabelText));
  ui->FrequencyHintLabel->setText(which_lingual(kFrequencyLabelText));
  ui->PortNameLabel->setText(which_lingual(kPortNameLabelText));
  ui->BaudRateLabel->setText(which_lingual(kBaudRateLabelText));
  ui->ConnectPushButton->setText(
      which_lingual(connect_button_current_lingual_));
  ui->tabWidget->setTabText(0, which_lingual({"Setup", "配置"}));
  this->setWindowTitle(which_lingual(kWindowTitle) +  " v" + kVersion);

  if (command_echo_widgets_manager_) {
    command_echo_widgets_manager_->UpdateUITexts();
  }
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
