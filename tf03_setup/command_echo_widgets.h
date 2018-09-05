#ifndef COMMAND_ECHO_WIDGETS_H
#define COMMAND_ECHO_WIDGETS_H

#include <QObject>
#include "lingual.h"
#include <QElapsedTimer>

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

  QElapsedTimer timer;

  std::shared_ptr<Driver> driver;
  std::shared_ptr<CommandEchoHandler> echo_handler;

  int timeout;

  bool respond_button_click = true;

  Q_OBJECT

 public slots:
  void OnButtonClicked();

 protected:
  virtual void ButtonClicked();
  static QLabel* UINullLabel();
  static void SetWidgetUINullLabel(QWidget*& widget);
  void SetOptionWidgetUINull();
  void SetStatusLabelUINull();
  void SetLineEditIntValidity(
      QLineEdit *edit, const int& min, const int& max);
  const static QString kUINullString;
};

////////////////////// SetProtocolWidgets /////////////////////////////

struct SetProtocolWidgets : public CommandEchoWidgets {
  SetProtocolWidgets();
  void SetOptionLingual() override;
  void ButtonClicked() override;
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
  const Lingual kSwitchOnLingual = {"Automated", "连续输出"};
  const Lingual kSwitchOffLingual = {"Manual", "指令触发"};
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
  void ButtonClicked() override;
  void SetOptionLingual() override;
  QLineEdit* edit;
  const int kSNLength = 14;
};

#endif // COMMAND_ECHO_WIDGETS_H
