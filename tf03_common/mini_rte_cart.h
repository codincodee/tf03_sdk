#ifndef MINI_RTE_CART_H
#define MINI_RTE_CART_H

#include "cart_driver.h"

enum class RTEStageType {
  i037,
  i037_burn,
  i037_back,
  heating,
  i037_temp,
  i037_temp_burn,
  auto_i,
  stop
};

using MessageList = std::shared_ptr<std::list<Message>>;

struct MiniCartStep {
  int position = 0;
  MessageList measures;
};

class API MiniRTECart : public CartDriver
{
public:
  MiniRTECart();
  bool Start() override;
  void StartCart(
      const uint32_t &distance, const uint32_t &step_length_) override;
  RTEStageType RTEStage();
  void FinishedHeating();
  void RegisterIntTimeTriggerCallback(std::function<void(int)> func);
  void RegisterRetrieveMeasuresCallback(std::function<MessageList()> func);
protected:
  void OnStep(const int& position) override;
  void OnEndPoint() override;
  void OnStartPoint() override;
private:
  std::atomic<RTEStageType> stage_;
  enum class I037Type {i0 = 0, i3 = 3, i7 = 7};
  std::atomic<I037Type> current_037_;
  std::function<void(int)> trigger_inttime_measure_;
  std::function<MessageList()> retrieve_measures_;
};

#endif // MINI_RTE_CART_H
