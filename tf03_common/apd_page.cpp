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

APDPage::APDPage()
{

}

APDPage::~APDPage() {
}

void APDPage::SetAPDDisplayLabel(QLabel *label) {
  apd_label_ = label;
}

void APDPage::SetTemperatureDisplayLabel(QLabel *label) {
  temp_label_ = label;
}

void APDPage::SetProgressBar(QProgressBar *progress) {
  progress_bar_ = progress;
}

void APDPage::SetAPDFromLineEdit(QLineEdit* edit) {
  apd_from_edit_ = edit;
}

void APDPage::SetAPDToLineEdit(QLineEdit* edit) {
  apd_to_edit_ = edit;
}

void APDPage::SetThresholdLineEdit(QLineEdit* edit) {
  threshold_edit_ = edit;
}

void APDPage::SetStartPushButton(QPushButton *button) {
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
  GetPlot().setTitle("Raw Distance (m)");
  status_label_->clear();
  return true;
}

void APDPage::IncomingMeasure(const MeasureDevel &measure) {
  GetPlot().AddPoint(measure.raw_dist1 / 100.f, measure.id);
  if (apd_label_) {
    apd_label_->setText(QString::number(measure.apd));
  }
  if (temp_label_) {
    temp_label_->setText(QString::number(measure.Celsius()));
  }
}

void APDPage::Update() {
  if (!ongoing_) {
    return;
  }

  if (apd_cmd_ > apd_to_) {
    OnStop();
    QMessageBox::warning(
        start_button_,
        "APD Experiment Failed",
        "APD Experiment ended. No crashed APD found.",
        QMessageBox::Abort);
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
//  driver_->SetAdaptiveAPD(true);
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
  for (auto& i = std::next(values.begin(), 1); i != values.end(); ++i) {
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
      return a.raw_dist1 > b.raw_dist1;
    }
  );
  int total = 0;
  // auto begin = dists->begin();
  auto begin = std::next(dists.begin(), kMaxToIgnore);
  auto end = std::prev(dists.end(), kMinToIgnore);
  for (auto i = begin; i != end; ++i) {
    total += i->raw_dist1;
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
    dist_vec.push_back(i.raw_dist1 * 1.0f / std_dist);
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
  auto button = QMessageBox::information(
      start_button_, "Experiment Results",
      "Experiment ended. \n"
      "Found APD crashed voltage: " + QString::number(apd_crash_) + " (V);\n"
      "temperature: " + QString::number(measure.Celsius(), 'f', 2) + " (C);\n"
      "result APD: " + QString::number(apd_result) + " (V).\n"
      "Write results to device?", QMessageBox::Yes, QMessageBox::No);
  if (button == QMessageBox::Yes) {
    driver_->SetAPD(apd_result);
  }
  start_button_->setText(kStartButtonStart);
}

void APDPage::ProceedExperiment() {
  apd_cmd_ += apd_step_;
  driver_->SetAPD(apd_cmd_);
  status_label_->setText("Setting APD: " + QString::number(apd_cmd_));
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
