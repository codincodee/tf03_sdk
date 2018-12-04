#ifndef CUSTOM_OBJECT_H
#define CUSTOM_OBJECT_H

#include <QObject>
#include "export.h"

class API CustomObject : public QObject
{
public:
  CustomObject();
  virtual ~CustomObject();
  static int DefaultTimerInterval();
protected:
  void timerEvent(QTimerEvent* event);
  virtual void CommonTimerCallback();
  int timer_id_;
};

#endif // CUSTOM_OBJECT_H
