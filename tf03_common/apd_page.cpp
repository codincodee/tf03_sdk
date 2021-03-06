#include "apd_page.h"
#include "distance_over_time_chart.h"
#include <QtCharts/QChartView>
#include <QLayout>
#include "parsed.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include "utils.h"
#include <QLineEdit>
#include <QElapsedTimer>
#include "command_echo_handler.h"
#include "driver.h"
#include <set>
#include <QMessageBox>
#include "page_base.h"
#include <QDir>
#include <QTime>

APDPage::APDPage()
{

}

APDPage::~APDPage() {
}

void APDPage::SetAPDDisplayLabel(QLabel *label) {
  if (use_page_base_specs_) {
    PageBase::SetWidgetFontLargeBold(label);
  }
  apd_label_ = label;
}

void APDPage::SetTemperatureDisplayLabel(QLabel *label) {
  if (use_page_base_specs_) {
    PageBase::SetWidgetFontLargeBold(label);
  }
  temp_label_ = label;
}

void APDPage::SetProgressBar(QProgressBar *progress) {
  if (use_page_base_specs_) {
    PageBase::SetWidgetFontCommon(progress);
  }
  progress_bar_ = progress;
}

void APDPage::SetAPDFromLineEdit(QLineEdit* edit) {
  if (use_page_base_specs_) {
    PageBase::SetWidgetFontCommon(edit);
  }
  apd_from_edit_ = edit;
}

void APDPage::SetAPDToLineEdit(QLineEdit* edit) {
  if (use_page_base_specs_) {
    PageBase::SetWidgetFontCommon(edit);
  }
  apd_to_edit_ = edit;
}

void APDPage::SetThresholdLineEdit(QLineEdit* edit) {
  if (use_page_base_specs_) {
    PageBase::SetWidgetFontCommon(edit);
  }
  threshold_edit_ = edit;
}

void APDPage::SetStartPushButton(QPushButton *button) {
  if (use_page_base_specs_) {
    PageBase::SetWidgetFontCommon(button);
  }
  start_button_ = button;
}

void APDPage::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void APDPage::SetCmdEchoHandler(std::shared_ptr<CommandEchoHandler> echoes) {
  echoes_ = echoes;
}

void APDPage::SetStatusLabel(QLabel *label) {
  status_label_ = label;
}

bool APDPage::Initialize() {
  if (!start_button_) {
    return false;
  }
  if (!progress_bar_) {
    return false;
  }
  if (!apd_from_edit_) {
    return false;
  }
  if (!apd_to_edit_) {
    return false;
  }
  if (!threshold_edit_) {
    return false;
  }
  if (!driver_) {
    return false;
  }
  if (!status_label_) {
    return false;
  }
  if (!PlotBase::Initialize()) {
    return false;
  }
  connect(start_button_, SIGNAL(clicked()), this, SLOT(OnStartButtonClicked()));
  start_button_->setText(kStartButtonStart);
  progress_bar_->setValue(0);
  SetLineEditIntValidity(apd_from_edit_, 0, 1000);
  SetLineEditIntValidity(apd_to_edit_, 0, 1000);
  SetLineEditIntValidity(threshold_edit_, 0, 1000);
  apd_from_edit_->setText("140");
  apd_to_edit_->setText("180");
  threshold_edit_->setText("20");
  ongoing_ = false;
#ifdef APD_EXPERIMENT_USE_RAWDIST2_DEPRECATED
  GetPlot().setTitle("Raw Distance 2 (m)");
#else
  GetPlot().setTitle("Raw Distance (m)");
#endif
  if (use_page_base_specs_) {
    GetPlot().setFont(PageBase::GetCommonFont());
    GetPlot().setTitleFont(PageBase::GetCommonFont());
#ifdef APD_EXPERIMENT_USE_RAWDIST2_DEPRECATED
    GetPlot().setTitle("Raw Dist 2 (m)");
#else
    GetPlot().setTitle("原始距离 (m)");
#endif
    GetPlot().SetLabelFont(PageBase::GetSmallFont());
  }
  status_label_->clear();
  return true;
}

