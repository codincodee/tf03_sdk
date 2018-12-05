#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>

class MiniRTECart;
class DriverServer;
class TFMiniDriver;
class CartDriver;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void timerEvent(QTimerEvent *event);

private slots:
  void on_DebugPushButton_clicked();

  void on_DebugAlphaPushButton_clicked();

private:
  Ui::MainWindow *ui;

  std::shared_ptr<TFMiniDriver> driver_;
  std::shared_ptr<DriverServer> driver_server_;
//  std::shared_ptr<CartDriver> cart_;
  std::shared_ptr<MiniRTECart> cart_;
  int timer_id_;
};

#endif // MAIN_WINDOW_H
