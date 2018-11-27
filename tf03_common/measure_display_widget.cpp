#include "measure_display_widget.h"
#include "ui_measure_display_widget.h"
#include <QDebug>

MeasureDisplayWidget::MeasureDisplayWidget(QWidget *parent) :
  CustomWidget(parent),
  ui(new Ui::MeasureDisplayWidget)
{
  ui->setupUi(this);
}

MeasureDisplayWidget::~MeasureDisplayWidget()
{
  delete ui;
}

void MeasureDisplayWidget::CommonTimerCallback() {
  if (!driver) {
    return;
  }

  auto dist_label = ui->DistanceLabel;
  auto freq_label = ui->FrequencyLabel;

  auto measure = driver->Measure();
  bool new_measure = false;
  if (measure) {
    dist_label->setText(QString::number(measure->dist));
    new_measure = true;
  }
  if (new_measure) {
    if (measure->id == last_freq_measure_id_) {
      new_measure = false;
    }
  }

  auto elapse = frequency_timer_.elapsed();
  if (new_measure) {
    dist_label->setText(QString::number(measure->dist));
    if (elapse > 1000) {
      auto id_diff = measure->id - last_freq_measure_id_;
      float frequency = id_diff / (elapse / 1000.0f);
      freq_label->setText(QString::number(std::ceil(frequency)));
      last_freq_measure_id_ = measure->id;
      frequency_timer_.restart();
    }
  }

  elapse = frequency_timer_.elapsed();
  if (elapse > 2000) {
    dist_label->clear();
    freq_label->clear();
  }
}
