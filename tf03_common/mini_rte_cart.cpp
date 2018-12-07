#include "mini_rte_cart.h"
#include <QDebug>

MiniRTECart::MiniRTECart()
{

}

bool MiniRTECart::Start() {
  stage_.store(RTEStageType::i037);
  current_037_ = I037Type::i0;
  return CartDriver::Start();
}


void MiniRTECart::StartCart(
    const uint32_t &distance, const uint32_t &step_length) {
  StartCartSingle(distance, step_length);
}

RTEStageType MiniRTECart::RTEStage() {
  return stage_.load();
}

void MiniRTECart::FinishedHeating() {
  current_037_ = I037Type::i0;
  CartDriver::Start();
}

void MiniRTECart::RegisterIntTimeTriggerCallback(
    std::function<void (int)> func) {
  trigger_inttime_measure_ = func;
}

void MiniRTECart::OnStep(const int& position) {
  auto stage = stage_.load();
  if (stage == RTEStageType::i037 || stage == RTEStageType::i037_temp) {
    auto current_037 = current_037_.load();
    if (current_037 == I037Type::i0) {
      current_037_ = I037Type::i3;
    } else if (current_037 == I037Type::i3) {
      current_037_ = I037Type::i7;
    } else if (current_037 == I037Type::i7) {
      current_037_ = I037Type::i0;
    }
    if (trigger_inttime_measure_) {
      trigger_inttime_measure_(int(current_037));
    }
  }
  if (position < GetStepLength()) {
    if (stage == RTEStageType::i037_back) {
      stage_ = RTEStageType::heating;
    } else if (stage == RTEStageType::auto_i) {
      stage_ = RTEStageType::stop;
    }
  }
  if (stage == RTEStageType::i037_burn) {
    stage_ = RTEStageType::i037_back;
  } else if (stage == RTEStageType::i037_temp_burn) {
    stage_ = RTEStageType::auto_i;
  }
}

void MiniRTECart::OnEndPoint() {
  auto stage = stage_.load();
  if (stage == RTEStageType::i037) {
    stage_ = RTEStageType::i037_burn;
  } else if (stage == RTEStageType::i037_temp) {
    stage_ = RTEStageType::i037_temp_burn;
  }
}

void MiniRTECart::OnStartPoint() {
  auto stage = stage_.load();
  if (stage == RTEStageType::heating) {
    stage_ = RTEStageType::i037_temp;
  }
}
