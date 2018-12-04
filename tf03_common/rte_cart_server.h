#ifndef RTE_CART_SERVER_H
#define RTE_CART_SERVER_H

#include "driver_server.h"
#include "mini_rte_cart.h"

class API RTECartServer : public DriverServer
{
  Q_OBJECT
public:
  RTECartServer();
  bool Start();
  void SetDriver(std::shared_ptr<MiniRTECart> driver);
  std::shared_ptr<MiniRTECart> Driver();
  void Spin() override;
protected:
  bool OnInitialized() override;
signals:
  void I037Burn();
  void I037TempBurn();
  void Heating();
  void Finished();
  void I037Back();
  void AutoI();
  void I037Temp();
private:
  void OnI037Burn();
  void OnI037TempBurn();
  void OnHeating();
  void OnStop();
  void OnAutoI();
  void OnI037Back();
  void OnI037Temp();
  std::shared_ptr<MiniRTECart> driver_;
  RTEStageType last_stage_;
};

#endif // RTE_CART_SERVER_H
