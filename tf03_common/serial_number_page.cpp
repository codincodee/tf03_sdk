#include "serial_number_page.h"
#include "command_echo_widgets.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

SerialNumberPage::SerialNumberPage()
{

}

void SerialNumberPage::SetWidgetsLayout(QGridLayout *layout) {
  layout_ = layout;
}

bool SerialNumberPage::Initialize() {
  if (!layout_) {
    return false;
  }
  write_.reset(new WriteSerialNumberPlusWidgets);
  write_->driver = GetDriver();
  write_->echo_handler = EchoHandler();
  write_->SetupUI();
  write_->DisableInputMask();

  read_.reset(new SerialNumberWidgets);
  read_->driver = GetDriver();
  read_->echo_handler = EchoHandler();
  read_->SetupUI();

  layout_->addWidget(write_->item, 0, 0);
  layout_->addWidget(write_->option, 0, 1);
  layout_->addWidget(write_->button, 0, 2);
  layout_->addWidget(write_->status, 0, 3);

  layout_->addWidget(read_->item, 1, 0);
  layout_->addWidget(read_->option, 1, 1);
  layout_->addWidget(read_->button, 1, 2);
  layout_->addWidget(read_->status, 1, 3);
  return true;
}

void SerialNumberPage::Update() {
  write_->Update();
  read_->Update();
}
