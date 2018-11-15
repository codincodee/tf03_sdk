#ifndef TF02_DRIVER_H
#define TF02_DRIVER_H

#include "export.h"
#include "driver_base.h"

class TF02Driver : public DriverBase
{
public:
  TF02Driver();
protected:
  void LoadAllParsers(std::vector<ReceiveParser>& parsers) override;
};

#endif // TF02_DRIVER_H
