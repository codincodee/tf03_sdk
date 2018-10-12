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
class CommandEchoWidgetsManager;
class APDPage;
class MeasureManifest;

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
  void SetupUIText();

  void on_ChinesePushButton_clicked();

  void on_EnglishPushButton_clicked();

  void on_ConnectPushButton_clicked();

private:
  void UpdatePortNameComboBox();

  Ui::MainWindow *ui;
  std::shared_ptr<Driver> driver_;
  int timer_id_;
  QElapsedTimer frequency_timer_;
  unsigned long last_freq_measure_id_ = 0;
  const Lingual kLanguageLabelText = {"Language", "语言"};
  const Lingual kDistanceLabelText = {"Distance (cm)", "距离 （cm）"};
  const Lingual kFrequencyLabelText = {"Frequency (Hz)", "频率 （Hz）"};
  const Lingual kPortNameLabelText = {"Port", "端口"};
  const Lingual kBaudRateLabelText = {"Baud Rate", "波特率"};
  const Lingual kConnectPushButtonText = {"Connect", "连接"};
  const Lingual kDisconnectPushButtonText = {"Disconnect", "断开"};
  const Lingual kWindowTitle =
#ifdef SWITCH_ON_ALL_FEATURES
  {"TF03 All-in-one", "TF03全功能上位机"};
#else
  {"TF03 Setup", "TF03 配置客户端"};
#endif
  const QString kVersion = tf03_sdk_VERSION_STRING;

  Lingual connect_button_current_lingual_;

  std::shared_ptr<CommandEchoHandler> command_echo_handler_;

  QStringList port_combo_;

  std::shared_ptr<APDPage> apd_page_;
};

#endif // MAIN_WINDOW_H
