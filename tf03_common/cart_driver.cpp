#include "cart_driver.h"
#include "static_unique_ptr_cast.h"
#include "utils.h"

CartDriver::CartDriver()
  : kHeadSegment(
      QByteArray(1, 0x50) +
      QByteArray(1, 0x4F) +
      QByteArray(1, 0x53) +
      QByteArray(1, 0x49))
{
  RegisterMeasureCallback([](){
      return std::unique_ptr<MeasureBasic>(new MeasureBasic);});
}

CartDriver::~CartDriver() {}

void CartDriver::StartCart(
    const uint32_t &distance, const uint32_t &step_length) {
  EnqueueCommand([this, distance, step_length](){
    return
        SendMessage(
            CommonCommand(char(0x01), distance, step_length, uint32_t(1)));
  });
}

void CartDriver::StartCartSingle(
    const uint32_t &distance, const uint32_t &step_length) {
  EnqueueCommand([this, distance, step_length](){
    return
        SendMessage(
            CommonCommand(char(0x0C), distance, step_length, uint32_t(1)));
  });
}

void CartDriver::GoOn() {
  EnqueueCommand([this](){
    return SendMessage(CommonCommand(char(0x04), 0, 0, 0, 1));
  });
}

void CartDriver::SetDistance(const int &distance) {
  distance_ = distance;
}

CartSendCommandType CartDriver::CurrentStage() {
  return cart_send_command_.load();
}

std::vector<int> CartDriver::DriverBaudRates() {
  return std::vector<int>(1, 460800);
}

bool CartDriver::Start() {
  if (!get_current_measure_) {
    return false;
  }
  cart_steps_buffer_mutex_.lock();
  cart_steps_buffer_.reset(new std::list<CartStep>);
  cart_steps_buffer_mutex_.unlock();
  cart_steps_.clear();
  current_position_ = 0;
  first_half_ = true;
  cart_send_command_.store(CartSendCommandType::step);
  StartCart(distance_, step_length_);
  return true;
}

std::shared_ptr<std::list<CartStep>> CartDriver::GetStepBuffer() {
  cart_steps_buffer_mutex_.lock();
  auto ptr = cart_steps_buffer_;
  cart_steps_buffer_.reset(new std::list<CartStep>);
  cart_steps_buffer_mutex_.unlock();
  return ptr;
}

std::list<CartStep> CartDriver::GetFullSteps() {
  return std::move(cart_steps_);
}

void CartDriver::SetStepLength(const int &len) {
  step_length_ = len;
}

int CartDriver::GetStepLength() {
  return step_length_;
}

void CartDriver::RegisterMeasureCallback(
    std::function<std::unique_ptr<MeasureBasic> ()> func) {
  get_current_measure_ = func;
}

void CartDriver::LoadAllParsers(std::vector<ReceiveParser> &parsers) {
  DriverBase::LoadAllParsers(parsers);
  parsers.push_back(
      std::bind(
          &CartDriver::Parse,
          this,
          std::placeholders::_1,
          std::placeholders::_2,
          std::placeholders::_3,
          std::placeholders::_4));
}

bool CartDriver::Parse(
    const QByteArray &buffer, Message &parsed, int &from, int &to) {
  auto msg = ParseBuffer(buffer, from, to);
  if (msg.isEmpty()) {
    return false;
  }
  switch (msg[4]) {
  case 0x11: // Step forward
  {
    if (first_half_) {
      current_position_ += step_length_;
    } else {
      current_position_ -= step_length_;
    }
    CartStep step_buffer, step;
    step_buffer.first_half = step.first_half = first_half_;
    step_buffer.position = step.position = current_position_;
    step.measure = get_current_measure_();
    if (step.measure) {
      step_buffer.measure =
          dynamic_unique_ptr_cast<MeasureBasic>(step.measure->Clone());
    }
    cart_steps_buffer_mutex_.lock();
    if (cart_steps_buffer_) {
      cart_steps_buffer_->emplace_back(std::move(step_buffer));
    }
    cart_steps_buffer_mutex_.unlock();
    cart_steps_.emplace_back(std::move(step));
    OnStep(current_position_);
  } break;
  case 0x12: // Reached end point
    first_half_ = false;
    OnEndPoint();
//    cart_send_command_.store(CartSendCommandType::end);
    break;
  case 0x13: // Reached start point
    OnStartPoint();
//    cart_send_command_.store(CartSendCommandType::start);
    break;
  default:
    break;
  }
  return true;
}

QByteArray CartDriver::ParseBuffer(
    const QByteArray &buffer, int &from, int &to) {
  QByteArray result;
  for (int i = 0; i < buffer.size(); ++i) {
    if (buffer[i] == 0x50) {
      auto ii = i + 1;
      if (ii < buffer.size()) {
        if (buffer[ii] == 0x4F) {
          auto iii = ii + 1;
          if (iii < buffer.size()) {
            if (buffer[iii] == 0x53) {
              auto iiii = iii + 1;
              if (iiii < buffer.size()) {
                if (buffer[iiii] == 0x49) {
                  auto end = iiii + 1;
                  if (end < buffer.size()) {
                    from = i;
                    to = end;
                    result.reserve(to - from + 1);
                    for (int j = from; j <= to; ++j) {
                      result.push_back(buffer[j]);
                    }
                    break;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return result;
}

QByteArray CartDriver::CommonCommand(
    const char& id,
    const uint32_t &arg1, const uint32_t &arg2,
    const uint32_t &arg3, const uint32_t& arg4) {
  return
      kHeadSegment + to_bytes(id) +
      to_bytes(arg1) + to_bytes(arg2) + to_bytes(arg3) + to_bytes(arg4);
}

QByteArray CartDriver::CommonCommand(
    const char& id,
    const uint32_t &arg1, const uint32_t &arg2, const uint32_t &arg3) {
  return
      kHeadSegment + to_bytes(id) +
      to_bytes(arg1) + to_bytes(arg2) + to_bytes(arg3) + to_bytes(uint32_t(1));
}

void CartDriver::OnStep(const int& current_position) {
}

void CartDriver::OnEndPoint() {}

void CartDriver::OnStartPoint() {}
