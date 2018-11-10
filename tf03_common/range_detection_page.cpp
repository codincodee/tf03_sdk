#include "range_detection_page.h"
#include "command_echo_widgets.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include "plot_base.h"
#include "distance_over_time_chart_with_threshold.h"
#include "driver.h"

RangeDetectionPage::RangeDetectionPage()
{

}

void RangeDetectionPage::SetWidgetsLayout(QGridLayout *layout) {
  layout_ = layout;
}

void RangeDetectionPage::SetTipsLabel(QLabel *label) {
  label->setText(
      "测试环境：14m，正对黑色纸板，加18%透过率衰减片，\n"
      "测试标准：有效距离探测概率>80%\n"
      "若点击检测按钮后显示未响应，请重新点击一次检测或下电重启");
}

void RangeDetectionPage::SetPlotLayout(QLayout *layout) {
  if (!layout) {
    return;
  }
  plot_.reset(new PlotBase);
  plot_->SetLayout(layout);
  auto chart = new DistanceOverTimeChartWithThreshold();
  chart->SetTimeSpan(400);
  chart->SetHigh(25.0f);
  chart->SetLow(0.0f);
  chart->SetThreshold(kThreshold);
  plot_->SetChart(chart);
  chart->setFont(PageBase::GetCommonFont());
  chart->setTitleFont(PageBase::GetCommonFont());
#ifdef APD_EXPERIMENT_USE_RAWDIST2
  chart->setTitle("Raw Dist 2 (m)");
#else
  chart->setTitle("距离 (m)");
#endif
  chart->SetLabelFont(PageBase::GetSmallFont());
}

bool RangeDetectionPage::Initialize() {
  if (!layout_) {
    return false;
  }
  if (plot_) {
    if (!plot_->Initialize()) {
      return false;
    }
  }
  core_.reset(new RangeValidityWidgets);
  GetDriver()->SetRangeDetectTaskThreshold(std::round(kThreshold * 100));
  core_->driver = GetDriver();
  core_->echo_handler = EchoHandler();
  core_->SetupUI();
  layout_->addWidget(core_->button, 0, 0);
  layout_->addWidget(core_->indicator, 0, 1);
  layout_->addWidget(core_->label, 0, 2);
  layout_->addWidget(core_->status, 0, 3);
  return true;
}

void RangeDetectionPage::Update() {
  core_->Update();
}

//void RangeDetectionPage::OnMeasured(const MeasureBasic &measure) {
//  PageBase::OnMeasured(measure);
//}

void RangeDetectionPage::OnMeasured(const MeasureDevel &measure) {
  PageBase::OnMeasured(measure);
  if (plot_) {
    plot_->IncomingMeasureRawDist2(measure);
  }
}
