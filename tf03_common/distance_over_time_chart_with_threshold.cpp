#include "distance_over_time_chart_with_threshold.h"
#include <QValueAxis>
#include <QDebug>
#include <QLineSeries>

using ThresholdChart = DistanceOverTimeChartWithThreshold;

ThresholdChart::DistanceOverTimeChartWithThreshold() : DistanceOverTimeChart()
{
  threshold_line_series_ = new QtCharts::QLineSeries;
  this->addSeries(threshold_line_series_);
}

bool ThresholdChart::AddPoint(
    const float &meter, const int &millisecond) {
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
  *threshold_line_series_ << QPointF(millisecond, threshold_);

  while (line_series->count()) {
    if ((line_series->at(0).x() + time_span_) < millisecond) {
      line_series->removePoints(0, 1);
      threshold_line_series_->removePoints(0, 1);
    } else {
      break;
    }
  }

  if (!line_series->count()) {
    return false;
  }

  this->removeSeries(line_series);
  this->addSeries(line_series);

  this->removeSeries(threshold_line_series_);
  this->addSeries(threshold_line_series_);

  auto axis_y = this->axisY();
  axis_y->setRange(low_, high_);
  this->setAxisY(axis_y, line_series);
  this->setAxisY(axis_y, threshold_line_series_);
  axis_y->setLabelsFont(label_font_);
  return true;
}

void ThresholdChart::SetLow(const float &low) {
  low_ = low;
}

void ThresholdChart::SetHigh(const float &high) {
  high_ = high;
}

void ThresholdChart::SetThreshold(const float &threshold) {
  threshold_ = threshold;
}

void ThresholdChart::Clear() {
  DistanceOverTimeChart::Clear();
  if (!threshold_line_series_) {
    return;
  }
  threshold_line_series_->clear();
}
