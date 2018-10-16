#include "command_echo_widgets.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <tf03_common/driver.h>
#include <QDebug>
#include <tf03_common/command_echo_handler.h>
#include <QLineEdit>
#include <QSizePolicy>
#include <QMessageBox>
#include <tf03_common/utils.h>

////////////////////// CommandEchoWidgets /////////////////////////////

CommandEchoWidgets::CommandEchoWidgets() : timeout(1000) {
  id = 0;
  item = new QLabel;
  option = nullptr;
  button = new QPushButton;
  status = new QLabel;
  button_lingual = kButtonSetLingual;
  button->setText(which_lingual(kButtonSetLingual));
  connect(button, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
}

void CommandEchoWidgets::SetOptionLingual() {

}

void CommandEchoWidgets::Update() {
  if (button->isEnabled()) {
    return;
  }
  if (timer.elapsed() > timeout) {
    button->setDisabled(false);
    status->setText(which_lingual(kNoResponseLingual));
    status_lingual = kNoResponseLingual;
  }
  if (echo_handler->IsCommandEchoed(id)) {
    button->setDisabled(false);
    if (echo_handler->IsCommandSucceeded(id)) {
      status_lingual = kSuccessLingual;
    } else if (echo_handler->IsCommandSucceeded(id)) {
      status_lingual = kFailLingual;
    } else {
      status_lingual = kUnknownLingual;
    }
    status->setText(which_lingual(status_lingual));
  }
}

void CommandEchoWidgets::SetupUI() {
  item->setText(which_lingual(item_lingual));
  button->setText(which_lingual(button_lingual));
  SetOptionLingual();
  status->setText(which_lingual(status_lingual));
}

void CommandEchoWidgets::OnButtonClicked() {
  if (!respond_button_click) {
    return;
  }
  return ButtonClicked();
}
void CommandEchoWidgets::ButtonClicked() {
  button->setDisabled(true);
  status->clear();
  timer.restart();
}

//void CommandEchoWidgets::SetWidgetNotApplicable(QWidget*& widget) {
//  if (widget) {
//    delete widget;
//  }
//  auto label = new QLabel;
//  label->setText("---");
//  label->setAlignment(Qt::AlignHCenter);
//  widget = label;
//}

const QString CommandEchoWidgets::kUINullString(" --- ");

QLabel* CommandEchoWidgets::UINullLabel() {
  auto label = new QLabel;
  label->setText(kUINullString);
  label->setAlignment(Qt::AlignHCenter);
  return label;
}

void CommandEchoWidgets::SetWidgetUINullLabel(QWidget *&widget) {
  if (widget) {
    delete widget;
  }
  widget = UINullLabel();
}

void CommandEchoWidgets::SetStatusLabelUINull() {
  if (status) {
    delete status;
  }
  status = UINullLabel();
  status_lingual = {kUINullString, kUINullString};
}

void CommandEchoWidgets::SetOptionWidgetUINull() {
  SetWidgetUINullLabel(option);
}

////////////////////// SetProtocolWidgets /////////////////////////////

SetProtocolWidgets::SetProtocolWidgets() : CommandEchoWidgets() {
  id = 0x44;
  item_lingual = {"Protocol", "通信协议"};
  combo = new QComboBox;
  option = combo;
}

void SetProtocolWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kDevelLingual));
  combo->addItem(which_lingual(kReleaseLingual));
}

void SetProtocolWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  if (lingual_equal(combo->currentText(), kDevelLingual)) {
    driver->SetDevelMode();
  } else if (lingual_equal(combo->currentText(), kReleaseLingual)) {
    driver->SetReleaseMode();
  } else {
    qDebug() << "Error: " << __FUNCTION__ << __LINE__;
  }
}

////////////////////// SetFrequencyWidgets /////////////////////////////

SetFrequencyWidgets::SetFrequencyWidgets() : CommandEchoWidgets() {
  id = 0x03;
  combo = new QComboBox;
  combo->addItem("1");
  combo->addItem("5");
  combo->addItem("10");
  combo->addItem("20");
  combo->addItem("30");
  combo->addItem("40");
  combo->addItem("50");
  combo->addItem("100");
  combo->addItem("200");
  combo->addItem("300");
  combo->addItem("400");
  combo->addItem("500");
  combo->addItem("600");
  combo->addItem("700");
  combo->addItem("800");
  combo->addItem("900");
  combo->addItem("1000");
  item_lingual = {"Frequency (Hz)", "帧率 (Hz)"};
  option = combo;
}

void SetFrequencyWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto freq = combo->currentText().toInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetFrequency(freq);
}

void SetFrequencyWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsFrequencyEchoed()) {
    auto str = QString::number(echo_handler->Frequency());
    status_lingual = kSuccessLingual;
    status_lingual.eng += ": " + str + "Hz";
    status_lingual.chn += ": " + str + "Hz";
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
}

////////////////////// SerialNumberWidgets /////////////////////////////

SerialNumberWidgets::SerialNumberWidgets() {
  id = 0x56;
  label = new QLabel;
  option = label;
  item_lingual = {"Serial Number", "序列号"};
  button_lingual = kButtonRequestLingual;
  button->setText(which_lingual(kButtonRequestLingual));
}

void SerialNumberWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  label->clear();
  driver->RequestSerialNumber();
}

void SerialNumberWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsSerialNumberEchoed()) {
    auto sn = echo_handler->SerialNumber();
    if (sn.isEmpty()) {
      status_lingual = kFailLingual;
    } else {
      status_lingual = kSuccessLingual;
    }
    status->setText(which_lingual(status_lingual));
    label->setText(sn);
    button->setDisabled(false);
  }
}

////////////////////// OutputSwitchWidgets /////////////////////////////

OutputSwitchWidgets::OutputSwitchWidgets() {
  id = 0x07;
  combo = new QComboBox;
  option = combo;
  item_lingual = {"Output Switch", "输出模式"};
}

void OutputSwitchWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  if (lingual_equal(combo->currentText(), kSwitchOnLingual)) {
    driver->SetOutputSwitchOn();
  } else if (lingual_equal(combo->currentText(), kSwitchOffLingual)) {
    driver->SetOutputSwitchOff();
  }
}

void OutputSwitchWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsOutputSwitchEchoed()) {
    if (echo_handler->IsOutputOn()) {
      status_lingual = kOutputOnLingual;
    } else {
      status_lingual = kOutputOffLingual;
    }
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
//  if (echo_handler->IsSerialNumberEchoed()) {
//    auto sn = echo_handler->SerialNumber();
//    if (sn.isEmpty()) {
//      status_lingual = kFailLingual;
//    } else {
//      status_lingual = kSuccessLingual;
//    }
//    status->setText(which_lingual(status_lingual));
//    label->setText(sn);
//    button->setDisabled(false);
//  }
}

void OutputSwitchWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kSwitchOnLingual));
  combo->addItem(which_lingual(kSwitchOffLingual));
}

////////////////////// MeasureTriggerWidgets /////////////////////////////

MeasureTriggerWidgets::MeasureTriggerWidgets() {
  id = 0x04;
  item_lingual = {"Trigger Once", "指令触发"};
  button_lingual = {"Trigger", "触发"};
  SetOptionWidgetUINull();
  SetStatusLabelUINull();
}

void MeasureTriggerWidgets::ButtonClicked() {
  driver->TriggerOnce();
}

void MeasureTriggerWidgets::Update() {

}

////////////////////// FlashSettingsWidgets /////////////////////////////

FlashSettingsWidgets::FlashSettingsWidgets() {
  id = 0x11;
  timeout = 3000;
  item_lingual = {"Save Settings", "保存配置"};
  SetOptionWidgetUINull();
}

void FlashSettingsWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  driver->SaveSettingsToFlash();
}

////////////////////// RestoreFactoryWidgets /////////////////////////////

RestoreFactoryWidgets::RestoreFactoryWidgets() {
  id = 0x10;
  timeout = 3000;
  item_lingual = {"Restore Factory", "恢复出厂设置"};
  SetWidgetUINullLabel(option);
}

void RestoreFactoryWidgets::ButtonClicked() {
  QMessageBox box(button);
  box.setWindowTitle(which_lingual(kMsgBoxInfoTitle));
  box.setText(
      which_lingual(
          {"Are you sure to restore factory?",
           "确定恢复出厂设置？"}));
  box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  box.setButtonText(QMessageBox::Ok, which_lingual(kMsgBoxOk));
  box.setButtonText(QMessageBox::Cancel, which_lingual(kMsgBoxCancel));
  if (box.exec() != QMessageBox::Ok) {
    return;
  }
  CommandEchoWidgets::ButtonClicked();
  driver->RestoreFactory();
}

