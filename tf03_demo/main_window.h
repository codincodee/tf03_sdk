#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <QElapsedTimer>
#include <tf03_common/lingual.h>
#include <unordered_map>
#include <tf03_common/config.h>

class CommandEchoHandler;
class Driver;

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

private:
  Ui::MainWindow *ui;
  std::shared_ptr<Driver> driver_;
  int timer_id_;
  const QString kVersion = tf03_sdk_VERSION_STRING;

  std::shared_ptr<CommandEchoHandler> command_echo_handler_;
};

#endif // MAIN_WINDOW_H
