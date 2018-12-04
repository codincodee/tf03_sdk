#include "custom_object.h"
#include <QTimerEvent>
#include <QDebug>

CustomObject::CustomObject()
{
  timer_id_ = startTimer(DefaultTimerInterval());
}

CustomObject::~CustomObject() {}

int CustomObject::DefaultTimerInterval() {
  return 100;
}

void CustomObject::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  CommonTimerCallback();
}

void CustomObject::CommonTimerCallback() {

}
