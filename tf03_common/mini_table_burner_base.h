#ifndef MINI_TABLE_BURNER_BASE_H
#define MINI_TABLE_BURNER_BASE_H

#include "export.h"
#include "tfmini_driver.h"

class API MiniTableBurnerBase
{
public:
  MiniTableBurnerBase();
  void SetDriver(std::shared_ptr<TFMiniDriver> driver);
protected:
  void EnterSettingMode();
  static uint8_t DataRegFrom();
  static uint8_t DataRegEnd();
  void SendDataFrame(const uint8_t& addr, const QByteArray& data);
  std::shared_ptr<TFMiniDriver> driver;
};

#endif // MINI_TABLE_BURNER_BASE_H
