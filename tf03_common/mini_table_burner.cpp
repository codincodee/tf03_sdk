#include "mini_table_burner.h"

MiniTableBurner::MiniTableBurner()
{

}

bool MiniTableBurner::StartBurn(
    std::vector<uint16_t> &tab_0,
    std::vector<uint16_t> &tab_3,
    std::vector<uint16_t> &tab_7) {
  if (!driver) {
    return false;
  }
  EnterSettingMode();
  const int kNumEntryPerLoop = (DataRegEnd() - DataRegFrom() + 1) * 2;
  const int kNumLoop_0 = std::ceil(1.0f * tab_0.size() / kNumEntryPerLoop);
  constexpr int kBytes = 2;

  if ((tab_0.size() % 2) != 0) {
    tab_0.push_back(0);
  }
  int index = 0;
  for (int i = 0; i < kNumLoop_0; ++i) {
    if (index >= tab_0.size()) {
      break;
    }
    char buf[kBytes * 2];
    memcpy(buf, &tab_0[index++], kBytes);
    memcpy(buf + kBytes, &tab_0[index++], kBytes);
    QByteArray buffer;
    buffer.fromRawData(buf, kBytes * 2);
    SendDataFrame((DataRegFrom() + i), buffer);
  }
  return true;
}
