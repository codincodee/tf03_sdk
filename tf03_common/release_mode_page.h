#ifndef RELEASE_MODE_PAGE_H
#define RELEASE_MODE_PAGE_H

#include "export.h"
#include "page_base.h"

class QGridLayout;
class SetReleaseModeWidgets;

class API ReleaseModePage : public PageBase
{
public:
  ReleaseModePage();
  void SetWidgetsGridLayout(QGridLayout* layout);
  bool Initialize() override;
  void Update() override;
private:
  QGridLayout* layout_ = nullptr;
  std::shared_ptr<SetReleaseModeWidgets> core_;
};

#endif // RELEASE_MODE_PAGE_H
