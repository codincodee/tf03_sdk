#ifndef DISTANCE_OVER_TIME_CHART_WITH_THRESHOLD_H
#define DISTANCE_OVER_TIME_CHART_WITH_THRESHOLD_H

#include "export.h"
#include "distance_over_time_chart.h"

class DistanceOverTimeChartWithThreshold  : public DistanceOverTimeChart
{
public:
  DistanceOverTimeChartWithThreshold();
  bool AddPoint(const float &meter, const int &millisecond) override;
  void SetLow(const float& low);
  void SetHigh(const float& high);
private:
  float low_;
  float high_;
};

#endif // DISTANCE_OVER_TIME_CHART_WITH_THRESHOLD_H
