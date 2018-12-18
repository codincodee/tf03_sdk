#ifndef RTE_CART_SERVER_H
#define RTE_CART_SERVER_H

#include "driver_server.h"
#include "mini_rte_cart.h"
#include "tfmini_driver.h"

class MiniTableBurner;

struct MiniCartStep29B {
  int position = 0;
  std::list<MiniMeasure29B> measures;
};

class API RTECartServer : public DriverServer
{
  Q_OBJECT
public:
  RTECartServer();
  virtual ~RTECartServer();
  bool Start();
  void SetDriver(std::shared_ptr<MiniRTECart> driver);
  void SetMiniDriver(std::shared_ptr<TFMiniDriver> driver);
  std::shared_ptr<MiniRTECart> Driver();
  void Spin() override;
  void PrintSteps(std::shared_ptr<std::list<CartStep>> measures);
  void PrintSteps(const std::list<MiniCartStep>& measures);
protected:
  bool OnInitialized() override;
  std::shared_ptr<TFMiniDriver> Sensor();
  bool I037BurnCallback(std::list<MiniCartStep> steps);
  virtual bool I037BurnCallback(std::list<MiniCartStep29B> steps);
  bool I037TempBurnCallback(std::list<MiniCartStep> steps);
  virtual bool I037TempBurnCallback(std::list<MiniCartStep29B> steps);
  bool AutoIntCheckCallback(std::list<MiniCartStep> steps);
  virtual bool AutoIntCheckCallback(std::list<MiniCartStep29B> steps);
  std::list<MiniCartStep29B> ConvertToMiniCartStep29B(
      std::list<MiniCartStep> steps);
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

  void HandleOnI037BurnFinished(std::shared_ptr<std::list<CartStep>> measures);
  std::shared_ptr<MiniRTECart> driver_;
  std::shared_ptr<TFMiniDriver> sensor_;
  std::shared_ptr<MiniTableBurner> burner_;
  RTEStageType last_stage_;
};

#endif // RTE_CART_SERVER_H
