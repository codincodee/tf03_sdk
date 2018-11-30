#include "common_command_widget.h"
#include <QDebug>

CommonCommandWidget::CommonCommandWidget(QWidget* parent)
  : parent_(parent),CustomWidget(parent)
{
  item_label = new QLabel(parent);
  push_button = new QPushButton(parent);
  status_label = new QLabel(parent);

  item = item_label;
  button = push_button;
  status = status_label;

  push_button->setText(kButtonTextSet);

  connect(push_button, SIGNAL(clicked()), this, SLOT(OnPushButtonClicked()));
}

CommonCommandWidget::~CommonCommandWidget() {}

void CommonCommandWidget::OnPushButtonClicked() {
  OnButtonClicked();
}

void CommonCommandWidget::OnButtonClicked() {
  push_button->setDisabled(true);
  status_label->clear();
  timer.restart();
  OnButtonClickedCommon();
}

void CommonCommandWidget::CommonTimerCallback() {
  CustomWidget::CommonTimerCallback();
  Update();
}

void CommonCommandWidget::Update() {
  if (!ProceedUpdate()) {
    return;
  }
  OnUpdate();
}

void CommonCommandWidget::OnUpdate() {

}

bool CommonCommandWidget::ProceedUpdate() {
  if (push_button->isEnabled()) {
    return false;
  }
  if (timer.elapsed() > timeout) {
    return false;
  }
  return true;
}

void CommonCommandWidget::SetCommandEchoed(const bool &success) {
  if (success) {
    push_button->setDisabled(false);
  }
}

void CommonCommandWidget::OnButtonClickedCommon() {

}
