#include "distance_over_time_chart_with_threshold.h"
#include <QValueAxis>
#include <QDebug>
#include <QLineSeries>

using ThresholdChart = DistanceOverTimeChartWithThreshold;

ThresholdChart::DistanceOverTimeChartWithThreshold() : DistanceOverTimeChart()
{

}

bool ThresholdChart::AddPoint(
    const float &meter_in, const int &millisecond) {
  float meter = meter_in;
  if (meter > high_) {
    meter = high_;
  }
  if (meter < low_) {
    meter = low_;
  }

  auto line_series = line_series_;
  if (!line_series) {
    return false;
  }
  if (line_series->count() > 0) {
    if (millisecond <= line_series->at(line_series->count() - 1).x()) {
      return false;
    }
  }
  *line_series << QPointF(millisecond, meter);

  while (line_series->count()) {
    if ((line_series->at(0).x() + time_span_) < millisecond) {
      line_series->removePoints(0, 1);
    } else {
      break;
    }
  }

  if (!line_series->count()) {
    return false;
  }

#if 0
  float min = line_series->at(0).y();
  float max = min;
  for (auto i = 0; i < line_series->count(); ++i) {
    if (line_series->at(i).y() > max) {
      max = line_series->at(i).y();
    }
    if (line_series->at(i).y() < min) {
      min = line_series->at(i).y();
    }
  }
  if (max > ceiling_) {
    max = ceiling_;
  }
  if (max < floor_) {
    max = floor_;
  }
#else
  float min = low_;
  float max = high_;
#endif

  // To make the plot look nicer, we currently set MIN to 0.0f;
//  min = 0.0f;

  this->removeSeries(line_series);
  this->addSeries(line_series);

#if 0
  if (line_series_ == line_series) {
    auto axis_y = this->axisY();
    axis_y->setRange(min, max);
    min_ = min;
    max_ = max;
    this->setAxisY(axis_y, line_series);
    axis_y->setLabelsFont(label_font_);
  }
#else
  auto axis_y = this->axisY();
  axis_y->setRange(min, max);
  this->setAxisY(axis_y, line_series);
  axis_y->setLabelsFont(label_font_);
#endif
}

void ThresholdChart::SetLow(const float &low) {
  low_ = low;
}

void ThresholdChart::SetHigh(const float &high) {
  high_ = high;
}
