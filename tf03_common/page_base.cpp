#include "page_base.h"
#include <QLabel>
#include "parsed.h"

PageBase::PageBase()
{

}

PageBase::~PageBase() {}

void PageBase::SetCommandEchoHandler(
    std::shared_ptr<CommandEchoHandler> echo_handler) {
  echo_handler_ = echo_handler;
}

void PageBase::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

std::shared_ptr<Driver> PageBase::GetDriver() {
  return driver_;
}

std::shared_ptr<CommandEchoHandler> PageBase::EchoHandler() {
  return echo_handler_;
}

void PageBase::Update() {
}

void PageBase::OnMeasured(const MeasureBasic &measure) {

}

void PageBase::OnMeasured(const MeasureDevel &measure) {
  OnMeasured(ToMeasureBasic(measure));
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
  font.setPointSize(7);
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
