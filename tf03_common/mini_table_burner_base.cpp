#include "mini_table_burner_base.h"

MiniTableBurnerBase::MiniTableBurnerBase()
{

}

void MiniTableBurnerBase::SetDriver(std::shared_ptr<TFMiniDriver> driver) {
  this->driver = driver;
}

void MiniTableBurnerBase::EnterSettingMode() {

}
