#include "rte_cart_server.h"
#include <QDebug>
#include <QThread>
#include "static_unique_ptr_cast.h"

RTECartServer::RTECartServer()
{

}

void RTECartServer::SetDriver(std::shared_ptr<MiniRTECart> driver) {
  driver_ = driver;
}

void RTECartServer::SetMiniDriver(std::shared_ptr<TFMiniDriver> driver) {
  sensor_ = driver;
}

std::shared_ptr<MiniRTECart> RTECartServer::Driver() {
  return driver_;
}

bool RTECartServer::Start() {
  if (!driver_) {
    return false;
  }
  last_stage_ = RTEStageType::i037;
  if (sensor_) {
    sensor_->SetMetricUnit(true);
    sensor_->SetOutputFormat(TFMiniOutputFormat::b_29);
    sensor_->SetTimer(false);
    QThread::msleep(1000);
  }
  return driver_->Start();
}

void RTECartServer::Spin() {
  if (!driver_) {
    return;
  }
  auto stage = driver_->RTEStage();
  if (stage == RTEStageType::i037_burn) {
    OnI037Burn();
  } else if (stage == RTEStageType::i037_back) {
    OnI037Back();
  } else if (stage == RTEStageType::i037_temp_burn) {
    OnI037TempBurn();
  } else if (stage == RTEStageType::heating) {
    OnHeating();
  } else if (stage == RTEStageType::stop) {
    OnStop();
  } else if (stage == RTEStageType::auto_i) {
    OnAutoI();
  } else if (stage == RTEStageType::i037_temp) {
    OnI037Temp();
  }
  last_stage_ = stage;
}

bool RTECartServer::OnInitialized() {
  if (!driver_) {
    return false;
  }
  return true;
}

void RTECartServer::OnI037Burn() {
  if (last_stage_ == RTEStageType::i037_burn) {
    return;
  }
  if (driver_) {
    auto measures = driver_->GetMeasures();
    qDebug() << "I037 Burn";
    PrintMeasures(measures);
  }
  emit I037Burn();
}

void RTECartServer::OnI037TempBurn() {
  if (last_stage_ == RTEStageType::i037_temp_burn) {
    return;
  }
  if (driver_) {
    auto measures = driver_->GetMeasures();
    qDebug() << "I037 Temp Burn";
    PrintMeasures(measures);
  }
  if (sensor_) {
    sensor_->SetIntTimeMode(TFMiniIntTimeMode::typical);
    sensor_->SetTimer(true);
  }
  emit I037TempBurn();
}

void RTECartServer::OnHeating() {
  if (last_stage_ == RTEStageType::heating) {
    return;
  }
  emit Heating();
}

void RTECartServer::OnStop() {
  if (last_stage_ == RTEStageType::stop) {
    return;
  }
  if (driver_) {
    auto measures = driver_->GetMeasures();
    qDebug() << "Auto Int";
    PrintMeasures(measures);
  }
  emit Finished();
}

void RTECartServer::OnAutoI() {
  if (last_stage_ == RTEStageType::auto_i) {
    return;
  }
  emit AutoI();
}

void RTECartServer::OnI037Back() {
  if (last_stage_ == RTEStageType::i037_back) {
    return;
  }
  emit I037Back();
}

void RTECartServer::OnI037Temp() {
  if (last_stage_ == RTEStageType::i037_temp) {
    return;
  }
  emit I037Temp();
}

void RTECartServer::PrintMeasures(
    std::shared_ptr<std::list<Message> > measures) {
  if (!measures) {
    return;
  }
  for (auto& measure : *measures) {
    std::unique_ptr<MiniMeasure29B> b29 =
          dynamic_unique_ptr_cast<MiniMeasure29B>(std::move(measure.data));
    if (!b29) {
      qDebug() << "One Corrupted Measure";
      continue;
    }
    qDebug() << b29->Manifest();
  }
}
