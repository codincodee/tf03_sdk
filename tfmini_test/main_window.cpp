#include "main_window.h"
#include "ui_main_window.h"
#include <tf03_common/connection_widget.h>
#include <tf03_common/tfmini_driver_impl.h>
#include <tf03_common/measure_display_widget.h>
#include <tf03_common/config.h>
#include <tf03_common/command_block.h>
#include <tf03_common/tfmini_command_widgets.h>
#include <QDebug>
#include <tf03_common/tfmini_plot_widget.h>
#include <tf03_common/driver_server.h>
#include <tf03_common/mini_rte_cart.h>
#include <tf03_common/rte_cart_server_impl.h>
#include <tf03_common/rte_cart_widget.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->DebugPushButton->setVisible(false);
  this->setWindowIcon(QIcon(":/image/logo.png"));
  this->setWindowTitle(QString("TFMini Test v") + tf03_sdk_VERSION_STRING);

  std::shared_ptr<TFMiniDriverImpl> driver(new TFMiniDriverImpl);

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

  auto trigger_inttime_measure =
      new tfmini::TriggerIntTimeMeasure(command_block);
  mini_widgets.push_back(trigger_inttime_measure);

  auto set_timer =
      new tfmini::SetTimer(command_block);
  mini_widgets.push_back(set_timer);

  auto set_metric_unit =
      new tfmini::SetMetricUnit(command_block);
  mini_widgets.push_back(set_metric_unit);

  auto set_inttime_mode =
      new tfmini::SetIntTimeMode(command_block);
  mini_widgets.push_back(set_inttime_mode);

  std::vector<CommonCommandWidget*> ccws;
  for (auto& w : mini_widgets) {
    w->SetDriver(driver);
    ccws.push_back(w);
    widgets.push_back(w);
  }

  std::shared_ptr<MiniRTECart> cart_driver(new MiniRTECart);
  cart_driver->RegisterRetrieveMeasuresCallback(
      [driver](){return driver->GetMeasures();});
//  cart_driver->RegisterMeasureCallback([driver](){return driver->Measure();});
//  cart_driver->RegisterIntTimeTriggerCallback(
//      [driver](int i){driver->TriggerIntTimeMeasure(i);});
//  auto cart_driver = std::shared_ptr<CartDriver>(new CartDriver);

  auto cart_connect_widget = new ConnectionWidget(this);
  cart_connect_widget->SetDriver(cart_driver);
  ui->gridLayout->addWidget(cart_connect_widget);
  widgets.push_back(cart_connect_widget);

  auto cart_driver_server =
      std::shared_ptr<RTECartServerImpl>(new RTECartServerImpl);
  cart_driver_server->SetMiniDriver(driver);
  cart_driver_server->SetDriver(cart_driver);
  //////////////// configurations ////////////

  ////////////////////////////////////////////
  if (!cart_driver_server->Initialize()) {
    exit(1);
  }

  auto cart_test_widget = new RTECartWidget(this);
  cart_test_widget->SetDriverServer(cart_driver_server);
  ui->gridLayout->addWidget(cart_test_widget);
  widgets.push_back(cart_test_widget);

  auto driver_server = std::shared_ptr<DriverServer>(new DriverServer);
  driver_server->SetDriver(driver);
  if (!driver_server->Initialize()) {
    exit(1);
  }
  driver_server_ = driver_server;

  for (auto& w : widgets) {
    driver_server->RegisterAsyncMeasureCallback(
        std::bind(&CustomWidget::MeasureCallback, w, std::placeholders::_1));
  }
  command_block->LoadWidgets(ccws);
#ifdef DEVEL_DEBUG
  ui->DebugPushButton->setVisible(true);
  driver_ = driver;
  cart_ = cart_driver;
#endif
  timer_id_ = startTimer(CustomWidget::DefaultTimerInterval());
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_DebugPushButton_clicked()
{
//  if (driver_) {
//    driver_->SetOutputFormat(TFMiniOutputFormat::b_29);
//  }
  if (cart_) {
    cart_->GoOn();
  }
}

void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }
//  if (driver_server_) {
//    driver_server_->Spin();
//  }
}

void MainWindow::on_DebugAlphaPushButton_clicked()
{
  if (cart_) {
    cart_->SetDistance(1200);
    cart_->Start();
  }
}
