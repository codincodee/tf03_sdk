#ifndef FIRMWARE_UPDATE_PAGE_H
#define FIRMWARE_UPDATE_PAGE_H

#include "export.h"
#include "page_base.h"

class UpgradeFirmwareWidgets;
class QGridLayout;

class API FirmwareUpdatePage : public PageBase
{
public:
  FirmwareUpdatePage();
  void SetCore(std::shared_ptr<UpgradeFirmwareWidgets> core);
  void SetWidgetsLayout(QGridLayout* layout);
  bool Initialize() override;
  void Update() override;
private:
  std::shared_ptr<UpgradeFirmwareWidgets> core_;
  QGridLayout* widgets_layout_ = nullptr;
};

#endif // FIRMWARE_UPDATE_PAGE_H
