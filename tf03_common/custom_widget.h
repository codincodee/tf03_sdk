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
  void SetReceiveMeasureEveryUpdate(const bool& receive);
  void MeasureCallback(std::unique_ptr<MeasureBasic> measure);
  static int DefaultTimerInterval();
protected:
  void timerEvent(QTimerEvent *event);
  virtual void CommonTimerCallback();
  virtual void OnMeasureCalled(std::unique_ptr<MeasureBasic> measure);
  std::shared_ptr<DriverBase> driver;
signals:

public slots:
private:
  int timer_id_;
  bool receive_measure_every_update_ = false;
};

#endif // CUSTOM_WIDGET_H
