#include "tfmini_command_widgets.h"
#include <QComboBox>

namespace tfmini {
// MiniSetOutputFormat
SetOutputFormat::SetOutputFormat(QWidget* parent)
  : CommonTFMiniWidget(parent) {
  item_label->setText("Output Format");
  combo = new QComboBox(this);
  combo->addItem(k9B);
  combo->addItem(k29B);
  option = combo;
}

void SetOutputFormat::OnButtonClickedCommon() {
  if (!driver) {
    return;
  }
  auto current = combo->currentText();
  if (current == k9B) {
    driver->SetOutputFormat(TFMiniOutputFormat::b_9);
  } else if (current == k29B) {
    driver->SetOutputFormat(TFMiniOutputFormat::b_29);
  }
}

void SetOutputFormat::OnUpdate() {
  SetCommandEchoed(true);
}
}
