#ifndef CART_TEST_WIDGET_H
#define CART_TEST_WIDGET_H

#include <QWidget>
#include "export.h"
#include <memory>
#include <QTextStream>

class CartTestSheet;
class DriverBase;
class CartDriver;
struct CartStep;

namespace Ui {
  class CartTestWidget;
}

class API CartTestWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CartTestWidget(QWidget *parent = 0);
  bool Initialize();
  void SetSensor(std::shared_ptr<DriverBase> driver);
  virtual ~CartTestWidget();

protected:
  void timerEvent(QTimerEvent *event);
  virtual bool Archive(const std::list<CartStep>& full);
  virtual bool Archive(const std::list<CartStep>& full, QTextStream& stream);
  QString ArchiveFolder();
  QString ArchiveFilePath();

private slots:
  void on_StartPushButton_clicked();
  void on_OpenFolderPushButton_clicked();
  void on_ConnectPushButton_clicked();

private:
  void UpdateSerialPortCombo();

  Ui::CartTestWidget *ui;
  CartTestSheet* sheet_ = nullptr;
  std::shared_ptr<DriverBase> sensor_;
  std::shared_ptr<CartDriver> cart_;
  int timer_id_;
  QStringList port_list_;
  const QString kConnectButtonConnect = "Connect";
  const QString kConnectButtonDisconnect = "Disconnect";
  const QString kStartButtonStart = "Start";
  const QString kStartButtonFinish = "Finish";
  QString archive_folder_ = "./cart_logs";
  QString kArchiveFileNameDefault = "log";
};

#endif // CART_TEST_WIDGET_H
