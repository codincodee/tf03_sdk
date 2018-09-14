#ifndef PLOT_BASE_H
#define PLOT_BASE_H

#include <QObject>

class QLayout;
class DistanceOverTimeChart;
namespace QtCharts {
  class QChartView;
}
struct MeasureDevel;

class PlotBase : public QObject
{
public:
  PlotBase();
  virtual ~PlotBase();
  void SetLayout(QLayout* layout);
  bool Initialize();
  void SetVisible(const bool& visible);
  void IncomingMeasure(const MeasureDevel& measure);
  DistanceOverTimeChart& GetPlot();
private:
  QLayout* plot_layout_ = nullptr;
  QtCharts::QChartView* main_chart_view_ = nullptr;
  DistanceOverTimeChart* main_chart_ = nullptr;
  std::vector<QWidget*> widgets_;
  bool visible_ = true;
};

#endif // PLOT_BASE_H
