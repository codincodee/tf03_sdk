#include "page_base.h"
#include <QLabel>

PageBase::PageBase()
{

}

PageBase::~PageBase() {}

void PageBase::SetCommandEchoHandler(
    std::shared_ptr<CommandEchoHandler> echo_handler) {
  echo_handler_ = echo_handler;
}

std::shared_ptr<CommandEchoHandler> PageBase::EchoHandler() {
  return echo_handler_;
}

void PageBase::Update() {
}

void PageBase::OnMeasured(const MeasureBasic &measure) {

}

void PageBase::OnMeasured(const MeasureDevel &measure) {

}

void PageBase::SetWidgetFontSmall(QWidget *widget) {
  widget->setFont(GetSmallFont());
}

void PageBase::SetWidgetFontCommon(QWidget *widget) {
  widget->setFont(GetCommonFont());
}

void PageBase::SetWidgetFontLargeBold(QWidget *widget) {
  widget->setFont(GetLargeBoldFont());
}

QFont PageBase::GetSmallFont() {
  QFont font;
  font.setPointSize(9);
  return font;
}

QFont PageBase::GetCommonFont() {
  QFont font;
  font.setPointSize(12);
  return font;
}

QFont PageBase::GetLargeBoldFont() {
  QFont font;
  font.setPointSize(16);
  font.setBold(true);
  return font;
}

bool PageBase::Initialize() {
  return true;
}
