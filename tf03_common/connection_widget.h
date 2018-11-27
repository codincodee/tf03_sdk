#ifndef CONNECTION_WIDGET_H
#define CONNECTION_WIDGET_H

#include "custom_widget.h"
#include <memory>
#include "export.h"

namespace Ui {
  class ConnectionWidget;
}

class API ConnectionWidget : public CustomWidget
{
  Q_OBJECT

public:
  explicit ConnectionWidget(QWidget *parent = 0);
  void SetDriver(std::shared_ptr<DriverBase> driver) override;
  ~ConnectionWidget();

protected:
  void CommonTimerCallback();

private slots:
  void on_ConnectPushButton_clicked();

private:
  Ui::ConnectionWidget *ui;
  QStringList port_list_;
  const QString kConnectButtonConnect = "Connect";
  const QString kConnectButtonDisconnect = "Disconnect";
};

#endif // CONNECTION_WIDGET_H
