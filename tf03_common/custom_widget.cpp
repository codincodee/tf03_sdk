#include "custom_widget.h"
#include <QTimerEvent>

CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
  timer_id_ = startTimer(DefaultTimerInterval());
}

CustomWidget::~CustomWidget() {}

void CustomWidget::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  CommonTimerCallback();
}

void CustomWidget::CommonTimerCallback() {}

void CustomWidget::SetDriver(std::shared_ptr<DriverBase> driver) {
  this->driver = driver;
}

void CustomWidget::SetReceiveMeasureEveryUpdate(const bool &receive) {
  receive_measure_every_update_ = receive;
}

void CustomWidget::MeasureCallback(std::unique_ptr<MeasureBasic> measure) {
  OnMeasureCalled(std::move(measure));
}

void CustomWidget::OnMeasureCalled(std::unique_ptr<MeasureBasic> measure) {

}

int CustomWidget::DefaultTimerInterval() {
  return 100;
}

