#include "mini_table_burner.h"

MiniTableBurner::MiniTableBurner()
{

}

bool MiniTableBurner::StartBurn(
    const std::vector<uint16_t> &tab_0,
    const std::vector<uint16_t> &tab_3,
    const std::vector<uint16_t> &tab_7) {
  if (!driver) {
    return false;
  }
  EnterSettingMode();
}
