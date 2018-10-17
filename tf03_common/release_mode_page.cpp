#include "release_mode_page.h"
#include "command_echo_widgets.h"
#include <QLabel>
#include <QLayout>
#include <QPushButton>

ReleaseModePage::ReleaseModePage()
{

}

void ReleaseModePage::SetWidgetsGridLayout(QGridLayout *layout) {
  layout_ = layout;
}

bool ReleaseModePage::Initialize() {
  if (!layout_) {
    return false;
  }
  core_.reset(new SetReleaseModeWidgets);
  core_->driver = GetDriver();
  core_->echo_handler = EchoHandler();
  core_->SetupUI();

  layout_->addWidget(core_->option, 0, 0);
  layout_->addWidget(core_->button, 0, 1);
  layout_->addWidget(core_->status, 0, 2);
  return true;
}

void ReleaseModePage::Update() {
  core_->Update();
}
