#include "rte_cart_server.h"
#include <QDebug>
#include <QThread>
#include "static_unique_ptr_cast.h"
#include "mini_generate_table_8.h"
#include "mini_table_burner.h"

RTECartServer::RTECartServer()
{

}

RTECartServer::~RTECartServer() {

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
    QThread::msleep(100);
    sensor_->SetTimer(true);
    QThread::msleep(100);
    sensor_->SetOutputFormat(TFMiniOutputFormat::b_29);
    QThread::msleep(100);
    sensor_->SetIntTimeMode(TFMiniIntTimeMode::cyclic);
    QThread::msleep(100);
    sensor_->SwitchOnMeasureStream(true);
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

std::shared_ptr<TFMiniDriver> RTECartServer::Sensor() {
  return sensor_;
}

bool RTECartServer::I037BurnCallback(std::list<MiniCartStep> steps) {
  return true;
}

bool RTECartServer::I037TempBurnCallback(std::list<MiniCartStep> steps) {
  return true;
}

bool RTECartServer::AutoIntCheckCallback(std::list<MiniCartStep> steps) {
  return true;
}

bool RTECartServer::OnInitialized() {
  if (!driver_) {
    return false;
  }
  if (!sensor_) {
    return false;
  }
  burner_.reset(new MiniTableBurner);
  burner_->SetDriver(sensor_);
  return true;
}

void RTECartServer::OnI037Burn() {
  if (last_stage_ == RTEStageType::i037_burn) {
    return;
  }
  if (driver_) {
//    auto steps = driver_->GetStepBuffer();
    auto steps = driver_->Steps();
    qDebug() << "I037 Burn";
    PrintSteps(steps);
    I037BurnCallback(steps);
  }
  emit I037Burn();
}

void RTECartServer::OnI037TempBurn() {
  if (last_stage_ == RTEStageType::i037_temp_burn) {
    return;
  }
  if (driver_) {
    auto steps = driver_->Steps();
    qDebug() << "I037 Temp Burn";
    PrintSteps(steps);
    I037TempBurnCallback(steps);
  }
  if (sensor_) {
    sensor_->SetIntTimeMode(TFMiniIntTimeMode::typical);
    QThread::msleep(100);
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
    auto steps = driver_->Steps();
    qDebug() << "Auto Int";
    PrintSteps(steps);
    AutoIntCheckCallback(steps);
  }
  emit Finished();
  sensor_->SwitchOnMeasureStream(false);
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

void RTECartServer::PrintSteps(
    const std::list<MiniCartStep>& steps) {
  for (auto& step : steps) {
    if (!step.measures) {
      continue;
    }
    qDebug() << "=== " << step.position << " ===";
    for (auto& measure : *step.measures) {
      auto m = measure.data->Clone();
      std::unique_ptr<MiniMeasure29B> b29 =
          dynamic_unique_ptr_cast<MiniMeasure29B>(std::move(m));
      if (!b29) {
        qDebug() << "One Corrupted Measure";
        continue;
      }
      qDebug() << b29->Manifest();
    }
  }
}

void RTECartServer::PrintSteps(
    std::shared_ptr<std::list<CartStep> > steps) {
  if (!steps) {
    return;
  }
  for (auto& step : *steps) {
    if (!step.measure) {
      continue;
    }
    auto measure = step.measure->Clone();
    std::unique_ptr<MiniMeasure29B> b29 =
          dynamic_unique_ptr_cast<MiniMeasure29B>(std::move(measure));
    if (!b29) {
      qDebug() << "One Corrupted Measure";
      continue;
    }
    qDebug() << step.position << b29->Manifest();
  }
}

void RTECartServer::HandleOnI037BurnFinished(
    std::shared_ptr<std::list<CartStep> > steps) {
  if (!sensor_) {
    return;
  }
  if (!steps) {
    return;
  }
  Int037Input dist, realdist, amp;
  for (auto& step : *steps) {
    if (!step.measure) {
      continue;
    }
    auto measure = step.measure->Clone();
    std::unique_ptr<MiniMeasure29B> b29 =
          dynamic_unique_ptr_cast<MiniMeasure29B>(std::move(measure));
    if (!b29) {
      qDebug() << "One Corrupted Measure";
      continue;
    }
    auto d = b29->ref_d;
    auto a = b29->amp;
    auto t = b29->temp;
    auto r = step.position;
    if (b29->inttime == 0) {
      r = MiniGenerateTable8::TempCompensateInt0(d, r, t);
      dist.i0.push_back(d);
      amp.i0.push_back(a);
      realdist.i0.push_back(r);
    } else if (b29->inttime == 3) {
      r = MiniGenerateTable8::TempCompensateInt3(d, r, t);
      dist.i3.push_back(d);
      amp.i3.push_back(a);
      realdist.i3.push_back(r);
    } else if (b29->inttime == 7) {
      r = MiniGenerateTable8::TempCompensateInt7(d, r, t);
      dist.i7.push_back(d);
      amp.i7.push_back(a);
      realdist.i7.push_back(r);
    } else {
      qDebug() << "One Unexpected Int-time";
    }
  }
  MiniGenerateTable8::TrimInput(dist);
  MiniGenerateTable8::TrimInput(amp);
  MiniGenerateTable8::TrimInput(realdist);
  std::vector<mxInt32> table_0, table_3, table_7;
  if (MiniGenerateTable8::GenerateWithoutException(
          dist, realdist, amp, table_0, table_3, table_7)) {
    // table_0, table_3, table_7
    auto tab_0 = MiniGenerateTable8::ConvertToDriverTable(table_0);
    auto tab_3 = MiniGenerateTable8::ConvertToDriverTable(table_3);
    auto tab_7 = MiniGenerateTable8::ConvertToDriverTable(table_7);
    if (burner_) {
      burner_->StartBurn(tab_0, tab_3, tab_7);
    }
  }
}
