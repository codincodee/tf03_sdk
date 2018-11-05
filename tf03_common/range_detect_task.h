#ifndef RANGE_DETECT_TASK_H
#define RANGE_DETECT_TASK_H

#include "devel_mode_task.h"

class RangeDetectTask : public DevelModeTask
{
public:
  RangeDetectTask();
  void IncomingMeasure(const MeasureDevel& measure) override;
  void SetThreshold(const unsigned short& threshold);
private:
  std::unique_ptr<std::list<MeasureDevel>> stream_;
  std::atomic_uint16_t threshold_;
};

#endif // RANGE_DETECT_TASK_H