void APDPage::IncomingMeasure(const MeasureDevel &measure) {
  last_measure_ = measure;
  last_measure_.stamp.restart();
#ifdef APD_EXPERIMENT_USE_RAWDIST2_DEPRECATED
  GetPlot().AddPoint(measure.e / 100.f, measure.id);
#else
  GetPlot().AddPoint(measure.raw_dist1 / 100.f, measure.id);
#endif
  if (apd_label_) {
    apd_label_->setText(QString::number(measure.apd));
  }
  if (temp_label_) {
    temp_label_->setText(QString::number(measure.Celsius(), 'f', 1));
  }
}

void APDPage::Update() {
  if (!ongoing_) {
    DetectWriteAPDResultEcho();
    return;
  }

  if (apd_cmd_ > apd_to_) {
    OnStop();
    QMessageBox box(start_button_);
    if (use_page_base_specs_) {
      box.setFont(PageBase::GetCommonFont());
    }
    box.setWindowTitle("APD实验失败");
    box.setText("APD实验失败. 找不到APD雪崩值.");
    box.addButton(QMessageBox::Abort);
    box.setButtonText(QMessageBox::Abort, "放弃");
    return;
  }

  bool echoed = echoes_->IsCommandEchoed(kSetAPDCmdID);
  bool success = false;
  if (echoed) {
    success = echoes_->IsCommandSucceeded(kSetAPDCmdID);
  }


  if (timeout_) {
    if (timeout_->elapsed() > 1000) {
      if (!echoed) {
        driver_->SetAPD(apd_cmd_);
        timeout_->restart();
        return;
      }
      if (!success) {
        driver_->SetAPD(apd_cmd_);
        timeout_->restart();
        return;
      }
    }
  }

  // echoed & succeeded
  timeout_.reset();

  auto measures_echoed = echoes_->IsMeasureDevelStreamEchoed();
  MeasureDevelStream measures;
  if (measures_echoed) {
    measures = echoes_->GetMeasureDevelStream();
  }

  if (phase_ == Phase::wait_for_echo) {
    if (measures_echoed) {
      phase_ = Phase::wait_for_stable;
    }
    return;
  }

  if (phase_ == Phase::wait_for_stable) {
    if (measures_echoed) {
      if (IsSampleStable(measures)) {
        phase_ = Phase::wait_for_sample;
      }
      return;
    }
  }

  if (phase_ == Phase::wait_for_sample) {
    if (measures_echoed) {
      if (apd_cmd_ == apd_from_) {
        std_dist_ = CalculateStandardDistance(measures);
        ProceedExperiment();
      } else {
        if (IsCrashed(measures, std_dist_, threshold_)) {
          Q_ASSERT(!measures.stream.empty());
          HandleCrashed(*measures.stream.rbegin(), apd_cmd_);
        } else {
          ProceedExperiment();
        }
      }
    }
    return;
  }
}

void APDPage::OnStartButtonClicked() {
  if (start_button_->text() == kStartButtonStart) {
    if (!OnStart()) {
      return;
    }
  } else if (start_button_->text() == kStartButtonStop) {
    OnStop();
  }
}

bool APDPage::OnStart() {
  if (least_mcu_temperature_ > 0) {
    if (last_measure_.stamp.elapsed() > 1000) {
      return false;
    }
    if (last_measure_.Celsius() < least_mcu_temperature_) {
      QMessageBox box(start_button_);
      if (use_page_base_specs_) {
        box.setFont(PageBase::GetCommonFont());
      }
      box.setWindowTitle("错误");
      box.setText(
          "MCU温度过低 (低于" + QString::number(least_mcu_temperature_) + "°C)，"
          "无法启动实验");
      box.addButton(QMessageBox::Abort);
      box.setButtonText(QMessageBox::Abort, "放弃");
      box.exec();
      return false;
    }
  }
  bool ok;
  apd_from_ = apd_from_edit_->text().toInt(&ok);
  if (!ok) return false;
  apd_to_ = apd_to_edit_->text().toInt(&ok);
  if (!ok) return false;
  threshold_ = threshold_edit_->text().toInt(&ok);
  if (!ok) return false;

  driver_->SetAPDClosedLoop(true);
  driver_->SetAutoGainAdjust(false);
  driver_->SetAdaptiveAPD(false);
  driver_->APDExperimentOn();
  apd_cmd_ = apd_from_;
  timeout_.reset(new QElapsedTimer);
  timeout_->restart();
  driver_->SetAPD(apd_cmd_);
  phase_ = Phase::wait_for_echo;
  ongoing_ = true;
  start_button_->setText(kStartButtonStop);
  return true;
}

