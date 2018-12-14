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
  std::shared_ptr<TFMiniDriver> driver;
};

#endif // MINI_TABLE_BURNER_BASE_H
