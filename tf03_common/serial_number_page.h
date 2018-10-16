#ifndef SERIAL_NUMBER_PAGE_H
#define SERIAL_NUMBER_PAGE_H

#include "export.h"
#include "page_base.h"

class WriteSerialNumberPlusWidgets;
class SerialNumberWidgets;
class QGridLayout;

class API SerialNumberPage : public PageBase
{
public:
  SerialNumberPage();
  void SetWidgetsLayout(QGridLayout* layout);
  bool Initialize() override;
  void Update() override;
private:
  QGridLayout* layout_ = nullptr;
  std::shared_ptr<WriteSerialNumberPlusWidgets> write_;
  std::shared_ptr<SerialNumberWidgets> read_;
};

#endif // SERIAL_NUMBER_PAGE_H
