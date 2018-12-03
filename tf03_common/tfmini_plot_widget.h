#ifndef TFMINI_PLOT_WIDGET_H
#define TFMINI_PLOT_WIDGET_H

#include "custom_widget.h"
#include "export.h"

class DistanceOverTimeChart;
class TFMiniDriver;

namespace Ui {
  class TFMiniPlotWidget;
}

class API TFMiniPlotWidget : public CustomWidget
{
  Q_OBJECT

public:
  explicit TFMiniPlotWidget(QWidget *parent = 0);
  ~TFMiniPlotWidget();
  void SetDriver(std::shared_ptr<TFMiniDriver> driver);
protected:
  void CommonTimerCallback();
  void OnMeasureCalled(std::unique_ptr<MeasureBasic> measure) override;
private:
  Ui::TFMiniPlotWidget *ui;
  DistanceOverTimeChart* chart_;
  std::shared_ptr<TFMiniDriver> driver_;
};

#endif // TFMINI_PLOT_WIDGET_H
