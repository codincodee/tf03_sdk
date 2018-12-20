#ifndef MINI_TABLE_BURNER_H
#define MINI_TABLE_BURNER_H

#include "export.h"
#include "tfmini_driver.h"
#include "mini_table_burner_base.h"

class API MiniTableBurner : public MiniTableBurnerBase
{
public:
  MiniTableBurner();
  bool StartBurn(
      std::vector<uint16_t>& tab_0,
      std::vector<uint16_t>& tab_3,
      std::vector<uint16_t>& tab_7);
};

#endif // MINI_TABLE_BURNER_H
