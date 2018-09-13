#ifndef DEVEL_MODE_TASK_H
#define DEVEL_MODE_TASK_H

#include "driver.h"

struct MeasureDevel;

class DevelModeTask
{
 public:
  virtual ~DevelModeTask();
  virtual void IncomingMeasure(const MeasureDevel& measure) = 0;
  void SetDriver(Driver* driver);
 protected:
  void EnqueueReceivedMessages(Message message);
 private:
  Driver* driver_ = nullptr;
};

#endif // DEVEL_MODE_TASK_H
