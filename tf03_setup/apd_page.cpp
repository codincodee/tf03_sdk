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

APDPage::APDPage()
{

}

APDPage::~APDPage() {
  if (main_chart_) {
    delete main_chart_;
    main_chart_ = nullptr;
  }

  if (main_chart_view_) {
    delete main_chart_view_;
    main_chart_view_ = nullptr;
  }
}

void APDPage::SetPlotLayout(QLayout *layout) {
  plot_layout_ = layout;
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
  main_chart_ = new DistanceOverTimeChart();
  main_chart_->SetCeiling(150.0f);
  main_chart_->SetFloor(5.0f);
  main_chart_view_ = new QtCharts::QChartView(main_chart_);
  if (plot_layout_) {
    plot_layout_->addWidget(main_chart_view_);
  }
  main_chart_->SetTimeSpan(400);
  connect(start_button_, SIGNAL(clicked()), this, SLOT(OnStartButtonClicked()));
  start_button_->setText(kStartButtonStart);
  progress_bar_->setValue(0);
  SetLineEditIntValidity(apd_from_edit_, 0, 1000);
  SetLineEditIntValidity(apd_to_edit_, 0, 1000);
  SetLineEditIntValidity(threshold_edit_, 0, 1000);
  apd_from_edit_->setText("150");
  apd_to_edit_->setText("180");
  threshold_edit_->setText("20");
  ongoing_ = false;
  return true;
}

void APDPage::IncomingMeasure(const MeasureDevel &measure) {
  if (main_chart_) {
    main_chart_->AddPoint(measure.dist / 100.f, measure.id);
  }
  if (apd_label_) {
    apd_label_->setText(QString::number(measure.apd));
  }
  if (temp_label_) {
    temp_label_->setText(QString::number(measure.temp));
  }
}

void APDPage::Update() {
  if (!ongoing_) {
    return;
  }

  if (apd_cmd_ > apd_to_) {
    ongoing_ = false;
    start_button_->setText(kStartButtonStop);
    OnStop();
    return;
  }

  bool echoed = echoes_->IsCommandEchoed(kSetAPDCmdID);
  bool success = false;
  if (echoed) {
    success = echoes_->IsCommandSucceeded(kSetAPDCmdID);
  }

  if (timeout_) {
    if (timeout_->elapsed() > 1000) {
      qDebug() << __LINE__;
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
  if (!timer_) {
    timer_.reset(new QElapsedTimer);
    timer_->restart();
  } else {
    if (timer_->elapsed() > 4000) {
      apd_cmd_ += 1;
      driver_->SetAPD(apd_cmd_);
      qDebug() << "Setting: " << apd_cmd_;
      timer_.reset();
      if (!timeout_) {
        timeout_.reset(new QElapsedTimer);
      }
      timeout_->restart();
    }
  }
}

void APDPage::OnStartButtonClicked() {
  if (start_button_->text() == kStartButtonStart) {
    bool ok;
    apd_from_ = apd_from_edit_->text().toInt(&ok);
    if (!ok) return;
    apd_to_ = apd_to_edit_->text().toInt(&ok);
    if (!ok) return;
    threshold_ = threshold_edit_->text().toInt(&ok);
    if (!ok) return;

    apd_cmd_ = apd_from_;
    OnStart();
    timeout_.reset(new QElapsedTimer);
    timeout_->restart();
    driver_->SetAPD(apd_cmd_);
    ongoing_ = true;
    start_button_->setText(kStartButtonStop);
  } else if (start_button_->text() == kStartButtonStop) {
    ongoing_ = false;
    OnStop();
    start_button_->setText(kStartButtonStart);
  }
}

void APDPage::OnStart() {
  driver_->SetAPDClosedLoop(true);
  driver_->SetAutoGainAdjust(false);
  driver_->SetAdaptiveAPD(false);
}

void APDPage::OnStop() {
  driver_->SetAutoGainAdjust(true);
  driver_->SetAdaptiveAPD(true);
}
