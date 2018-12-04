#ifndef CART_DRIVER_H
#define CART_DRIVER_H

#include "export.h"
#include "driver_base.h"

enum class CartSendCommandType {
  start, end, step
};

struct API CartStep {
  int position = 0;
  std::unique_ptr<MeasureBasic> measure;
  bool first_half = true;
};

class API CartDriver : public DriverBase
{
public:
  CartDriver();
  virtual ~CartDriver();
  virtual bool Start();
//  bool IsEnded();
  virtual std::vector<int> DriverBaudRates() override;
  std::list<CartStep> GetFullSteps();
  std::shared_ptr<std::list<CartStep>> GetStepBuffer();
  void SetStepLength(const int& len);
  int GetStepLength();
  void RegisterMeasureCallback(
      std::function<std::unique_ptr<MeasureBasic>()> func);
  virtual void StartCart(const uint32_t& distance, const uint32_t& step_length_);
  void SetDistance(const int& distance);
  [[deprecated]] CartSendCommandType CurrentStage();
  void GoOn();
protected:
  void LoadAllParsers(std::vector<ReceiveParser>& parsers) override;
  virtual void OnStep(const int& current_position);
  virtual void OnEndPoint();
  virtual void OnStartPoint();
  QByteArray CommonCommand(
      const char& id,
      const uint32_t &arg1, const uint32_t &arg2,
      const uint32_t &arg3, const uint32_t& arg4);
  QByteArray CommonCommand(
      const char& id,
      const uint32_t& arg1, const uint32_t& arg2, const uint32_t& arg3);
private:
  bool Parse(
      const QByteArray &buffer, Message &parsed, int &from, int &to);
  static QByteArray ParseBuffer(
      const QByteArray &buffer, int &from, int &to);
  const QByteArray kHeadSegment;
//  std::atomic<CartSendCommandType> current_command_type_;
  std::mutex cart_steps_buffer_mutex_;
  std::shared_ptr<std::list<CartStep>> cart_steps_buffer_;
  std::list<CartStep> cart_steps_;
//  std::atomic_bool ended_;
  int current_position_;
  int step_length_ = 1;
  int distance_ = 500;
  bool first_half_ = true;
  std::function<std::unique_ptr<MeasureBasic>()> get_current_measure_;
  std::atomic<CartSendCommandType> cart_send_command_;
};

#endif // CART_DRIVER_H
