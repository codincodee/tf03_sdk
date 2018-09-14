#ifndef MEASURE_MANIFEST_H
#define MEASURE_MANIFEST_H

#include <vector>
#include "plot_base.h"
#include <memory>

namespace QtCharts {
  class QChartView;
}
class DistanceOverTimeChart;
class QLayout;
class QWidget;
class MeasureDevel;
class QLabel;
class QGridLayout;

struct ManifestBase {
  virtual ~ManifestBase();
  std::vector<std::pair<QLabel*, QLabel*>> labels;
  virtual void Setup(QGridLayout* layout) = 0;
  void SetFormat();
  void SetVisible(const bool& visible);
};

struct ManifestDevel : public ManifestBase {
  ~ManifestDevel();
  void Setup(QGridLayout* layout) override;
  void IncomingMeasure(const MeasureDevel& measure);
  void Clear();
};

struct ManifestBasic : public ManifestBase {
  void Setup(QGridLayout* layout) override;
};

class MeasureManifest : public PlotBase
{
public:
  MeasureManifest();
  ~MeasureManifest();
  bool Initialize();
  void AddWidget(QWidget* widget);
  void IncomingMeasure(const MeasureDevel& measure);
  void SetManifestGrid(QGridLayout* layout);
  void SetVisible(const bool& visible);
 private:
  QGridLayout* manifest_grid_ = nullptr;
  std::vector<QWidget*> widgets_;
  std::shared_ptr<ManifestDevel> manifest_devel_;
  std::shared_ptr<ManifestBasic> manifest_basic_;
  std::shared_ptr<ManifestBase> current_manifest_;
};

#endif // MEASURE_MANIFEST_H
