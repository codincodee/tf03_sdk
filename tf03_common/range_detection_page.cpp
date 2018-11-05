#include "range_detection_page.h"
#include "command_echo_widgets.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include "plot_base.h"
#include "distance_over_time_chart_with_threshold.h"

RangeDetectionPage::RangeDetectionPage()
{

}

void RangeDetectionPage::SetWidgetsLayout(QGridLayout *layout) {
  layout_ = layout;
}

void RangeDetectionPage::SetPlotLayout(QLayout *layout) {
  if (!layout) {
    return;
  }
  plot_.reset(new PlotBase);
  plot_->SetLayout(layout);
  auto chart = new DistanceOverTimeChartWithThreshold();
  chart->SetTimeSpan(400);
  chart->SetHigh(20.0f);
  chart->SetLow(0.0f);
  plot_->SetChart(chart);
}

bool RangeDetectionPage::Initialize() {
  if (!layout_) {
    return false;
  }
  if (plot_) {
    if (!plot_->Initialize()) {
      return false;
    }
  }
  core_.reset(new RangeValidityWidgets);
  core_->driver = GetDriver();
  core_->echo_handler = EchoHandler();
  core_->SetupUI();
  layout_->addWidget(core_->button, 0, 0);
  layout_->addWidget(core_->indicator, 0, 1);
  layout_->addWidget(core_->label, 0, 2);
  layout_->addWidget(core_->status, 0, 3);
  return true;
}

void RangeDetectionPage::Update() {
  core_->Update();
}

void RangeDetectionPage::OnMeasured(const MeasureBasic &measure) {
  PageBase::OnMeasured(measure);
  if (plot_) {
    plot_->IncomingMeasure(measure);
  }
}
