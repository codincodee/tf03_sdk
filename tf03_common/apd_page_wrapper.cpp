#include "apd_page_wrapper.h"
#include "apd_page.h"

APDPageWrapper::APDPageWrapper()
{

}

void APDPageWrapper::SetAPDPageCore(std::shared_ptr<APDPage> core) {
  core_ = core;
}

std::shared_ptr<APDPage> APDPageWrapper::GetCore() {
  return core_;
}

void APDPageWrapper::Update() {
  if (core_) {
    core_->Update();
  }
}

void APDPageWrapper::OnMeasured(const MeasureDevel &measure) {
  if (core_) {
    core_->IncomingMeasure(measure);
  }
}
