#ifndef APD_PAGE_H
#define APD_PAGE_H

struct MeasureBasic;
// #include <QLayout>
class QLayout;
class DistanceOverTimeChart;
namespace QtCharts {
  class QChartView;
}

class APDPage
{
public:
  APDPage();
  ~APDPage();
  void SetPlotLayout(QLayout* layout);
  bool Initialize();
  void IncomingMeasure(const MeasureBasic& measure);
private:
  QLayout* plot_layout_ = nullptr;
  QtCharts::QChartView* main_chart_view_ = nullptr;
  DistanceOverTimeChart* main_chart_ = nullptr;
};

#endif // APD_PAGE_H
