#ifndef CART_DRIVER_H
#define CART_DRIVER_H

#include "export.h"
#include "driver_base.h"

enum class CartSendCommandType {
  start, end, step
};

struct CartStep {
  int position = 0;
  std::unique_ptr<MeasureBasic> measure;
  bool first_half = true;
};

class CartDriver : public DriverBase
{
public:
  CartDriver();
  bool Start();
//  bool IsEnded();
  std::list<CartStep> GetFullSteps();
  std::shared_ptr<std::list<CartStep>> GetStepBuffer();
  void SetStepLength(const int& len);
  void RegisterMeasureCallback(
      std::function<std::unique_ptr<MeasureBasic>()> func);
  void StartCart(const uint32_t& distance, const uint32_t& step_length);
  void SetDistance(const int& distance);
protected:
  void LoadAllParsers(std::vector<ReceiveParser>& parsers) override;
private:
  bool Parse(
      const QByteArray &buffer, Message &parsed, int &from, int &to);
  static QByteArray ParseBuffer(
      const QByteArray &buffer, int &from, int &to);
  QByteArray CommonCommand(
      const char& id, const uint32_t& arg1, const uint32_t& arg2, const uint32_t& arg3);
  const QByteArray kHeadSegment;
//  std::atomic<CartSendCommandType> current_command_type_;
  std::mutex cart_steps_buffer_mutex_;
  std::shared_ptr<std::list<CartStep>> cart_steps_buffer_;
  std::list<CartStep> cart_steps_;
//  std::atomic_bool ended_;
  int current_position_;
  int step_length_ = 1;
  int distance_ = 0;
  bool first_half_ = true;
  std::function<std::unique_ptr<MeasureBasic>()> get_current_measure_;
};

#endif // CART_DRIVER_H
