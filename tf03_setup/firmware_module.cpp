#include "firmware_module.h"
#include <QFile>
#include <QDebug>
#include "parsed.h"
#include "driver.h"
#include <QProgressBar>
#include <QPushButton>
#include <QMessageBox>
#include "command_echo_handler.h"
#include <QLabel>
#include <QFileDialog>
#include <QComboBox>

FirmwareModule::FirmwareModule()
{
  SetModeSerial();
}

void FirmwareModule::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void FirmwareModule::SetModeCAN() {
//  segments_per_send_ = 10;
  segments_per_send_ = 10;
  segment_length_ = 2;
}

void FirmwareModule::SetModeSerial() {
  segments_per_send_ = 1;
  segment_length_ = 249;
}

bool FirmwareModule::LoadBin(const QString &path) {
  QFile file(path);
  file.open(QIODevice::ReadOnly);
  auto stream = file.readAll();
  file.close();
  if (stream.isEmpty()) {
    return false;
  }
  while (!queue_.empty()) {
    queue_.pop();
  }
  int index = 0;
  while (index < stream.size()) {
    auto end = index + segment_length_;
    if (end > stream.size()) {
      end = stream.size();
    }
    QByteArray seg;
    seg.reserve(segment_length_);
    for (int i = index; i < end; ++i){
      seg.push_back(stream[i]);
    }
    queue_.push(seg);
    index += segment_length_;
  }
  segment_total_ = queue_.size();
  bin_bytes_ = stream.size();
  return true;
}

int FirmwareModule::SegmentNum() {
  return queue_.size();
}

bool FirmwareModule::Step() {
  driver_->SendFirmwareFirstSegment(
      bin_bytes_, QByteArray(segment_length_ - 2, 0));
  return true;
}

bool FirmwareModule::Step(const FirmwareUpdateStatus &status) {
  if (status != FirmwareUpdateStatus::ok) {
    return false;
  }

  auto id = segment_total_ - SegmentNum() + 1;

  std::vector<QByteArray> segments;
  for (int i = 0; i < segments_per_send_; ++i) {
    if (queue_.empty()) {
      break;
    }
    auto seg = queue_.front();
    queue_.pop();
    segments.push_back(seg);
  }

  if (queue_.empty()) {
    if (!segments.empty()) {
      *segments.rbegin() +=
          QByteArray(segment_length_ - segments.rbegin()->size(), 0);
    }
  }

  driver_->SendFirmwareMultiSegment(id, segments);

  return true;
}

float FirmwareModule::GetProgress() {
  return (segment_total_ - SegmentNum()) * 1.0f / segment_total_;
}

////////////////////// UpgradeFirmwareWidgets /////////////////////////////

UpgradeFirmwareWidgets::UpgradeFirmwareWidgets() {
  id = 0x49;
  item_lingual = {"Upgrade Firmware", "升级固件"};
  button_lingual = kUpgradeLingual;
  browse = new QPushButton;
  combo = new QComboBox;
  option = combo;
//  browse->setFlat(true);
  progress = new QProgressBar;
  progress->setMinimum(0);
  progress->setMaximum(100);
  progress->setValue(0);
  progress->setVisible(false);
  status->setVisible(false);
//  DeleteStatusWidget();
//  status_base = progress;
  connect(browse, SIGNAL(clicked()), this, SLOT(OnBrowsing()));
  browse_lingual = kSelectBinaryLingual;
}

