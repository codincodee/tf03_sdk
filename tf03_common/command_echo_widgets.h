#ifndef COMMAND_ECHO_WIDGETS_H
#define COMMAND_ECHO_WIDGETS_H

#include <QObject>
#include <tf03_common/lingual.h>
#include <QElapsedTimer>
#include <set>
#include <queue>

class QLabel;
class QPushButton;
class Driver;
class QComboBox;
class CommandEchoHandler;
class QLineEdit;

////////////////////// CommandEchoWidgets /////////////////////////////

struct CommandEchoWidgets : public QObject
{
  CommandEchoWidgets();
  virtual ~CommandEchoWidgets();

  char id;
  QLabel* item;
  QWidget* option;
  QPushButton* button;
  QLabel* status;

  Lingual item_lingual;
  Lingual button_lingual;
  virtual void SetOptionLingual();
  Lingual status_lingual;

  virtual void Update();

  void SetupUI();

  const Lingual kButtonSetLingual = {"Set", "设置"};
  const Lingual kButtonRequestLingual = {"Request", "请求"};
  const Lingual kNoResponseLingual = {"No Response", "未响应"};
  const Lingual kSuccessLingual = {"Successful", "成功"};
  const Lingual kFailLingual = {"Failed", "失败"};
  const Lingual kUnknownLingual = {"Unknown", "未知错误"};
  const Lingual kMsgBoxErrorTitle = {"Error", "错误"};
  const Lingual kMsgBoxInfoTitle = {"Info", "提示"};
  const Lingual kMsgBoxAbort = {"Abort", "放弃"};
  const Lingual kMsgBoxOk = {"Ok", "确定"};
  const Lingual kMsgBoxCancel = {"Cancel", "取消"};
  const Lingual kOnLingual = {"ON", "开"};
  const Lingual kOffLingual = {"OFF", "关"};

  QElapsedTimer timer;

  std::shared_ptr<Driver> driver;
  std::shared_ptr<CommandEchoHandler> echo_handler;

  int timeout;

  bool respond_button_click = true;

  Q_OBJECT

 public slots:
  void OnButtonClicked();

 protected:
  enum class CheckStatus {succeeded, failed, no_response};
  virtual void ButtonClicked();
  static QLabel* UINullLabel();
  static void SetWidgetUINullLabel(QWidget*& widget);
  void SetOptionWidgetUINull();
  void SetStatusLabelUINull();
  bool ProceedUpdate();
  virtual CheckStatus CheckCommandEcho();
  virtual void OnRetry();
  const static QString kUINullString;
  int retry = 2;
  int retry_cnt = 0;
};

////////////////////// EchoWidgetsBase /////////////////////////////

struct EchoWidgetsBase : public CommandEchoWidgets {
  void ButtonClicked() override;
  bool stream_off_tip = false;
  bool proceed_button = true;
private:
  bool stream_off_tip_showed = false;
};

////////////////////// SequentialCommandsWidgets /////////////////////////////

struct SequentialCommandsWidgets : public CommandEchoWidgets {
  SequentialCommandsWidgets();
  virtual ~SequentialCommandsWidgets();
  virtual void Update() override;
  virtual void ButtonClicked() override;
  void Start();
  virtual void LoadCommands();
  void LoadCommand(std::function<void()> cmd, const int& id);
  void LoadCommand(
      std::function<void ()> cmd,
      std::function<CheckStatus()> check);
  void LoadCommand(std::function<void()> cmd);
  std::queue<
      std::pair<
          std::function<void()>,
          std::function<CheckStatus()>>> task_queue;
};

////////////////////// SetProtocolWidgets /////////////////////////////

struct SetProtocolWidgets : public CommandEchoWidgets {
  SetProtocolWidgets();
  void SetOptionLingual() override;
  void ButtonClicked() override;
  static int ID();
  const Lingual kDevelLingual = {"Devel", "开发"};
  const Lingual kReleaseLingual = {"Release", "发布"};
  QComboBox* combo;
};

////////////////////// SetFrequencyWidgets /////////////////////////////

struct SetFrequencyWidgets : public CommandEchoWidgets {
  SetFrequencyWidgets();
  void ButtonClicked() override;
  void Update() override;
  QComboBox* combo;
};

////////////////////// SerialNumberWidgets /////////////////////////////

struct SerialNumberWidgets : public CommandEchoWidgets {
  SerialNumberWidgets();
  void ButtonClicked() override;
  void Update() override;
  QLabel* label;
};

////////////////////// OutputSwitchWidgets /////////////////////////////

