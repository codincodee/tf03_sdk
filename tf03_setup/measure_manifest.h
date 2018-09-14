#ifndef MEASURE_MANIFEST_H
#define MEASURE_MANIFEST_H

#include <vector>
#include "plot_base.h"

namespace QtCharts {
  class QChartView;
}
class DistanceOverTimeChart;
class QLayout;
class QWidget;
class MeasureDevel;

class MeasureManifest : public PlotBase
{
public:
  MeasureManifest();
  ~MeasureManifest();
  bool Initialize();
  void SetPlotLayout(QLayout* layout);
  void AddWidget(QWidget* widget);
 private:
  std::vector<QWidget*> widgets_;
};

#endif // MEASURE_MANIFEST_H
