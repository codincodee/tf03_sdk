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

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->DebugPushButton->setVisible(false);
  this->setWindowIcon(QIcon(":/image/logo.png"));
  this->setWindowTitle(QString("TFMini Test v") + tf03_sdk_VERSION_STRING);

  auto driver = std::shared_ptr<TFMiniDriver>(new TFMiniDriver);

  auto measure_display = new MeasureDisplayWidget(this);
  measure_display->SetDriver(driver);
  ui->gridLayout->addWidget(measure_display);

  auto connection_widget = new ConnectionWidget(this);
  connection_widget->SetDriver(driver);
  ui->gridLayout->addWidget(connection_widget);

  auto plot_widget = new TFMiniPlotWidget(this);
  plot_widget->SetDriver(driver);
  ui->gridLayout->addWidget(plot_widget);

  auto command_block = new CommandBlock(this);
  ui->gridLayout->addWidget(command_block);
  std::vector<std::shared_ptr<CommonTFMiniWidget>> mini_widgets;

  auto set_output_format =
      std::shared_ptr<tfmini::SetOutputFormat>(
          new tfmini::SetOutputFormat(command_block));
  mini_widgets.push_back(set_output_format);

  std::vector<std::shared_ptr<CommonCommandWidget>> ccws;
  for (auto& w : mini_widgets) {
    w->SetDriver(driver);
    ccws.push_back(w);
  }

  command_block->LoadWidgets(ccws);
#ifdef DEVEL_DEBUG
  ui->DebugPushButton->setVisible(true);
  driver_ = driver;
#endif
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