////////////////////// SetSerialBaudRateWidgets /////////////////////////////

SetSerialBaudRateWidgets::SetSerialBaudRateWidgets() {
  id = 0x06;
  item_lingual = {"Baud Rate", "串口波特率"};
  combo = new QComboBox;
  auto baud_rates = Driver::BaudRates();
  for (auto& rate : baud_rates) {
    combo->addItem(QString::number(rate));
  }
  option = combo;
}

void SetSerialBaudRateWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto rate = combo->currentText().toInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetDeviceBaudRate(rate);
}

void SetSerialBaudRateWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsBaudRateEchoed()) {
    auto rate = echo_handler->BaudRate();
    auto str = QString::number(rate);
    status_lingual = kSuccessLingual;
    status_lingual.eng = status_lingual.eng + ": " + str;
    status_lingual.chn = status_lingual.chn + ": " + str;
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
}

////////////////////// SetPortTypeWidgets /////////////////////////////

SetPortTypeWidgets::SetPortTypeWidgets() {
  id = 0x45;
  item_lingual = {"Port Type", "传输类型"};
  combo = new QComboBox;
  SetOptionLingual();
  option = combo;
}

void SetPortTypeWidgets::SetOptionLingual() {
  CommandEchoWidgets::SetOptionLingual();
  combo->clear();
  combo->addItem(which_lingual(kSerial));
  combo->addItem(which_lingual(kCAN));
}

void SetPortTypeWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto type = combo->currentText();
  if (lingual_equal(type, kSerial)) {
    driver->SetTransTypeSerial();
  } else if (lingual_equal(type, kCAN)) {
    driver->SetTransTypeCAN();
  }
}

////////////////////// SetOutputFormatWidgets /////////////////////////////

SetOutputFormatWidgets::SetOutputFormatWidgets() {
  id = 0x05;
  // timeout = 3000;
  item_lingual = {"Output Format", "输出格式"};
  combo = new QComboBox;
  option = combo;
}

void SetOutputFormatWidgets::SetOptionLingual() {
  CommandEchoWidgets::SetOptionLingual();
  combo->clear();
  combo->addItem(which_lingual(kNineBytes));
  combo->addItem(which_lingual(kPixhawk));
}

void SetOutputFormatWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto type = combo->currentText();
  if (lingual_equal(type, kNineBytes)) {
    driver->SetOutputFormatNineBytes();
  } else if (lingual_equal(type, kPixhawk)) {
    driver->SetOutputFormatPIX();
  }
}

void SetOutputFormatWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsOutputFormatEchoed()) {
    auto format = echo_handler->GetOutputFormat();
    status_lingual = kSuccessLingual;
    if (format == OutputFormat::nine_bytes) {
      status_lingual = status_lingual + ": " + kNineBytes;
    } else if (format == OutputFormat::pix) {
      status_lingual = status_lingual + ": " + kPixhawk;
    } else {
      return;
    }
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
}

////////////////////// SetCANIDWidgetsBase /////////////////////////////

SetCANIDWidgetsBase::SetCANIDWidgetsBase() {
  edit = new QLineEdit;
  SetLineEditIntValidity(edit, 0, 100000);
  option = edit;
}

////////////////////// SetCANSendIDWidgets /////////////////////////////

SetCANSendIDWidgets::SetCANSendIDWidgets() {
  id = 0x50;
  item_lingual = {"CAN Send ID", "CAN发送ID"};
}

void SetCANSendIDWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto id = edit->text().toUInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetCANSendID(id);
}

////////////////////// SetCANReceiveIDWidgets /////////////////////////////

SetCANReceiveIDWidgets::SetCANReceiveIDWidgets() {
  id = 0x51;
  item_lingual = {"CAN Receive ID", "CAN接收ID"};
}

void SetCANReceiveIDWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto id = edit->text().toUInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetCANReceiveID(id);
}

////////////////////// SetCANBaudRateWidgets /////////////////////////////


