#include "distance_over_time_chart.h"
#include <QValueAxis>
#include <QDebug>
#include <QLineSeries>

DistanceOverTimeChart::~DistanceOverTimeChart() {
//  qDebug() << __LINE__ << __FILE__;
//  if (line_series_) {
//    qDebug() << __LINE__ << __FILE__;
//    delete line_series_;
//    qDebug() << __LINE__ << __FILE__;
//    line_series_ = nullptr;
//    qDebug() << __LINE__ << __FILE__;
//  }
//  qDebug() << __LINE__ << __FILE__;
}

bool DistanceOverTimeChart::AddPoint(
    const float &meter, const int &millisecond, QtCharts::QLineSeries* line_series) {
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

  // To make the plot look nicer, we currently set MIN to 0.0f;
  min = 0.0f;

  this->removeSeries(line_series);
  this->addSeries(line_series);

  if (line_series_ == line_series) {
    auto axis_y = this->axisY();
    axis_y->setRange(min, max);
    min_ = min;
    max_ = max;
    this->setAxisY(axis_y, line_series);
    axis_y->setLabelsFont(label_font_);
  }
  return true;
}

void DistanceOverTimeChart::Clear() {
  if (!line_series_) {
    return;
  }
  line_series_->clear();
}

void DistanceOverTimeChart::SetLabelFont(const QFont &font) {
  label_font_ = font;
}

bool DistanceOverTimeChart::AddPoint(
    const float &meter, const int &millisecond) {
  return AddPoint(meter, millisecond, line_series_);
}

DistanceOverTimeChart::DistanceOverTimeChart()
    : ceiling_(20.0f), floor_(5.0f), time_span_(4000) {
  line_series_ = new QtCharts::QLineSeries;
  this->addSeries(line_series_);
  this->setTitle("Distance (m) - Time Elapse (ms)");
  this->legend()->hide();
  auto axis_y = new QtCharts::QValueAxis;
  axis_y->setGridLineVisible(true);
  axis_y->setTickCount(20);
  this->addAxis(axis_y, Qt::AlignLeft);
  label_font_ = axis_y->labelsFont();
}

float DistanceOverTimeChart::GetMax() const {
  return max_;
}

float DistanceOverTimeChart::GetMin() const {
  return min_;
}

void DistanceOverTimeChart::SetCeiling(const float &ceiling) {
  ceiling_ = ceiling;
}

void DistanceOverTimeChart::SetFloor(const float &floor) {
  floor_ = floor;
}

void DistanceOverTimeChart::SetTimeSpan(const int &span) {
  time_span_ = span;
}

QtCharts::QLineSeries* DistanceOverTimeChart::Series() {
  return line_series_;
}

void DistanceOverTimeChart::CurrentAverageAndStandardDeviation(
    float &average, float &stddev) {
  average = 0.0f;
  stddev = 0.0f;
  if (!line_series_) {
    return;
  }
  if (!line_series_->count()) {
    return;
  }
  std::vector<float> v;
  v.reserve(line_series_->count());
  for (int i = 0; i < line_series_->count(); ++i) {
    v.push_back(line_series_->at(i).y());
  }
  auto sum = std::accumulate(std::begin(v), std::end(v), 0.0);
  auto m = sum / v.size();
  average = m;

  float accum = 0.0f;
  std::for_each (std::begin(v), std::end(v), [&](const float d) {
      accum += (d - m) * (d - m);
  });

  stddev = std::sqrt(accum / (v.size()-1));
}

float DistanceOverTimeChart::CurrentStandardDeviation() {
  float avg, stddev;
  CurrentAverageAndStandardDeviation(avg, stddev);
  return stddev;
}
