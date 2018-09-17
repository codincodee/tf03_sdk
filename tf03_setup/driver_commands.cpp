#include "driver.h"
#include <QThread>
#include <QSerialPort>
#include <QDebug>
#include "apd_exp_task.h"

void Driver::EnqueueCommand(const CommandFunc &command) {
  command_queue_mutex_.lock();
  command_queue_.push(command);
  command_queue_mutex_.unlock();
}

void Driver::SetDevelMode() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x44), QByteArray(1, 2)));
  });
}

void Driver::SetReleaseMode() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x44), QByteArray(1, 1)));
  });
}

void Driver::SetFrequency(const unsigned short &frequency) {
  EnqueueCommand([this, frequency](){
    return SendMessage(CommonCommand(char(0x03), to_bytes(frequency)));
  });
}

void Driver::RequestSerialNumber() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x56), QByteArray(1, 0x00)));
  });
}

void Driver::SetOutputSwitchOn() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x07), QByteArray(1, 0x01)));
  });
}

void Driver::SetOutputSwitchOff() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x07), QByteArray(1, 0x00)));
  });
}

void Driver::TriggerOnce() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x04), QByteArray()));
  });
}

void Driver::SaveSettingsToFlash() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x11), QByteArray()));
  });
}

void Driver::RestoreFactory() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x10), QByteArray()));
  });
}

void Driver::SetDeviceBaudRate(const uint32_t &rate) {
  EnqueueCommand([this, rate](){
    return SendMessage(CommonCommand(char(0x06), to_bytes(rate)));
  });
}

void Driver::SetTransTypeCAN() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x45), QByteArray(1, 0x02)));
  });
}

void Driver::SetOutputFormatNineBytes() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x05), QByteArray(1, 0x01)));
  });
}

void Driver::SetOutputFormatPIX() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x05), QByteArray(1, 0x02)));
  });
}

void Driver::SetCANSendID(const uint32_t &id) {
  EnqueueCommand([this, id](){
    return SendMessage(CommonCommand(char(0x50), to_bytes(id)));
  });
}

void Driver::SetCANReceiveID(const uint32_t &id) {
  EnqueueCommand([this, id](){
    return SendMessage(CommonCommand(char(0x51), to_bytes(id)));
  });
}

void Driver::SetDeviceCANBaudRate(const uint32_t &rate) {
  EnqueueCommand([this, rate](){
    return SendMessage(CommonCommand(char(0x52), to_bytes(rate)));
  });
}

void Driver::SetOutRangeValue(const uint16_t &value) {
  EnqueueCommand([this, value](){
    return SendMessage(CommonCommand(char(0x4f), to_bytes(value)));
  });
}

void Driver::SendFirmwareSegment(const uint16_t& id, const QByteArray &seg) {
  EnqueueCommand([this, id, seg](){
    return SendMessage(CommonCommand(char(0x49), to_bytes(id) + seg));
  });
}

void Driver::SendFirmwareLastSegment(const uint16_t &id, const QByteArray &seg) {
  EnqueueCommand([this, id, seg](){
    return SendMessage(CommonCommand(char(0x49), to_bytes(id) + seg));
  });
}

//void Driver::SendFirmwareLastSegment(const uint16_t &id, const QByteArray &seg) {
//  auto msg = CommonCommand(char(0x49), to_bytes(id) + seg);
//  *msg.rbegin() = 'B';
//  msg += "ENEWAKE";
//  msg += QByteArray(4, 0);
//  int32_t sum = 0;
//  for (auto& c : msg) {
//    sum += c;
//  }
//  memcpy(msg.end() - 4, &sum, 4);
//  EnqueueCommand([this, msg]{
//    return SendMessage(msg);
//  });
//}

void Driver::SendFirmwareFirstSegment(
    const uint16_t &bytes, const QByteArray &seg) {
  EnqueueCommand([this, bytes, seg](){
    return SendMessage(
        CommonCommand(char(0x49), to_bytes(uint16_t(0)) + to_bytes(bytes) + seg));
  });
}

