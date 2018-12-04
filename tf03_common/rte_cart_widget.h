#ifndef RTE_CART_WIDGET_H
#define RTE_CART_WIDGET_H

#include "custom_widget.h"

class RTECartServer;
class MiniRTECart;

namespace Ui {
  class RTECartWidget;
}

class API RTECartWidget : public CustomWidget
{
  Q_OBJECT

public:
  explicit RTECartWidget(QWidget *parent = 0);
  void SetDriverServer(std::shared_ptr<RTECartServer> server);
  ~RTECartWidget();
private slots:
  void OnI037Burn();
  void OnI037TempBurn();
  void OnHeating();
  void OnFinished();
  void OnAutoI();
  void OnI037Back();
  void OnI037Temp();

  void on_ContinuePushButton_clicked();

  void on_StartPushButton_clicked();

private:
  Ui::RTECartWidget *ui;
  std::shared_ptr<RTECartServer> server_;
  std::shared_ptr<MiniRTECart> driver_;
  QString kStartButtonStart = "start";
  QString kStartButtonStop = "stop";
};

#endif // RTE_CART_WIDGET_H
