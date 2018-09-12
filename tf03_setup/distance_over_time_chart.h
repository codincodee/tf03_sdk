#ifndef DISTANCE_OVER_TIME_CHART_H
#define DISTANCE_OVER_TIME_CHART_H

#include <QtCharts/qchart.h>

namespace QtCharts {
  class QLineSeries;
}

class DistanceOverTimeChart : public QtCharts::QChart {
  Q_OBJECT
public:
  DistanceOverTimeChart();
  virtual ~DistanceOverTimeChart();
  bool AddPoint(const float& meter, const int& millisecond);
  void SetTimeWindow(const int& millisecond);
  float GetMin() const;
  float GetMax() const;
  void SetCeiling(const float& ceiling);
  void SetFloor(const float& floor);
  QtCharts::QLineSeries* Series();
  float CurrentStandardDeviation();
  void CurrentAverageAndStandardDeviation(float& average, float& stddev);
  void Clear();
protected:
  bool AddPoint(
      const float &meter,
      const int &millisecond,
      QtCharts::QLineSeries* series);
private:
  QtCharts::QLineSeries* line_series_;
  float min_;
  float max_;
  float ceiling_;
  float floor_;
};

#endif // DISTANCE_OVER_TIME_CHART_H
