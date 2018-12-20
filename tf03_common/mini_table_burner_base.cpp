#include "mini_table_burner_base.h"
#include <QThread>

MiniTableBurnerBase::MiniTableBurnerBase()
{

}

void MiniTableBurnerBase::SetDriver(std::shared_ptr<TFMiniDriver> driver) {
  this->driver = driver;
}

void MiniTableBurnerBase::EnterSettingMode() {
  if (!driver) {
    return;
  }
  driver->EnterSettingMode();
  QThread::msleep(100);
}

uint8_t MiniTableBurnerBase::DataRegFrom() {
  return 0x10;
}

uint8_t MiniTableBurnerBase::DataRegEnd() {
  return 0x1E;
}

void MiniTableBurnerBase::SendDataFrame(
    const uint8_t &addr, const QByteArray &data) {
  if (!driver) {
    return;
  }
  if (data.size() > 4) {
    return;
  }
  char head[4] = {42, 57, 02, 0};
  memcpy(head + 3, &addr, 1);
  driver->EnqueueSendMessage(QByteArray::fromRawData(head, 4) + data);
  QThread::msleep(100);
}