struct OutputSwitchWidgets : public CommandEchoWidgets {
  OutputSwitchWidgets();
  void ButtonClicked() override;
  void Update() override;
  void SetOptionLingual() override;
  QComboBox* combo;
  static const Lingual kSwitchOnLingual;
  static const Lingual kSwitchOffLingual;
  static const Lingual kItemLingual;
  const Lingual kOutputOnLingual = {"Automated", "连续输出"};
  const Lingual kOutputOffLingual = {"Manual", "指令触发"};
};

////////////////////// MeasureTriggerWidgets /////////////////////////////

struct MeasureTriggerWidgets : public CommandEchoWidgets {
  MeasureTriggerWidgets();
  void ButtonClicked() override;
  void Update() override;
};

////////////////////// FlashSettingsWidgets /////////////////////////////

struct FlashSettingsWidgets : public CommandEchoWidgets {
  FlashSettingsWidgets();
  void ButtonClicked() override;
  static int ID();
};

////////////////////// RestoreFactoryWidgets /////////////////////////////

struct RestoreFactoryWidgets : public CommandEchoWidgets {
  RestoreFactoryWidgets();
  void ButtonClicked() override;
};

////////////////////// SetSerialBaudRateWidgets /////////////////////////////

struct SetSerialBaudRateWidgets : public CommandEchoWidgets {
  SetSerialBaudRateWidgets();
  void ButtonClicked() override;
  void Update() override;
  QComboBox* combo;
};

////////////////////// SetPortTypeWidgets /////////////////////////////

struct SetPortTypeWidgets : public CommandEchoWidgets {
  SetPortTypeWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  static int ID();
  QComboBox* combo;
  const Lingual kSerial = {"Serial Port", "串口"};
  const Lingual kCAN = {"CAN", "CAN"};
};

////////////////////// SetOutputFormatWidgets /////////////////////////////

struct SetOutputFormatWidgets : public CommandEchoWidgets {
  SetOutputFormatWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  void Update() override;
  QComboBox* combo;
  const Lingual kNineBytes = {"Standard", "标准串口"};
  const Lingual kPixhawk = {"Pixhawk", "Pixhawk"};
  const Lingual kIO = {"IO", "高低电平"};
  const Lingual k485 = {"485", "485"};
  const Lingual kMsg = {"Msg", "Msg"};
};

////////////////////// SetCANIDWidgetsBase /////////////////////////////

struct SetCANIDWidgetsBase : public CommandEchoWidgets {
  SetCANIDWidgetsBase();
  QLineEdit* edit;
};

////////////////////// SetCANSendIDWidgets /////////////////////////////

struct SetCANSendIDWidgets : public SetCANIDWidgetsBase {
  SetCANSendIDWidgets();
  void ButtonClicked() override;
};

////////////////////// SetCANReceiveIDWidgets /////////////////////////////

struct SetCANReceiveIDWidgets : public SetCANIDWidgetsBase {
  SetCANReceiveIDWidgets();
  void ButtonClicked() override;
};

////////////////////// SetCANBaudRateWidgets /////////////////////////////

struct SetCANBaudRateWidgets : public CommandEchoWidgets {
  SetCANBaudRateWidgets();
  void ButtonClicked() override;
  QComboBox* combo;
};

////////////////////// SetOutRangeValueWidgets /////////////////////////////

struct SetOutRangeValueWidgets : public CommandEchoWidgets {
  SetOutRangeValueWidgets();
  void ButtonClicked() override;
  QLineEdit* edit;
};

////////////////////// RequestVersionWidgets /////////////////////////////

struct RequestVersionWidgets : public CommandEchoWidgets {
  RequestVersionWidgets();
  void ButtonClicked() override;
  void Update() override;
  QLabel* label;
};

////////////////////// WriteSerialNumberWidgets /////////////////////////////

struct WriteSerialNumberWidgets : public CommandEchoWidgets {
  WriteSerialNumberWidgets();
  virtual ~WriteSerialNumberWidgets();
  virtual void ButtonClicked() override;
  bool ButtonClickedImpl();
  void SetOptionLingual() override;
  void DisableInputMask();
  QString default_input_mask;
  QLineEdit* edit;
  const int kSNLength = 14;
};

//////////////////// WriteSerialNumberPlusWidgets ///////////////////////////

struct WriteSerialNumberPlusWidgets : public WriteSerialNumberWidgets {
  WriteSerialNumberPlusWidgets();
  void Update() override;
};

////////////////////// SetAPDWidgets /////////////////////////////

struct SetAPDWidgets : public CommandEchoWidgets {
  SetAPDWidgets();
  void ButtonClicked() override;
  QLineEdit* edit;
};

////////////////////// AutoGainAdjustWidgets /////////////////////////////

struct AutoGainAdjustWidgets : public CommandEchoWidgets {
  AutoGainAdjustWidgets();
  static int ID();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QComboBox* combo;
};

////////////////////// AdaptiveAPDWidgets /////////////////////////////

