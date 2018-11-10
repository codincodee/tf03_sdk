#ifndef RANGE_DETECTION_PAGE_H
#define RANGE_DETECTION_PAGE_H

#include "export.h"
#include "page_base.h"

class RangeValidityWidgets;
class QGridLayout;
class PlotBase;
class QLayout;

class API RangeDetectionPage : public PageBase
{
public:
  RangeDetectionPage();
  void SetWidgetsLayout(QGridLayout* layout);
  void SetTipsLabel(QLabel* label);
  void SetPlotLayout(QLayout* layout);
  bool Initialize() override;
  void Update() override;
  void OnMeasured(const MeasureDevel &measure);
private:
  std::shared_ptr<PlotBase> plot_;
  std::shared_ptr<RangeValidityWidgets> core_;
  QGridLayout* layout_ = nullptr;
  const float kThreshold = 18.0f;
};

#endif // RANGE_DETECTION_PAGE_H
