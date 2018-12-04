#include "mini_rte_cart.h"
#include <QDebug>

MiniRTECart::MiniRTECart()
{

}

bool MiniRTECart::Start() {
  stage_.store(RTEStageType::i037);
  return CartDriver::Start();
}


void MiniRTECart::StartCart(
    const uint32_t &distance, const uint32_t &step_length) {
  EnqueueCommand([this, distance, step_length](){
    return
        SendMessage(
            CommonCommand(char(0x0C), distance, step_length, uint32_t(1)));
  });
}

RTEStageType MiniRTECart::RTEStage() {
  return stage_.load();
}

void MiniRTECart::FinishedHeating() {
  CartDriver::Start();
}

void MiniRTECart::OnStep(const int& position) {
  auto stage = stage_.load();
  qDebug() << position;
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
