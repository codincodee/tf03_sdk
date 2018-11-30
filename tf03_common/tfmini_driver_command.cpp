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
