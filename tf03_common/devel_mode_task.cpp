#include "devel_mode_task.h"

DevelModeTask::~DevelModeTask()
{

}

void DevelModeTask::SetDriver(Driver *driver) {
  driver_ = driver;
}

void DevelModeTask::EnqueueReceivedMessages(Message message) {
  if (!driver_) {
    return;
  }
  driver_->EnqueueReceivedMessages(std::move(message));
}

void DevelModeTask::Start() {
  is_on_ = true;
}

void DevelModeTask::Stop() {
  is_on_ = false;
}

bool DevelModeTask::IsStarted() const {
  return is_on_;
}
