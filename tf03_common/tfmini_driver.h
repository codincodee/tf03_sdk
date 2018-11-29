#ifndef TFMINI_DRIVER_H
#define TFMINI_DRIVER_H

#include "driver_base.h"
#include "export.h"

class API TFMiniDriver : public DriverBase
{
public:
  TFMiniDriver();
protected:
  void LoadAllParsers(std::vector<ReceiveParser>& parsers) override;
private:
  bool ParseDevelModeMeasure(
      const QByteArray &buffer, Message &parsed, int &from, int &to);
  static QByteArray Parse29BytesMessageAtFront(
      const QByteArray &buffer, int &from, int &to);
};

#endif // TFMINI_DRIVER_H
