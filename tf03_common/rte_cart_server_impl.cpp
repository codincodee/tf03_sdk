#include "rte_cart_server_impl.h"
#include "tfmini_driver_impl.h"

RTECartServerImpl::RTECartServerImpl()
{
}

bool RTECartServerImpl::OnInitialized() {
  if (!RTECartServer::OnInitialized()) {
    return false;
  }
  auto sensor = Sensor();
  if (!sensor) {
    return false;
  }
  sensor_ = std::dynamic_pointer_cast<TFMiniDriverImpl>(sensor);
  if (!sensor_) {
    return false;
  }
  ////////////// Initialization //////////////


  return true;
}

void RTECartServerImpl::Spin() {
  RTECartServer::Spin();
}

bool RTECartServerImpl::I037BurnCallback(
    std::shared_ptr<std::list<CartStep>> steps) {
  return true;
}

bool RTECartServerImpl::I037TempBurnCallback(
    std::shared_ptr<std::list<CartStep>> steps) {
  return true;
}
bool RTECartServerImpl::AutoIntCheckCallback(
    std::shared_ptr<std::list<CartStep>> steps) {
  return true;
}
