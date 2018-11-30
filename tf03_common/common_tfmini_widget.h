#ifndef COMMON_TFMINI_WIDGET_H
#define COMMON_TFMINI_WIDGET_H

#include "common_command_widget.h"
#include <memory>
#include "tfmini_driver.h"
#include "tfmini_driver_server.h"
#include "export.h"

class API CommonTFMiniWidget : public CommonCommandWidget
{
public:
  CommonTFMiniWidget(QWidget* parent);
  virtual ~CommonTFMiniWidget();
  void SetDriver(std::shared_ptr<TFMiniDriver> driver);
  void SetDriverServer(std::shared_ptr<TFMiniDriverServer> server);
protected:
  std::shared_ptr<TFMiniDriver> driver;
  std::shared_ptr<TFMiniDriverServer> server;
private:
};

#endif // COMMON_TFMINI_WIDGET_H
