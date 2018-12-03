#ifndef DRIVER_SERVER_H
#define DRIVER_SERVER_H

#include "export.h"
#include <functional>
#include "driver_base.h"

class API DriverServer
{
public:
  DriverServer();
  virtual ~DriverServer();
  void SetDriver(std::shared_ptr<DriverBase> driver);
  virtual bool Initialize();
  using MeasureCallback = std::function<void(std::unique_ptr<MeasureBasic>)>;
  void RegisterAsyncMeasureCallback(MeasureCallback func);
  void Spin();
private:
  std::vector<MeasureCallback> measure_callbacks_;
  std::shared_ptr<DriverBase> driver_;
  unsigned long measure_id_ = 0;
};

#endif // DRIVER_SERVER_H
