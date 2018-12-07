#ifndef RTE_CART_SERVER_H
#define RTE_CART_SERVER_H

#include "driver_server.h"
#include "mini_rte_cart.h"
#include "tfmini_driver.h"

class API RTECartServer : public DriverServer
{
  Q_OBJECT
public:
  RTECartServer();
  bool Start();
  void SetDriver(std::shared_ptr<MiniRTECart> driver);
  void SetMiniDriver(std::shared_ptr<TFMiniDriver> driver);
  std::shared_ptr<MiniRTECart> Driver();
  void Spin() override;
  void PrintMeasures(std::shared_ptr<std::list<Message>> measures);
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
  std::shared_ptr<TFMiniDriver> sensor_;
  RTEStageType last_stage_;
};

#endif // RTE_CART_SERVER_H
