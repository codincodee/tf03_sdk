#include "common_tfmini_widget.h"

CommonTFMiniWidget::CommonTFMiniWidget(QWidget* parent)
  : CommonCommandWidget(parent)
{

}

CommonTFMiniWidget::~CommonTFMiniWidget() {}

void CommonTFMiniWidget::SetDriver(std::shared_ptr<TFMiniDriver> driver) {
  this->driver = driver;
}

void CommonTFMiniWidget::SetDriverServer(
    std::shared_ptr<TFMiniDriverServer> server) {
  this->server = server;
}
