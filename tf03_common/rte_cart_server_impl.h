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
  bool I037BurnCallback(std::shared_ptr<std::list<CartStep>> steps) override;
  bool I037TempBurnCallback(std::shared_ptr<std::list<CartStep>> steps) override;
  bool AutoIntCheckCallback(std::shared_ptr<std::list<CartStep>> steps) override;
private:
  std::shared_ptr<TFMiniDriverImpl> sensor_;
};

#endif // RTE_CART_SERVER_IMPL_H
