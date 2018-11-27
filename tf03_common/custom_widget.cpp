#include "custom_widget.h"
#include <QTimerEvent>

CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
  timer_id_ = startTimer(100);
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