SetCANBaudRateWidgets::SetCANBaudRateWidgets() {
  id = 0x52;
  item_lingual = {"CAN Baud Rate", "CAN波特率"};
  auto rates = Driver::CANBaudRates();
  combo = new QComboBox;
  for (auto& rate : rates) {
    combo->addItem(QString::number(rate));
  }
  option = combo;
}

void SetCANBaudRateWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto rate = combo->currentText().toUInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetDeviceCANBaudRate(rate);
}

////////////////////// SetOutRangeValueWidgets /////////////////////////////

SetOutRangeValueWidgets::SetOutRangeValueWidgets() {
  id = 0x4f;
  item_lingual = {"Out-range Value", "超量程输出值"};
  edit = new QLineEdit;
  SetLineEditIntValidity(edit, 0, 65534);
  edit->setPlaceholderText(edit->placeholderText() + " (cm)");
  option = edit;
}

void SetOutRangeValueWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto value = edit->text().toUShort(&ok);
  if (!ok) {
    return;
  }
  driver->SetOutRangeValue(value);
}

////////////////////// RequestVersionWidgets /////////////////////////////

RequestVersionWidgets::RequestVersionWidgets() {
  id = 0x01;
  label = new QLabel;
  option = label;
  item_lingual = {"Firmware Version", "固件版本"};
  button_lingual = kButtonRequestLingual;
}

void RequestVersionWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  label->clear();
  driver->RequestVersion();
}

void RequestVersionWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsVersionEchoed()) {
    auto version = echo_handler->Version();
    label->setText(
        QString::number(version.major) + "." +
        QString::number(version.minor) + "." +
        QString::number(version.patch));
    status_lingual = kSuccessLingual;
    status->setText(which_lingual(status_lingual));
    button->setDisabled(false);
  }
}

////////////////////// WriteSerialNumberWidgets /////////////////////////////

WriteSerialNumberWidgets::WriteSerialNumberWidgets() {
  id = 0x56;
  edit = new QLineEdit;
  option = edit;
  edit->setInputMask(QString(kSNLength, 'n'));
  edit->setMaxLength(kSNLength);
  edit->setCursorPosition(0);
  item_lingual = {"Write Serial Number", "写入序列号"};
}

void WriteSerialNumberWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto sn = edit->text();
  if (sn.size() != kSNLength) {
    return;
  }
  driver->WriteSerialNumber(sn);
}

void WriteSerialNumberWidgets::SetOptionLingual() {
  edit->setPlaceholderText(
      which_lingual(
          {
            "Only accept " + QString::number(kSNLength) + " "
            "alphabets, numbers, or their combinations.",
            "接受" + QString::number(kSNLength) + "个字母或数字的组合。"
          }));
}

////////////////////// SetAPDWidgets /////////////////////////////

SetAPDWidgets::SetAPDWidgets() {
  id = 0x40;
  item_lingual = {"Set APD", "APD偏压"};
  edit = new QLineEdit;
  SetLineEditIntValidity(edit, 70, 200);
  option = edit;
}

void SetAPDWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  uint8_t value = edit->text().toInt(&ok);
  if (!ok) {
    return;
  }
  driver->SetAPD(value);
}

////////////////////// AutoGainAdjustWidgets /////////////////////////////

AutoGainAdjustWidgets::AutoGainAdjustWidgets() {
  id = 0x4E;
  item_lingual = {"Auto Gain", "自动增益调节"};
  combo = new QComboBox;
  option = combo;
}

void AutoGainAdjustWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kOnLingual));
  combo->addItem(which_lingual(kOffLingual));
}

void AutoGainAdjustWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto text = combo->currentText();
  if (lingual_equal(text, kOnLingual)) {
    driver->SetAutoGainAdjust(true);
  } else if (lingual_equal(text, kOffLingual)) {
    driver->SetAutoGainAdjust(false);
  }
}

////////////////////// AdaptiveAPDWidgets /////////////////////////////

AdaptiveAPDWidgets::AdaptiveAPDWidgets() {
  id = 0x4C;
  item_lingual = {"Adaptive APD", "APD电压自动调节"};
  combo = new QComboBox;
  option = combo;
}

void AdaptiveAPDWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kOnLingual));
  combo->addItem(which_lingual(kOffLingual));
}

void AdaptiveAPDWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto text = combo->currentText();
  if (lingual_equal(text, kOnLingual)) {
    driver->SetAdaptiveAPD(true);
  } else if (lingual_equal(text, kOffLingual)) {
    driver->SetAdaptiveAPD(false);
  }
}

////////////////////// APDClosedLoopWidgets /////////////////////////////

APDClosedLoopWidgets::APDClosedLoopWidgets() {
  id = 0x4D;
  item_lingual = {"APD Closed-loop", "APD闭环调节"};
  combo = new QComboBox;
  option = combo;
}

void APDClosedLoopWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kOnLingual));
  combo->addItem(which_lingual(kOffLingual));
}

void APDClosedLoopWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto text = combo->currentText();
  if (lingual_equal(text, kOnLingual)) {
    driver->SetAPDClosedLoop(true);
  } else if (lingual_equal(text, kOffLingual)) {
    driver->SetAPDClosedLoop(false);
  }
}

////////////////////// DistanceL1WriteWidgets /////////////////////////////

DistanceL1WriteWidgets::DistanceL1WriteWidgets() {
  id = 0x58;
  item_lingual = {"Write L1 (cm)", "写入L1 (cm)"};
  edit = new QLineEdit;
  SetLineEditUShortValidity(edit);
  option = edit;
}

void DistanceL1WriteWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto value = edit->text().toUShort(&ok);
  if (!ok) {
    return;
  }
  driver->SetDistanceL1(value);
}

////////////////////// DistanceL1ReadWidgets /////////////////////////////

DistanceL1ReadWidgets::DistanceL1ReadWidgets() {
  id = 0;
  item_lingual = {"Read L1 (cm)", "读取L1 (cm)"};
  SetOptionWidgetUINull();
  button_lingual = kButtonRequestLingual;
}

void DistanceL1ReadWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  driver->RequestDistanceL1();
}

void DistanceL1ReadWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsDistanceL1Echoed()) {
    auto distance = echo_handler->GetDistanceL1();
    if (distance.type == DistanceType::distance_l1) {
      if (distance.success) {
        status_lingual = kSuccessLingual;
        status_lingual = status_lingual + QString(": ") + QString::number(distance.distance);
      } else {
        status_lingual = kFailLingual;
      }
      status->setText(which_lingual(status_lingual));
      button->setDisabled(false);
    }
  }
}

////////////////////// DistanceLWriteWidgets /////////////////////////////

DistanceLWriteWidgets::DistanceLWriteWidgets() {
  id = 0x59;
  item_lingual = {"Write L (cm)", "写入L (cm)"};
  edit = new QLineEdit;
  SetLineEditUShortValidity(edit);
  option = edit;
}

void DistanceLWriteWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto value = edit->text().toUShort(&ok);
  if (!ok) {
    return;
  }
  driver->SetDistanceL(value);
}

////////////////////// DistanceLReadWidgets /////////////////////////////

DistanceLReadWidgets::DistanceLReadWidgets() {
  id = 0;
  item_lingual = {"Read L (cm)", "读取L (cm)"};
  button_lingual = kButtonRequestLingual;
  SetOptionWidgetUINull();
}

void DistanceLReadWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  driver->RequestDistanceL();
}

void DistanceLReadWidgets::Update() {
  CommandEchoWidgets::Update();
  if (echo_handler->IsDistanceLEchoed()) {
    auto distance = echo_handler->GetDistanceL();
    if (distance.type == DistanceType::distance_l) {
      if (distance.success) {
        status_lingual = kSuccessLingual;
        status_lingual =
            status_lingual + QString(": ") + QString::number(distance.distance);
      } else {
        status_lingual = kFailLingual;
      }
      status->setText(which_lingual(status_lingual));
      button->setDisabled(false);
    }
  }
}

////////////////////// CustomizationWidgets /////////////////////////////

CustomizationWidgets::CustomizationWidgets() {
  id = 0x5A;
  item_lingual = {"Customization", "定制化"};
  combo = new QComboBox;
  option = combo;
}

void CustomizationWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  auto text = combo->currentText();
  if (lingual_equal(text, kCommon)) {
    driver->SetCustomization(Customization::common);
  } else if (lingual_equal(text, kBL)) {
    driver->SetCustomization(Customization::bl);
  } else if (lingual_equal(text, kI13)) {
    driver->SetCustomization(Customization::i13);
  } else if (lingual_equal(text, kQL)) {
    driver->SetCustomization(Customization::ql);
  }
}

void CustomizationWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kCommon));
  combo->addItem(which_lingual(kBL));
  combo->addItem(which_lingual(kI13));
  combo->addItem(which_lingual(kQL));
}

////////////////////// RangeDetectWidgets /////////////////////////////

RangeDetectWidgets::RangeDetectWidgets() {
  id = 0;
  item_lingual = {"Range Detection", "检测量程"};
  combo = new QComboBox;
  option = combo;
}

void RangeDetectWidgets::SetOptionLingual() {
  combo->clear();
  combo->addItem(which_lingual(kOnLingual));
  combo->addItem(which_lingual(kOffLingual));
}

void RangeDetectWidgets::ButtonClicked() {
  static bool shown = false;
  if (lingual_equal(combo->currentText(), kOnLingual)) {
    if (!shown) {
      shown = true;
      QMessageBox box(button);
      box.setWindowTitle(which_lingual(kMsgBoxInfoTitle));
      box.setText(
          which_lingual(
              {"1. This feature only works in Devel mode;\n"
               "2. Distance will flash red when out-range is detected.",
               "1. 该功能只在开发模式下工作；\n"
               "2. 超量程时，距离数字将显示红色。"}));
      box.setStandardButtons(QMessageBox::Ok);
      box.setButtonText(QMessageBox::Ok, which_lingual(kMsgBoxOk));
      box.exec();
    }
    status_lingual = kOnLingual;
    driver->RangeDetectionTask(true);
  } else if (lingual_equal(combo->currentText(), kOffLingual)) {
    status_lingual = kOffLingual;
    driver->RangeDetectionTask(false);
  }
  status->setText(which_lingual(status_lingual));
}

////////////////////// HorizontalAngleWidgets /////////////////////////////

HorizontalAngleWidgets::HorizontalAngleWidgets() {
  id = 0x5B;
  item_lingual = {"Horizontal Angle", "水平安装角度"};
  edit = new QLineEdit;
  option = edit;
  SetLineEditIntValidity(edit, 0, 90);
}

void HorizontalAngleWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  bool ok;
  auto value = edit->text().toInt(&ok);
  if (ok) {
    driver->SetHorizontalAngle(value);
  }
}

////////////////////// RangeValidityWidgets /////////////////////////////

RangeValidityWidgets::RangeValidityWidgets() {
  id = 0x00;
  item_lingual = {"Range Validity", "量程筛选"};
  label = new QLabel;
  option = label;
  timeout = 2000;
  button_lingual = {"Detect", "检测"};
}

void RangeValidityWidgets::ButtonClicked() {
  CommandEchoWidgets::ButtonClicked();
  option_lingual = Lingual();
  label->clear();
  driver->RangeDetectionTask(true);
}

void RangeValidityWidgets::Update() {
  if (button->isEnabled()) {
    return;
  }
  if (timer.elapsed() > timeout) {
    button->setDisabled(false);
    status->setText(which_lingual(kNoResponseLingual));
    status_lingual = kNoResponseLingual;
    driver->RangeDetectionTask(false);
  }

  if (echo_handler->IsRangeDetectEchoed()) {
    if (echo_handler->IsRangeDetectTooNear()) {
      QMessageBox box(button);
      box.setWindowTitle(which_lingual(kMsgBoxErrorTitle));
      box.setText(which_lingual(
          {
            "A short distance detected.",
            "检测到过短距离。"
          }));
      box.addButton(QMessageBox::Ok);
      box.setButtonText(QMessageBox::Ok, which_lingual(kMsgBoxOk));
      box.exec();
      option_lingual = Lingual();
      label->clear();
      driver->RangeDetectionTask(false);
    } else {
      if (echo_handler->IsOutOfRange()) {
        option_lingual = kInvalid;
      } else {
        option_lingual = kValid;
      }
      label->setText(which_lingual(option_lingual));
    }
    button->setDisabled(false);
    driver->RangeDetectionTask(false);
  }
}

void RangeValidityWidgets::SetOptionLingual() {
  label->setText(which_lingual(option_lingual));
}
