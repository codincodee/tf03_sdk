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

// TriggerIntTimeMeasure
class API TriggerIntTimeMeasure : public CommonTFMiniWidget {
public:
  TriggerIntTimeMeasure(QWidget* parent);
private:
  void OnButtonClickedCommon() override;
  void OnUpdate() override;
  QComboBox* combo = nullptr;
  const QString kI0 = "Int 0";
  const QString kI3 = "Int 3";
  const QString kI7 = "Int 7";
};

// SetTimer
class API SetTimer : public CommonTFMiniWidget {
public:
  SetTimer(QWidget* parent);
private:
  void OnButtonClickedCommon() override;
  void OnUpdate() override;
  QComboBox* combo = nullptr;
  const QString kOn = "On";
  const QString kOff = "Off";
};

// SetMetricUnit
class API SetMetricUnit : public CommonTFMiniWidget {
public:
  SetMetricUnit(QWidget* parent);
private:
  void OnButtonClickedCommon() override;
  void OnUpdate() override;
  QComboBox* combo = nullptr;
  const QString kMM = "mm";
  const QString kCM = "cm";
};

// SetIntTimeMode
class API SetIntTimeMode : public CommonTFMiniWidget {
public:
  SetIntTimeMode(QWidget* parent);
private:
  void OnButtonClickedCommon() override;
  void OnUpdate() override;
  QComboBox* combo = nullptr;
  const QString kTypical = "Typical";
  const QString kFixed = "Fixed";
};
} // namespace tfmini

#endif // TFMINI_COMMAND_WIDGETS_H
