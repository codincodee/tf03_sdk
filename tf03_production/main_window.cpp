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
#include <tf03_common/connection_page.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  if (!IsAPDExperimentPagUsed()) {
    exit(1);
  }

  if (!IsDevelModeSupported()) {
    exit(1);
  }

  ui->setupUi(this);

  this->setWindowIcon(QIcon(":/image/logo.png"));
  QRect desktop = QApplication::desktop()->availableGeometry();
  auto window = this->rect();
  this->move(desktop.topLeft() - window.topLeft());
  this->move(QPoint(desktop.width() / 2 - window.width() / 2, 0));

  this->resize(1000, 800);

  driver_.reset(new Driver);
  driver_->Open();
  timer_id_ = startTimer(100);

  command_echo_handler_.reset(new CommandEchoHandler);
  command_echo_handler_->SetDriver(driver_);

  ui->tabWidget->setFont(PageBase::GetCommonFont());

  std::shared_ptr<APDPage> apd_core(new APDPage);
  apd_page_.reset(new APDPageWrapper);
  apd_page_->SetAPDPageCore(apd_core);

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

  pages_.push_back(apd_page_);

  connection_page_.reset(new ConnectionPage);
  connection_page_->SetBaudRateComboxBox(ui->BaudRateComboBox);
  connection_page_->SetPortComboBox(ui->SerialPortComboBox);
  connection_page_->SetDistanceDisplayLabel(ui->DistanceDisplayLabel);
  connection_page_->SetFrequencyDisplayLabel(ui->FrequencyDisplayLabel);
  connection_page_->SetConnectPushButton(ui->ConnectPushButton);
  connection_page_->SetVersionLayout(ui->ConnectionPageVersionGridLayout);
  connection_page_->SetCommandEchoHandler(command_echo_handler_);
  connection_page_->SetDriver(driver_);
  if (!connection_page_->Initialize()) {
    exit(1);
  }
  pages_.push_back(connection_page_);

  ui->tabWidget->setCurrentIndex(0);
  this->setWindowTitle("TF03生产上位机 v" + kVersion);

  ui->NextStepPushButton->setFont(PageBase::GetCommonFont());
  ui->PreviousStepPushButton->setFont(PageBase::GetCommonFont());
}

MainWindow::~MainWindow()
{
  driver_->Close();
}


void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }

  MeasureBasic measure;
  auto measure_basic = driver_->LastMeasure();
  if (measure_basic) {
    measure = *measure_basic;
  }

  auto measure_devel = ToMeasureDevel(measure_basic);

  command_echo_handler_->Probe();

  if (measure_devel) {
    for(auto& page : pages_) {
      page->OnMeasured(*measure_devel);
    }
  }

  for(auto& page : pages_) {
    page->Update();
  }
}

void MainWindow::on_NextStepPushButton_clicked()
{
  auto total = ui->tabWidget->count();
  auto current = ui->tabWidget->currentIndex();
  ++current;
  if (current >= total) {
    current = 0;
  }
  ui->tabWidget->setCurrentIndex(current);
}

void MainWindow::on_PreviousStepPushButton_clicked()
{
  auto total = ui->tabWidget->count();
  auto current = ui->tabWidget->currentIndex();
  --current;
  if (current < 0) {
    current = total - 1;
  }
  ui->tabWidget->setCurrentIndex(current);
}
