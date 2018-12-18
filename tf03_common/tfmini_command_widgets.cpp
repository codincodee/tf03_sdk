#include "tfmini_command_widgets.h"
#include <QComboBox>

namespace tfmini {
// SetOutputFormat
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

// TriggerIntTimeMeasure
TriggerIntTimeMeasure::TriggerIntTimeMeasure(QWidget* parent)
  : CommonTFMiniWidget(parent) {
  item_label->setText("Trigger Int-time Measure");
  combo = new QComboBox(this);
  combo->addItem(kI0);
  combo->addItem(kI3);
  combo->addItem(kI7);
  option = combo;
}

void TriggerIntTimeMeasure::OnButtonClickedCommon() {
  if (!driver) {
    return;
  }
  auto current = combo->currentText();
  if (current == kI0) {
    driver->TriggerIntTimeMeasure(0);
  } else if (current == kI3) {
    driver->TriggerIntTimeMeasure(3);
  } else if (current == kI7) {
    driver->TriggerIntTimeMeasure(7);
  }
}

void TriggerIntTimeMeasure::OnUpdate() {
  SetCommandEchoed(true);
}

// SetTimer
SetTimer::SetTimer(QWidget *parent)
  : CommonTFMiniWidget(parent) {
  item_label->setText("Set Timer");
  combo = new QComboBox(this);
  combo->addItem(kOn);
  combo->addItem(kOff);
  option = combo;
}

void SetTimer::OnButtonClickedCommon() {
  if (!driver) {
    return;
  }
  auto current = combo->currentText();
  if (current == kOn) {
    driver->SetTimer(true);
  } else if (current == kOff) {
    driver->SetTimer(false);
  }
}

void SetTimer::OnUpdate() {
  SetCommandEchoed(true);
}

// SetMetricUnit
SetMetricUnit::SetMetricUnit(QWidget *parent)
  : CommonTFMiniWidget(parent) {
  item_label->setText("Metric Unit");
  combo = new QComboBox(this);
  combo->addItem(kMM);
  combo->addItem(kCM);
  option = combo;
}

void SetMetricUnit::OnButtonClickedCommon() {
  if (!driver) {
    return;
  }
  auto current = combo->currentText();
  if (current == kMM) {
    driver->SetMetricUnit(true);
  } else if (current == kCM) {
    driver->SetMetricUnit(false);
  }
}

void SetMetricUnit::OnUpdate() {
  SetCommandEchoed(true);
}

// SetIntTimeMode
SetIntTimeMode::SetIntTimeMode(QWidget *parent)
  : CommonTFMiniWidget(parent) {
  item_label->setText("Int-time Mode");
  combo = new QComboBox(this);
  combo->addItem(kTypical);
  combo->addItem(kFixed);
  combo->addItem(kCyclic);
  option = combo;
}

void SetIntTimeMode::OnButtonClickedCommon() {
  if (!driver) {
    return;
  }
  auto current = combo->currentText();
  if (current == kTypical) {
    driver->SetIntTimeMode(TFMiniIntTimeMode::typical);
  } else if (current == kFixed) {
    driver->SetIntTimeMode(TFMiniIntTimeMode::fixed);
  } else if (current == kCyclic) {
    driver->SetIntTimeMode(TFMiniIntTimeMode::cyclic);
  }
}

void SetIntTimeMode::OnUpdate() {
  SetCommandEchoed(true);
}
}