void APDPage::OnStop() {
//  driver_->SetAutoGainAdjust(true);
  driver_->SetAdaptiveAPD(true);
  ongoing_ = false;
  driver_->APDExperimentOff();
  start_button_->setText(kStartButtonStart);
  status_label_->clear();
  progress_bar_->setValue(0);
}

bool APDPage::IsSampleStable(MeasureDevelStream stream) {
  std::set<int> values;
  for (auto& measure : stream.stream) {
    values.insert(measure.apd);
  }
  if (values.empty()) {
    return false;
  }
  if (values.size() > 4) {
    return false;
  }
  int value = *values.begin();
  for (auto i = std::next(values.begin(), 1); i != values.end(); ++i) {
    if (value != (*i - 1)) {
      return false;
    } else {
      value = *i;
    }
  }
  return true;
}

int APDPage::CalculateStandardDistance(MeasureDevelStream stream) {
  auto& dists = stream.stream;
  if (dists.size() <= (kMaxToIgnore + kMinToIgnore)) {
    return -1;
  }
  dists.sort(
    [](const MeasureDevel& a,
       const MeasureDevel& b){
#ifdef APD_EXPERIMENT_USE_RAWDIST2_DEPRECATED
      return a.raw_dist2 > b.raw_dist2;
#else
      return a.raw_dist1 > b.raw_dist1;
#endif
    }
  );
  int total = 0;
  // auto begin = dists->begin();
  auto begin = std::next(dists.begin(), kMaxToIgnore);
  auto end = std::prev(dists.end(), kMinToIgnore);
  for (auto i = begin; i != end; ++i) {
#ifdef APD_EXPERIMENT_USE_RAWDIST2_DEPRECATED
    total += i->raw_dist2;
#else
    total += i->raw_dist1;
#endif
  }
  return total * 1.0f / (dists.size() - kMaxToIgnore - kMinToIgnore);
}

bool APDPage::IsCrashed(
    const MeasureDevelStream &stream, const int &std_dist, const int& threshold) {
  // auto& dists = stream.stream;
  std::list<MeasureDevel> dists;
  int cnt = 0;
  for (auto& dist : stream.stream) {
    if (cnt > 50) {
      break;
    }
    dists.push_back(dist);
  }
  if (dists.size() <= (kMaxToIgnore + kMinToIgnore)) {
    return false;
  }
  std::vector<float> dist_vec;
  dist_vec.reserve(dists.size());
  for (auto& i : dists) {
#ifdef APD_EXPERIMENT_USE_RAWDIST2_DEPRECATED
    dist_vec.push_back(i.raw_dist2 * 1.0f / std_dist);
#else
    dist_vec.push_back(i.raw_dist1 * 1.0f / std_dist);
#endif
  }
  std::sort(dist_vec.begin(), dist_vec.end());
  auto begin = std::next(dist_vec.begin(), kMaxToIgnore);
  auto end = std::prev(dist_vec.end(), kMinToIgnore);

  int num = dist_vec.size() - kMaxToIgnore - kMinToIgnore;
  float mean = 0;
  for (auto i = begin; i != end; ++i) {
    mean += *i;
  }
  mean = mean / num;

  float stddev = 0;
  float diff;
  for (auto i = begin; i != end; ++i) {
    diff = *i - mean;
    stddev += diff * diff;
  }
  auto result = (std::sqrt(stddev / num) * 120);
//  qDebug() << result;
  return result > threshold;
}

