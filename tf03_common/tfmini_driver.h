#ifndef TFMINI_DRIVER_H
#define TFMINI_DRIVER_H

#include "driver_base.h"
#include "export.h"

enum class TFMiniOutputFormat {
  b_9 = 0x01, b_29 = 0x03,
};

class API TFMiniDriver : public DriverBase
{
public:
  TFMiniDriver();
  void SetOutputFormat(const TFMiniOutputFormat& format);
  void TriggerIntTimeMeasure(const int& inttime);
  void SetMetricUnit(const bool& mm);
  void SetTimer(const bool& on);
protected:
  void LoadAllParsers(std::vector<ReceiveParser>& parsers) override;
private:
  bool ParseDevelModeMeasure(
      const QByteArray &buffer, Message &parsed, int &from, int &to);
  static QByteArray Parse29BytesMessageAtFront(
      const QByteArray &buffer, int &from, int &to);
  static QByteArray CommonCommand(const char& id, const char& single);
};

#endif // TFMINI_DRIVER_H
