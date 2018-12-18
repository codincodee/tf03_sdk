#ifndef RTE_CART_SERVER_IMPL_H
#define RTE_CART_SERVER_IMPL_H

#include "rte_cart_server.h"

class TFMiniDriverImpl;

class API RTECartServerImpl : public RTECartServer
{
public:
  RTECartServerImpl();
protected:
  bool OnInitialized() override;
  void Spin() override;
  bool I037BurnCallback(std::list<MiniCartStep> steps) override;
  bool I037TempBurnCallback(std::list<MiniCartStep> steps) override;
  bool AutoIntCheckCallback(std::list<MiniCartStep> steps) override;
private:
  std::shared_ptr<TFMiniDriverImpl> sensor_;
};

#endif // RTE_CART_SERVER_IMPL_H
