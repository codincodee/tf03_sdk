#ifndef CUSTOM_WIDGET_H
#define CUSTOM_WIDGET_H

#include <QWidget>
#include "driver_base.h"
#include "export.h"

class API CustomWidget : public QWidget
{
  Q_OBJECT
public:
  explicit CustomWidget(QWidget *parent = nullptr);
  virtual ~CustomWidget();
  virtual void SetDriver(std::shared_ptr<DriverBase> driver);
protected:
  void timerEvent(QTimerEvent *event);
  virtual void CommonTimerCallback();
  std::shared_ptr<DriverBase> driver;
signals:

public slots:
private:
  int timer_id_;
};

#endif // CUSTOM_WIDGET_H
