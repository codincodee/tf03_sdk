#include "main_window.h"
#include "ui_main_window.h"
#include <tf03_common/connection_widget.h>
#include <tf03_common/tfmini_driver.h>
#include <tf03_common/measure_display_widget.h>
#include <tf03_common/config.h>
#include <tf03_common/command_block.h>
#include <tf03_common/tfmini_command_widgets.h>
#include <QDebug>
#include <tf03_common/tfmini_plot_widget.h>
#include <tf03_common/driver_server.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->DebugPushButton->setVisible(false);
  this->setWindowIcon(QIcon(":/image/logo.png"));
  this->setWindowTitle(QString("TFMini Test v") + tf03_sdk_VERSION_STRING);

  auto driver = std::shared_ptr<TFMiniDriver>(new TFMiniDriver);

  std::vector<CustomWidget*> widgets;

  auto measure_display = new MeasureDisplayWidget(this);
  measure_display->SetDriver(driver);
  ui->gridLayout->addWidget(measure_display);
  widgets.push_back(measure_display);

  auto connection_widget = new ConnectionWidget(this);
  connection_widget->SetDriver(driver);
  ui->gridLayout->addWidget(connection_widget);
  widgets.push_back(connection_widget);

  auto plot_widget = new TFMiniPlotWidget(this);
  plot_widget->SetDriver(driver);
  ui->gridLayout->addWidget(plot_widget);
  widgets.push_back(plot_widget);

  auto command_block = new CommandBlock(this);
  ui->gridLayout->addWidget(command_block);
  widgets.push_back(command_block);

  std::vector<CommonTFMiniWidget*> mini_widgets;

  auto set_output_format =
      new tfmini::SetOutputFormat(command_block);
  mini_widgets.push_back(set_output_format);

  std::vector<CommonCommandWidget*> ccws;
  for (auto& w : mini_widgets) {
    w->SetDriver(driver);
    ccws.push_back(w);
    widgets.push_back(w);
  }

  driver_server_.reset(new DriverServer);
  driver_server_->SetDriver(driver);
  if (!driver_server_->Initialize()) {
    exit(1);
  }

  for (auto& w : widgets) {
    driver_server_->RegisterAsyncMeasureCallback(
        std::bind(&CustomWidget::MeasureCallback, w, std::placeholders::_1));
  }
  command_block->LoadWidgets(ccws);
#ifdef DEVEL_DEBUG
  ui->DebugPushButton->setVisible(true);
  driver_ = driver;
#endif
  timer_id_ = startTimer(CustomWidget::DefaultTimerInterval());
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_DebugPushButton_clicked()
{
  if (driver_) {
    driver_->SetOutputFormat(TFMiniOutputFormat::b_29);
  }
}

void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  if (driver_server_) {
    driver_server_->Spin();
  }
}
