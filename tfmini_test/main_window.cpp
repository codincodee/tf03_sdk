#include "main_window.h"
#include "ui_main_window.h"
#include <tf03_common/connection_widget.h>
#include <tf03_common/tfmini_driver.h>
#include <tf03_common/measure_display_widget.h>
#include <tf03_common/config.h>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->setWindowIcon(QIcon(":/image/logo.png"));
  this->setWindowTitle(QString("TFMini Test v") + tf03_sdk_VERSION_STRING);

  auto driver = std::shared_ptr<TFMiniDriver>(new TFMiniDriver);

  auto measure_display = new MeasureDisplayWidget(this);
  measure_display->SetDriver(driver);
  ui->gridLayout->addWidget(measure_display);

  auto connection_widget = new ConnectionWidget(this);
  connection_widget->SetDriver(driver);
  ui->gridLayout->addWidget(connection_widget);
}

MainWindow::~MainWindow()
{
  delete ui;
}
