#include "apd_page_wrapper.h"

APDPageWrapper::APDPageWrapper()
{

}

void APDPageWrapper::SetAPDPageCore(std::shared_ptr<APDPage> core) {
  core_ = core;
}
