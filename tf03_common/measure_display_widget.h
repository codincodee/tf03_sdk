#ifndef MEASURE_DISPLAY_WIDGET_H
#define MEASURE_DISPLAY_WIDGET_H

#include "custom_widget.h"

namespace Ui {
  class MeasureDisplayWidget;
}

class API MeasureDisplayWidget : public CustomWidget
{
  Q_OBJECT

public:
  explicit MeasureDisplayWidget(QWidget *parent = 0);
  ~MeasureDisplayWidget();

protected:
  void CommonTimerCallback() override;
  void OnMeasureCalled(std::unique_ptr<MeasureBasic> measure) override;
private:
  Ui::MeasureDisplayWidget *ui;
  QElapsedTimer frequency_timer_;
  unsigned long last_freq_measure_id_ = 0;
  std::unique_ptr<MeasureBasic> measure_;
};

#endif // MEASURE_DISPLAY_WIDGET_H