void APDPage::HandleCrashed(
    const MeasureDevel &measure, const int &apd_crash_) {
  auto apd_result = CalculateResultAPD(apd_crash_, measure.Celsius());
  ongoing_ = false;
  OnStop();
  QMessageBox box(start_button_);
  if (use_page_base_specs_) {
    box.setFont(PageBase::GetCommonFont());
  }
  box.setWindowTitle("实验结果");
  box.setText(
      "实验结束. \n"
      "找到APD雪崩值: " + QString::number(apd_crash_) + " (V);\n"
      "温度: " + QString::number(measure.Celsius(), 'f', 2) + " (C);\n"
      "APD目标值: " + QString::number(apd_result) + " (V).\n"
      "将APD目标值写入结果?");
  box.addButton(QMessageBox::Yes);
  box.addButton(QMessageBox::No);
  box.setButtonText(QMessageBox::Yes, "是");
  box.setButtonText(QMessageBox::No, "否");
  auto button = box.exec();
  if (button == QMessageBox::Yes) {
    HandleLogging(apd_crash_, apd_result, measure.Celsius());
    driver_->SetAPD(apd_result);
    if (echo_write_apd_result_) {
      write_apd_result_timeout_.reset(new QElapsedTimer);
      write_apd_result_timeout_->restart();
    }
    if (save_settings_after_setting_result_apd_) {
      driver_->SaveSettingsToFlash();
    }
  }
  start_button_->setText(kStartButtonStart);
}

void APDPage::ProceedExperiment() {
  apd_cmd_ += apd_step_;
  driver_->SetAPD(apd_cmd_);
  status_label_->setText("正在设置APD: " + QString::number(apd_cmd_));
  if (!timeout_) {
    timeout_.reset(new QElapsedTimer);
  }
  timeout_->restart();
  progress_bar_->setValue(
      100.0f * (apd_cmd_ - apd_from_) / (apd_to_ - apd_from_));
  phase_ = Phase::wait_for_echo;
}

int APDPage::CalculateResultAPD(const int &apd_crash, const float &temp) {
  return (apd_crash - (temp - 30) * 0.9) * 0.9;
}

void APDPage::UsePageBaseSpecs(const bool &use) {
  use_page_base_specs_ = use;
}

void APDPage::SetLeastStartTemperature(const int &temp) {
  least_mcu_temperature_ = temp;
}

void APDPage::SetSaveSettingsWhenWriteResult(const bool &save) {
  save_settings_after_setting_result_apd_ = save;
}

void APDPage::SetLogPath(const QString &path) {
  log_dir_path_ = path;
}

void APDPage::EchoWriteAPDResult() {
  echo_write_apd_result_ = true;
}

void APDPage::HandleLogging(
    const int &apd_crash, const int &apd_result, const float &temp) {
  if (log_dir_path_.isEmpty()) {
    return;
  }
  QDir dir(log_dir_path_);
  if (!dir.exists()) {
    return;
  }
  QFile file(
      log_dir_path_ + "/" +
      QDateTime::currentDateTime().toString("yyyy_MM_dd") + ".txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    return;
  }
  QTextStream stream(&file);
  if (file.size() == 0) {
    stream << "Time (h:m:s) | APD Crash | APD Write | Temperature\n";
  }
  stream << QDateTime::currentDateTime().toString("hh:mm:ss") + " " << apd_crash << " " << apd_result << " " << temp << "\n";
  file.close();
}

void APDPage::DetectWriteAPDResultEcho() {
  if (!echo_write_apd_result_) {
    return;
  }

  if (!write_apd_result_timeout_) {
    return;
  }
  if (write_apd_result_timeout_->elapsed() > 1000) {
    status_label_->setText("结果写入失败");
    write_apd_result_timeout_.reset();
    return;
  }
  if (echoes_->IsCommandEchoed(0x40)) {
    if (echoes_->IsCommandSucceeded(0x40)) {
      status_label_->setText("成功写入结果");
    } else {
      status_label_->setText("结果写入失败");
    }
    write_apd_result_timeout_.reset();
  }
}
