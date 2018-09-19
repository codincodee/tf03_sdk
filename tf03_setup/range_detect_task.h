#ifndef RANGE_DETECT_TASK_H
#define RANGE_DETECT_TASK_H

#include "devel_mode_task.h"

class RangeDetectTask : public DevelModeTask
{
public:
  RangeDetectTask();
  void IncomingMeasure(const MeasureDevel& measure) override;
private:
  std::unique_ptr<std::list<MeasureDevel>> stream_;
};

#endif // RANGE_DETECT_TASK_H
