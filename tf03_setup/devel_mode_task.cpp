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
