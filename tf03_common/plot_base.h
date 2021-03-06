#ifndef PLOT_BASE_H
#define PLOT_BASE_H

#include <QObject>
#include "distance_over_time_chart.h"
#include "export.h"

class QLayout;
class DistanceOverTimeChart;
namespace QtCharts {
  class QChartView;
}
struct API MeasureBasic;
struct API MeasureDevel;

class API PlotBase : public QObject
{
public:
  PlotBase();
  virtual ~PlotBase();
  void SetChart(DistanceOverTimeChart* main_chart);
  void SetLayout(QLayout* layout);
  bool Initialize();
  void SetVisible(const bool& visible);
  void IncomingMeasure(const MeasureBasic& measure);
  void IncomingMeasureRawDist2(const MeasureDevel& measure);
  DistanceOverTimeChart& GetPlot();
  DistanceOverTimeChart* GetPlotPtr();
private:
  QLayout* plot_layout_ = nullptr;
  QtCharts::QChartView* main_chart_view_ = nullptr;
  DistanceOverTimeChart* main_chart_ = nullptr;
  std::vector<QWidget*> widgets_;
  bool visible_ = true;
};

#endif // PLOT_BASE_H
