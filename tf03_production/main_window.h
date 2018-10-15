#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#ifndef SUPPORT_DEVEL_MODE_PROTOCOL_
#error "MACRO 'SUPPORT_DEVEL_MODE_PROTOCOL_' not found."
#endif

#ifndef USE_APD_EXPERIMENT_PAGE
#error "MACRO 'USE_APD_EXPERIMENT_PAGE' not found."
#endif

#include <QMainWindow>
#include <memory>
#include <QElapsedTimer>
#include <tf03_common/lingual.h>
#include <unordered_map>
#include <tf03_common/config.h>

class CommandEchoHandler;
class Driver;
class CommandEchoWidgetsManager;
class APDPageWrapper;
class MeasureManifest;
class ConnectionPage;
class PageBase;

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

  std::shared_ptr<APDPageWrapper> apd_page_;
  std::shared_ptr<ConnectionPage> connection_page_;

  std::vector<std::shared_ptr<PageBase>> pages_;
};

#endif // MAIN_WINDOW_H
