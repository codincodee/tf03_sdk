#include "page_base.h"

PageBase::PageBase()
{

}

PageBase::~PageBase() {}

void PageBase::SetCommandEchoHandler(
    std::shared_ptr<CommandEchoHandler> echo_handler) {
  echo_handler_ = echo_handler;
}

std::shared_ptr<CommandEchoHandler> PageBase::EchoHandler() {
  return echo_handler_;
}
