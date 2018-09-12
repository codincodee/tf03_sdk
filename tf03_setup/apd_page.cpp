#include "apd_page.h"
#include "distance_over_time_chart.h"
#include <QtCharts/QChartView>
#include <QLayout>
#include "parsed.h"

APDPage::APDPage()
{

}

APDPage::~APDPage() {
  if (main_chart_) {
    delete main_chart_;
    main_chart_ = nullptr;
  }

  if (main_chart_view_) {
    delete main_chart_view_;
    main_chart_view_ = nullptr;
  }
}

void APDPage::SetPlotLayout(QLayout *layout) {
  plot_layout_ = layout;
}

bool APDPage::Initialize() {
  main_chart_ = new DistanceOverTimeChart();
  main_chart_->SetCeiling(150.0f);
  main_chart_->SetFloor(5.0f);
  main_chart_view_ = new QtCharts::QChartView(main_chart_);
  if (plot_layout_) {
    plot_layout_->addWidget(main_chart_view_);
  }
  return true;
}

void APDPage::IncomingMeasure(const MeasureBasic &measure) {
  if (!main_chart_) {
    return;
  }
  static int cnt = 0;
  main_chart_->AddPoint(measure.dist / 100.f, cnt);
  cnt += 100;
}
