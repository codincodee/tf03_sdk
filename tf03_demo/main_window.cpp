#include "main_window.h"
#include "ui_main_window.h"
#include <tf03_common/driver.h>
#include <QElapsedTimer>
#include <QComboBox>
#include <QDebug>
#include <tf03_common/command_echo_handler.h>
#include <QSerialPortInfo>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
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

  this->setWindowTitle("TF03 Demo v" + kVersion);
}

MainWindow::~MainWindow()
{
  driver_->Close();
}


void MainWindow::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }

  bool has_basic = false;
  MeasureBasic basic;
  auto measure_basic = driver_->LastMeasure();
  if (measure_basic) {
    has_basic = true;
    basic = *measure_basic;
  }

  bool has_devel = false;
  MeasureDevel devel;
  auto measure_devel = ToMeasureDevel(measure_basic);
  if (measure_devel) {
    has_devel = true;
    devel = *measure_devel;
  }

  command_echo_handler_->Probe();
}
