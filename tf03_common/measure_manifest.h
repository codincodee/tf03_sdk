#ifndef MEASURE_MANIFEST_H
#define MEASURE_MANIFEST_H

#include <vector>
#include "plot_base.h"
#include <memory>
#include "export.h"

namespace QtCharts {
  class QChartView;
}
class DistanceOverTimeChart;
class QLayout;
class QWidget;
struct MeasureDevel;
struct MeasureBasic;
class QLabel;
class QGridLayout;

struct API ManifestBase {
  virtual ~ManifestBase();
  std::vector<std::pair<QLabel*, QLabel*>> labels;
  virtual void Setup(QGridLayout* layout) = 0;
  void SetFormat();
  void SetVisible(const bool& visible);
  void Clear();
};

struct API ManifestDevel : public ManifestBase {
  ~ManifestDevel();
  void Setup(QGridLayout* layout) override;
  void IncomingMeasure(const MeasureDevel& measure);
};

struct API ManifestBasic : public ManifestBase {
  void Setup(QGridLayout* layout) override;
};

class API MeasureManifest : public PlotBase
{
public:
  MeasureManifest();
  ~MeasureManifest();
  bool Initialize();
  void AddWidget(QWidget* widget);
  void IncomingMeasure(const MeasureDevel& measure);
  void IncomingMeasure(const MeasureBasic& measure);
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
