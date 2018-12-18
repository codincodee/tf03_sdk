#ifndef TFMINI_DRIVER_H
#define TFMINI_DRIVER_H

#include "driver_base.h"
#include "export.h"

enum class TFMiniOutputFormat {
  b_9 = 0x01, b_29 = 0x03,
};

enum class TFMiniIntTimeMode {
  typical = 0x00, fixed = 0x01
};

class API TFMiniDriver : public DriverBase
{
public:
  TFMiniDriver();
  virtual ~TFMiniDriver();
  void SetOutputFormat(const TFMiniOutputFormat& format);
  void TriggerIntTimeMeasure(const int& inttime);
  void SetMetricUnit(const bool& mm);
  void SetTimer(const bool& on);
  void SetIntTimeMode(const TFMiniIntTimeMode& mode);
  void EnterSettingMode();
protected:
  void LoadAllParsers(std::vector<ReceiveParser>& parsers) override;
  static QByteArray CommonCommand(const char& id, const char& single);
private:
  bool ParseDevelModeMeasure(
      const QByteArray &buffer, Message &parsed, int &from, int &to);
  static QByteArray Parse29BytesMessageAtFront(
      const QByteArray &buffer, int &from, int &to);
};

#endif // TFMINI_DRIVER_H
