#include "plot_base.h"
#include <QWidget>
#include "distance_over_time_chart.h"
#include <QtCharts/qchartview.h>
#include <QLayout>
#include "parsed.h"

PlotBase::PlotBase()
{

}

PlotBase::~PlotBase() {
  if (main_chart_) {
    delete main_chart_;
    main_chart_ = nullptr;
  }

  if (main_chart_view_) {
    delete main_chart_view_;
    main_chart_view_ = nullptr;
  }
}

void PlotBase::SetChart(DistanceOverTimeChart *main_chart) {
  main_chart_ = main_chart;
}

void PlotBase::SetLayout(QLayout *layout) {
  plot_layout_ = layout;
}

bool PlotBase::Initialize() {
  if (!plot_layout_) {
    return false;
  }
  if (!main_chart_) {
    main_chart_ = new DistanceOverTimeChart();
    main_chart_->SetCeiling(150.0f);
    main_chart_->SetFloor(5.0f);
    main_chart_->SetTimeSpan(400);
  }
  main_chart_view_ = new QtCharts::QChartView(main_chart_);
  if (plot_layout_) {
    plot_layout_->addWidget(main_chart_view_);
  }

  widgets_.push_back(main_chart_view_);
  return true;
}

void PlotBase::SetVisible(const bool &visible) {
  visible_ = visible;
  for (auto& widget : widgets_) {
    if (widget) {
      widget->setVisible(visible);
    }
  }
}

void PlotBase::IncomingMeasure(const MeasureBasic& measure) {
  if (!visible_) {
    return;
  }
  main_chart_->AddPoint(measure.dist / 100.f, measure.id);
}

void PlotBase::IncomingMeasureRawDist2(const MeasureDevel &measure) {
  if (!visible_) {
    return;
  }
  main_chart_->AddPoint(measure.raw_dist2 / 100.f, measure.id);
}

DistanceOverTimeChart& PlotBase::GetPlot() {
  return *main_chart_;
}

DistanceOverTimeChart* PlotBase::GetPlotPtr() {
  return main_chart_;
}