struct AdaptiveAPDWidgets : public CommandEchoWidgets {
  AdaptiveAPDWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QComboBox* combo;
};

////////////////////// APDClosedLoopWidgets /////////////////////////////

struct APDClosedLoopWidgets : public CommandEchoWidgets {
  APDClosedLoopWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QComboBox* combo;
};

////////////////////// DistanceL1WriteWidgets /////////////////////////////

struct DistanceL1WriteWidgets: public EchoWidgetsBase {
  DistanceL1WriteWidgets();
  void ButtonClicked() override;
  QLineEdit* edit;
};

////////////////////// DistanceL1ReadWidgets /////////////////////////////

struct DistanceL1ReadWidgets : public EchoWidgetsBase {
  DistanceL1ReadWidgets();
  void ButtonClicked() override;
  void Update() override;
};

////////////////////// DistanceLWriteWidgets /////////////////////////////

struct DistanceLWriteWidgets : public EchoWidgetsBase {
  DistanceLWriteWidgets();
  void ButtonClicked() override;
  QLineEdit* edit;
};

////////////////////// DistanceLReadWidgets /////////////////////////////

struct DistanceLReadWidgets : public EchoWidgetsBase {
  DistanceLReadWidgets();
  void ButtonClicked() override;
  void Update() override;
};

////////////////////// CustomizationWidgets /////////////////////////////

struct CustomizationWidgets : public EchoWidgetsBase {
  CustomizationWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  static int ID();
  QComboBox* combo;
  const Lingual kCommon = {"Common", "通用版本"};
  const Lingual kBL = {"BL", "BL定制版"};
  const Lingual kI13 = {"I13", "I13定制版"};
  const Lingual kQL = {"QL", "QL定制版"};
  const Lingual kZC = {"ZC", "ZC定制版"};
};

////////////////////// RangeDetectWidgets /////////////////////////////

struct RangeDetectWidgets : public CommandEchoWidgets {
  RangeDetectWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QComboBox* combo;
};

////////////////////// HorizontalAngleWidgets /////////////////////////////

struct HorizontalAngleWidgets : public CommandEchoWidgets {
  HorizontalAngleWidgets();
  void ButtonClicked() override;
  QLineEdit* edit;
};

////////////////////// RangeValidityWidgets /////////////////////////////

struct RangeValidityWidgets : public CommandEchoWidgets {
  RangeValidityWidgets();
  void ButtonClicked() override;
  void Update() override;
  void SetOptionLingual() override;
  const Lingual kValid = {"Valid", "合格"};
  const Lingual kInvalid = {"Invalid", "不合格"};
  Lingual option_lingual;
  QLabel* label;
  QLabel* indicator;
  int echo_cnt = 0;
};

////////////////////// SetReleaseModeWidgets /////////////////////////////

struct SetReleaseModeWidgets : public SequentialCommandsWidgets {
  SetReleaseModeWidgets();
  void LoadCommands() override;
  void SetOptionLingual() override;
  const Lingual kDevelTest = {"Devel Test", "内部测试"};
  const Lingual kUARTStandard = {"UART Standard", "串口标品"};
  const Lingual kCANStandard = {"CAN Standard", "CAN标品"};
  const Lingual kClientBL = {"Client BL", "BL客户"};
  const Lingual kClientI13 = {"Client I13", "I13客户"};
  const Lingual kClientQL = {"Client QL", "QL客户"};
  std::function<CheckStatus()> output_format_check;
  QComboBox* combo;
};

////////////////////// VdbsAutoAdjustWidgets /////////////////////////////

struct VdbsAutoAdjustWidgets : public CommandEchoWidgets {
  VdbsAutoAdjustWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QComboBox* combo;
};

////////////////////// CANFrameTypeWidgets /////////////////////////////

struct CANFrameTypeWidgets : public CommandEchoWidgets {
  CANFrameTypeWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QComboBox* combo;
  const Lingual kStandard = {"Standard", "标准帧"};
  const Lingual kExtended = {"Extended", "扩展帧"};
};

////////////////////// MeasureLoggingWidgets /////////////////////////////

struct MeasureLoggingWidgets : public CommandEchoWidgets {
  MeasureLoggingWidgets();
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QPushButton* browse;
  const Lingual kBrowseTip = {"Browse Folder", "单击选择文件夹"};
  const Lingual kBrowseReady = {"Folder Selected", "文件夹已选择"};
  const Lingual kRecordLingual = {"Record", "录制"};
  const Lingual kStopLingual = {"Stop", "停止"};
  const Lingual kRecordingLingual = {"Recording", "录制中"};
  Lingual browse_lingual;
  QString save_path_;

  Q_OBJECT
public slots:
  void HandleStart();
  void HandleStop();
  void OnBrowseButtonClicked();
};
#endif // COMMAND_ECHO_WIDGETS_H
