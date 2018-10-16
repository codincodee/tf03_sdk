#include "range_detection_page.h"
#include "command_echo_widgets.h"
#include <QLayout>
#include <QPushButton>
#include <QLabel>

RangeDetectionPage::RangeDetectionPage()
{

}

void RangeDetectionPage::SetWidgetsLayout(QGridLayout *layout) {
  layout_ = layout;
}

bool RangeDetectionPage::Initialize() {
  if (!layout_) {
    return false;
  }
  core_.reset(new RangeValidityWidgets);
  core_->driver = GetDriver();
  core_->echo_handler = EchoHandler();
  core_->SetupUI();
  layout_->addWidget(core_->button, 0, 0);
  layout_->addWidget(core_->label, 0, 1);
  layout_->addWidget(core_->status, 0, 2);
  return true;
}

void RangeDetectionPage::Update() {
  core_->Update();
}
