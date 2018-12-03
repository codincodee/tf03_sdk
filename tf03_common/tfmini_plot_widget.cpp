#include "tfmini_plot_widget.h"
#include "ui_tfmini_plot_widget.h"
#include "distance_over_time_chart.h"
#include <QtCharts/qchartview.h>
#include "tfmini_driver.h"
#include "static_unique_ptr_cast.h"
#include <QDebug>

TFMiniPlotWidget::TFMiniPlotWidget(QWidget *parent) :
  CustomWidget(parent),
  ui(new Ui::TFMiniPlotWidget)
{
  ui->setupUi(this);

  chart_ = new DistanceOverTimeChart();
  chart_->SetCeiling(30.0f);
  chart_->SetFloor(8.0f);
  chart_->SetTimeSpan(400);

  ui->gridLayout->addWidget(new QtCharts::QChartView(chart_));
}

TFMiniPlotWidget::~TFMiniPlotWidget()
{
  delete ui;
}

void TFMiniPlotWidget::OnMeasureCalled(std::unique_ptr<MeasureBasic> measure) {
  if (measure) {
    chart_->AddPoint(measure->dist / 1000.0f, measure->id);
  }
}

void TFMiniPlotWidget::CommonTimerCallback() {
}

void TFMiniPlotWidget::SetDriver(std::shared_ptr<TFMiniDriver> driver) {
  driver_ = driver;
}
