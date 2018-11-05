#ifndef APD_PAGE_H
#define APD_PAGE_H

#include <QObject>
#include <QElapsedTimer>
#include <memory>
#include "plot_base.h"
#include "export.h"
#include "parsed.h"

struct MeasureDevel;
struct MeasureDevelStream;
// #include <QLayout>
class QLayout;
class DistanceOverTimeChart;
namespace QtCharts {
  class QChartView;
}
class QLabel;
class QPushButton;
class QProgressBar;
class QLineEdit;
class QElapsedTimer;
class Driver;
class CommandEchoHandler;

class API APDPage : public PlotBase
{
  Q_OBJECT
public:
  APDPage();
  ~APDPage();
  void SetAPDDisplayLabel(QLabel* label);
  void SetTemperatureDisplayLabel(QLabel* label);
  void SetStartPushButton(QPushButton* button);
  void SetProgressBar(QProgressBar* progress);
  void SetAPDFromLineEdit(QLineEdit* edit);
  void SetAPDToLineEdit(QLineEdit* edit);
  void SetThresholdLineEdit(QLineEdit* edit);
  void SetDriver(std::shared_ptr<Driver> driver);
  void SetCmdEchoHandler(std::shared_ptr<CommandEchoHandler> echoes);
  void SetStatusLabel(QLabel* label);
  bool Initialize();
  void IncomingMeasure(const MeasureDevel& measure);
  void Update();
  void UsePageBaseSpecs(const bool& use = true);
  void SetLeastStartTemperature(const int& temp);
  void SetSaveSettingsWhenWriteResult(const bool& save);
  void SetLogPath(const QString& path);
  void EchoWriteAPDResult();
public slots:
  void OnStartButtonClicked();
private:
  bool OnStart();
  void OnStop();
  bool IsSampleStable(MeasureDevelStream stream);
  int CalculateStandardDistance(MeasureDevelStream stream);
  bool IsCrashed(
      const MeasureDevelStream& stream,
      const int& std_dist, const int& threshold);
  void HandleCrashed(const MeasureDevel& measure, const int& apd_crash_);
  void ProceedExperiment();
  int CalculateResultAPD(const int& apd_crash, const float& temp);
  void HandleLogging(
      const int& apd_crash, const int& apd_result, const float& temp);
  void DetectWriteAPDResultEcho();

  QLabel* apd_label_ = nullptr;
  QLabel* temp_label_ = nullptr;
  QPushButton* start_button_ = nullptr;
  QProgressBar* progress_bar_ = nullptr;
  QLineEdit* apd_from_edit_ = nullptr;
  QLineEdit* apd_to_edit_ = nullptr;
  QLineEdit* threshold_edit_ = nullptr;
  QLabel* status_label_ = nullptr;
  const QString kStartButtonStart = "开始";
  const QString kStartButtonStop = "停止";
  int apd_from_;
  int apd_to_;
  int threshold_;
  int apd_cmd_;
  int apd_step_ = 1;
  bool ongoing_ = false;
  std::shared_ptr<QElapsedTimer> timeout_;
  std::shared_ptr<QElapsedTimer> timer_;
  std::shared_ptr<Driver> driver_;
  std::shared_ptr<CommandEchoHandler> echoes_;
  const char kSetAPDCmdID = 0x40;

  enum class Phase {
    wait_for_echo,
    wait_for_stable,
    wait_for_sample
  };
  Phase phase_;

  int std_dist_;

  const int kMaxToIgnore = 2;
  const int kMinToIgnore = 2;

  bool use_page_base_specs_ = false;

  MeasureDevelStamped last_measure_;

  int least_mcu_temperature_ = 0;

  bool save_settings_after_setting_result_apd_ = false;

  QString log_dir_path_;

  std::shared_ptr<QElapsedTimer> write_apd_result_timeout_;
  bool echo_write_apd_result_ = false;
};

#endif // APD_PAGE_H
