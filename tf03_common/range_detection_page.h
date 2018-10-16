#ifndef RANGE_DETECTION_PAGE_H
#define RANGE_DETECTION_PAGE_H

#include "export.h"
#include "page_base.h"

class RangeValidityWidgets;
class QGridLayout;

class API RangeDetectionPage : public PageBase
{
public:
  RangeDetectionPage();
  void SetWidgetsLayout(QGridLayout* layout);
  bool Initialize() override;
  void Update() override;
private:
  std::shared_ptr<RangeValidityWidgets> core_;
  QGridLayout* layout_ = nullptr;
};

#endif // RANGE_DETECTION_PAGE_H
