#include "tfmini_driver.h"

QByteArray TFMiniDriver::CommonCommand(const char& id, const char& single) {
  return
      QByteArray(1, 0x42) + QByteArray(1, 0x57) +
      QByteArray(1, 0x02) +
      QByteArray(1, 0x00) +
      QByteArray(2, 0x00) + QByteArray(1, single) +
      QByteArray(1, id);
}

void TFMiniDriver::SetOutputFormat(const TFMiniOutputFormat &format) {
  EnqueueCommand([this, format](){
    return SendMessage(CommonCommand(char(0x06), char(format)));
  });
}

void TFMiniDriver::TriggerIntTimeMeasure(const int &inttime) {
  EnqueueCommand([this, inttime](){
    return SendMessage(CommonCommand(char(0x42), char(inttime)));
  });
}

void TFMiniDriver::SetMetricUnit(const bool &mm) {
  EnqueueCommand([this, mm](){
    return SendMessage(CommonCommand(char(0x1A), char(mm ? 0x00 : 0x01)));
  });
}

void TFMiniDriver::SetTimer(const bool &on) {
  EnqueueCommand([this, on](){
    return SendMessage(CommonCommand(char(0x40), char(on ? 0x01 : 0x00)));
  });
}

void TFMiniDriver::SetIntTimeMode(const TFMiniIntTimeMode &mode) {
  EnqueueCommand([this, mode](){
    return SendMessage(CommonCommand(char(0x14), char(mode)));
  });
}

void TFMiniDriver::EnterSettingMode() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x02), char(0x01)));
  });
}
