#ifndef FIRMWARE_MODULE_H
#define FIRMWARE_MODULE_H

#include <QString>
#include <queue>
#include <memory>
#include "export.h"

enum class API FirmwareUpdateStatus;
class Driver;

class API FirmwareModule
{
 public:
  FirmwareModule();
  virtual ~FirmwareModule();
  void SetDriver(std::shared_ptr<Driver> driver);
  void SetModeSerial();
  void SetModeCAN();
  bool LoadBin(const QString& path);
  int SegmentNum();
  bool Step();
  bool Step(const FirmwareUpdateStatus &status);
  float GetProgress();
 private:
  // int segment_length_ = 249;
  int segment_length_ = 2;
  int segments_per_send_ = 10;
  std::queue<QByteArray> queue_;
  std::shared_ptr<Driver> driver_;
  int segment_total_;
  int bin_bytes_;
  // QByteArray last_segment_;
};

///////////////////////////////////////////////////////////////////

#include "command_echo_widgets.h"

class QProgressBar;

struct API UpgradeFirmwareWidgets : public CommandEchoWidgets {
  UpgradeFirmwareWidgets();
  void ButtonClicked() override;
  void Update() override;
  void SetOptionLingual() override;
  QComboBox* combo;
  QPushButton* browse;
  QProgressBar* progress;
  const Lingual kUpgradeLingual = {"Upgrade", "升级"};
  const Lingual kStopLingual = {"Stop", "停止"};
  const Lingual kSelectBinaryLingual =
      {"Select Firmware File", "选择固件文件"};
  const Lingual kBinaryReady = {"Firmware File Ready", "固件文件就绪"};
  const Lingual kSerial = {"Serial Port", "串口"};
  const Lingual kCAN = {"CAN", "CAN"};

  QString bin_file;
  Lingual browse_lingual;
  std::shared_ptr<FirmwareModule> module;

  void HandleFailure();
  void HandleSuccess();
  void HandleStop();

  std::function<void(bool)> set_respond_all_button;

  bool before_upgrade_instructions_shown = false;

  Q_OBJECT
 public slots:
  void OnBrowsing();
};

#endif // FIRMWARE_MODULE_H
