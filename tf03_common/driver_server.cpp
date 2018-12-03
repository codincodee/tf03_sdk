#include "driver_server.h"
#include "static_unique_ptr_cast.h"

DriverServer::DriverServer()
{

}

DriverServer::~DriverServer() {}

void DriverServer::SetDriver(std::shared_ptr<DriverBase> driver) {
  driver_ = driver;
}

bool DriverServer::Initialize() {
  return true;
}

void DriverServer::RegisterAsyncMeasureCallback(MeasureCallback func) {
  measure_callbacks_.push_back(std::move(func));
}

void DriverServer::Spin() {
  if (!driver_) {
    return;
  }
  auto measure = driver_->Measure();
  if (!measure) {
    return;
  }
  if (measure->id == measure_id_) {
    return;
  }
  for (auto& func : measure_callbacks_) {
    auto m = dynamic_unique_ptr_cast<MeasureBasic>(measure->Clone());
    if (m) {
      func(std::move(m));
    }
  }
}