void Driver::SendFirmwareMultiSegment(
    const uint16_t &id, const std::vector<QByteArray> &segments) {
  EnqueueCommand([this, id, segments](){
    auto i = id;
    auto read = (segments.size() != 1);
    for (auto& segment : segments) {
      if (!SendMessage(CommonCommand(char(0x49), to_bytes(i++) + segment))) {
        return false;
      }
      if (read) {
        if (serial_port_->waitForReadyRead(100)) {
          buffer_ += serial_port_->readAll();
          ProcessBufferInWorkThread(buffer_);
        }
      }
    }
    return true;
  });
}

void Driver::RequestVersion() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x01), QByteArray()));
  });
}

void Driver::WriteSerialNumber(const QString &sn) {
//  QString ns = sn;
//  std::reverse_copy(sn.begin(), sn.end(), ns.begin());
  EnqueueCommand([this, sn](){
    return SendMessage(
        CommonCommand((0x56), QByteArray(1, 0x01) + sn.toLocal8Bit()));
  });
}

void Driver::SetAPD(const uint8_t &voltage) {
  char c;
  memcpy(&c, &voltage, 1);
  EnqueueCommand([this, c](){
    return SendMessage(CommonCommand(char(0x40), QByteArray(1, c)));
  });
}

void Driver::SetAutoGainAdjust(const bool &on) {
  EnqueueCommand([this, on](){
    return SendMessage(CommonCommand(char(0x4E), QByteArray(1, on ? 0 : 1)));
  });
}

void Driver::SetAdaptiveAPD(const bool &on) {
  EnqueueCommand([this, on](){
    return SendMessage(CommonCommand(char(0x4C), QByteArray(1, on ? 0 : 1)));
  });
}

void Driver::SetAPDClosedLoop(const bool &on) {
  EnqueueCommand([this, on](){
    return SendMessage(CommonCommand(char(0x4D), QByteArray(1, on ? 0 : 1)));
  });
}

void Driver::APDExperimentOn() {
  EnqueueCommand([this](){
    if (apd_exp_task_) {
      apd_exp_task_->Start();
      return true;
    }
    return false;
  });
}

void Driver::APDExperimentOff() {
  EnqueueCommand([this](){
    if (apd_exp_task_) {
      apd_exp_task_->Stop();
      return true;
    }
    return false;
  });
}

void Driver::SetDistanceL1(const uint16_t &value) {
  EnqueueCommand([this, value](){
    return SendMessage(
        CommonCommand(char(0x58), QByteArray(1, 0x01) + to_bytes(value)));
  });
}

void Driver::SetDistanceL(const uint16_t &value) {
  EnqueueCommand([this, value](){
    return SendMessage(
        CommonCommand(char(0x59), QByteArray(1, 0x01) + to_bytes(value)));
  });
}

void Driver::RequestDistanceL1() {
  EnqueueCommand([this](){
    return SendMessage(
        CommonCommand(char(0x58), QByteArray(1, 0x00)));
  });
}

void Driver::RequestDistanceL() {
  EnqueueCommand([this](){
    return SendMessage(
        CommonCommand(char(0x59), QByteArray(1, 0x00)));
  });
}

void Driver::SetCustomization(const Customization &type) {
  char id;
  switch (type) {
  case Customization::common: id = 0; break;
  case Customization::bl: id = 1; break;
  case Customization::i13: id = 2; break;
  default: return;
  }
  EnqueueCommand([this, id](){
    return SendMessage(
        CommonCommand(char(0x5A), QByteArray(1, id)));
  });
}

void Driver::SetTransTypeSerial() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x45), QByteArray(1, 0x01)));
  });
}

std::vector<Message> Driver::GetMessages() {
  receive_messages_mutex_.lock();
  auto messages = std::move(receive_messages_);
  receive_messages_mutex_.unlock();
  return messages;
}

QByteArray Driver::CommonCommand(const char &id, const QByteArray &data) {
  QByteArray msg;
  msg += 0x5a;
  msg += data.size() + 4;
  msg += id;
  msg += data;
  msg += CalculateSum(msg);
  return msg;
}

QByteArray Driver::CalculateSum(const QByteArray &msg) {
  char sum = 0;
  for (auto& b : msg) {
    sum += b;
  }
  return QByteArray(1, sum);
}
