#ifndef TFMINI_COMMAND_WIDGETS_H
#define TFMINI_COMMAND_WIDGETS_H

#include "common_tfmini_widget.h"
#include "export.h"

class QComboBox;

namespace tfmini {
// SetOutputFormat
class API SetOutputFormat : public CommonTFMiniWidget {
public:
  SetOutputFormat(QWidget* parent);
private:
  void OnButtonClickedCommon() override;
  void OnUpdate() override;
  QComboBox* combo = nullptr;
  const QString k9B = "9-bytes";
  const QString k29B = "29-bytes";
};
} // namespace tfmini

#endif // TFMINI_COMMAND_WIDGETS_H
