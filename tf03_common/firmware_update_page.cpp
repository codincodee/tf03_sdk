#include "firmware_update_page.h"
#include "firmware_module.h"
#include <QGridLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>

FirmwareUpdatePage::FirmwareUpdatePage()
{

}

void FirmwareUpdatePage::SetCore(
    std::shared_ptr<UpgradeFirmwareWidgets> core) {
  core_ = core;
}

void FirmwareUpdatePage::SetWidgetsLayout(QGridLayout *layout) {
  widgets_layout_ = layout;
}

bool FirmwareUpdatePage::Initialize() {
  if (!widgets_layout_) {
    return false;
  }
  core_.reset(new UpgradeFirmwareWidgets);
  core_->driver = GetDriver();
  core_->echo_handler = EchoHandler();
  core_->SetupUI();
  core_->before_upgrade_instructions_shown = true;
  widgets_layout_->addWidget(core_->browse, 0, 0);
  widgets_layout_->addWidget(core_->button, 0, 1);
  widgets_layout_->addWidget(core_->progress, 0, 2);
  widgets_layout_->addWidget(core_->status, 0, 3);
  return true;
}

void FirmwareUpdatePage::Update() {
  core_->Update();
}
