#ifndef APD_EXP_TASK_H
#define APD_EXP_TASK_H

#include "devel_mode_task.h"
#include <memory>
#include <list>

class APDExpTask : public DevelModeTask
{
public:
  APDExpTask();
  void IncomingMeasure(const MeasureDevel& measure) override;
 private:
  std::unique_ptr<std::list<MeasureDevel>> stream_;
};

#endif // APD_EXP_TASK_H