void UpgradeFirmwareWidgets::ButtonClicked() {
  if (!module && driver) {
    module.reset(new FirmwareModule);
    module->SetDriver(driver);
  }
  if (lingual_equal(button->text(), kUpgradeLingual)) {
    if (bin_file.isEmpty()) {
      return;
    }
    if (lingual_equal(combo->currentText(), kSerial)) {
      module->SetModeSerial();
    } else if (lingual_equal(combo->currentText(), kCAN)) {
      module->SetModeCAN();
    } else {
      return;
    }
    if (!module->LoadBin(bin_file)) {
      QMessageBox box(button);
      box.setWindowTitle(which_lingual(kMsgBoxErrorTitle));
      box.setText(
          which_lingual({"Failed to open firmware file.", "固件文件无法打开。"}));
      box.setStandardButtons(QMessageBox::Abort);
      box.setButtonText(QMessageBox::Abort, which_lingual(kMsgBoxAbort));
      box.exec();
      HandleFailure();
      return;
    }
    if (!before_upgrade_instructions_shown_) {
      QMessageBox box(button);
      box.setWindowTitle(which_lingual(kMsgBoxInfoTitle));
      box.setText(
          which_lingual(
              {"Please read through following instructions before upgrade:\n"
               "  1. Please unplug the device and reconnect it before continue.\n"
               "  2. The program will switch off the sensor output during upgrade;\n"
               "     You may manually switch it back after the upgrade.\n"
               "  3. During upgrade, all setup buttons will not respond clicks.",
               "升级前请仔细阅读以下注意事项：\n"
               "  1. 升级前，请拔下设备并重连。\n"
               "  2. 固件升级期间，程序将关闭传感器输出；\n"
               "     如需恢复，请在升级结束后手动设置。\n"
               "  3. 升级期间，所有设置按钮将不响应点击。"}));
      box.setStandardButtons(QMessageBox::Ok);
      box.setButtonText(QMessageBox::Ok, which_lingual(kMsgBoxOk));
      box.exec();
      before_upgrade_instructions_shown_ = true;
      return;
    }

    if (set_respond_all_button) {
      set_respond_all_button(false);
    }
//    driver->SetOutputSwitchOff();
    driver->SetBufferCleanerBytes(200);
    button_lingual = kStopLingual;
    button->setText(which_lingual(button_lingual));
    timer.restart();
    progress->setValue(0);
    progress->setVisible(true);
    status->setVisible(false);
    module->Step();
  } else if (lingual_equal(button->text(), kStopLingual)) {
    HandleFailure();
  }
}

void UpgradeFirmwareWidgets::Update() {
  if (!lingual_equal(button->text(), kStopLingual)) {
    return;
  }
  if (timer.elapsed() > 4000) {
    HandleFailure();
    return;
  }
  if (module->SegmentNum() == 0) {
    HandleSuccess();
    return;
  }
  if (echo_handler->IsFirmwareUpdateEchoed()) {
    if (!module->Step(echo_handler->GetFirmwareUpdateStatus())) {
      HandleFailure();
      return;
    } else {
      timer.restart();
      progress->setValue(module->GetProgress() * 100);
    }
  }
}

void UpgradeFirmwareWidgets::SetOptionLingual() {
  browse->setText(which_lingual(browse_lingual));
  combo->clear();
  combo->addItem(which_lingual(kSerial));
  combo->addItem(which_lingual(kCAN));
}

void UpgradeFirmwareWidgets::HandleStop() {
  status->setVisible(true);
  progress->setVisible(false);
  button_lingual = kUpgradeLingual;
  button->setText(which_lingual(button_lingual));
  bin_file.clear();
  browse_lingual = kSelectBinaryLingual;
  browse->setText(which_lingual(browse_lingual));
  driver->SetBufferCleanerBytesDefault();
  if (set_respond_all_button) {
    set_respond_all_button(true);
  }
}

void UpgradeFirmwareWidgets::HandleSuccess() {
  status_lingual = kSuccessLingual;
  status->setText(which_lingual(status_lingual));
  HandleStop();
}

void UpgradeFirmwareWidgets::HandleFailure() {
  status_lingual = kFailLingual;
  status->setText(which_lingual(status_lingual));
  HandleStop();
}

void UpgradeFirmwareWidgets::OnBrowsing() {
  auto file =
      QFileDialog::getOpenFileName(
          option, which_lingual({"Select Firmware File", "选择固件文件"}),
          QDir::homePath(), "Firmware File (*.bin)");
  if (file.isEmpty()) {
    return;
  }
  bin_file = file;
  browse_lingual = kBinaryReady;
  browse->setText(which_lingual(browse_lingual));
}
