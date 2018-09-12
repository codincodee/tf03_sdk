#include "command_echo_widgets_manager.h"
#include <QGridLayout>
#include "command_echo_widgets.h"
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include "firmware_module.h"
#include <QProgressBar>

CommandEchoWidgetsManager::CommandEchoWidgetsManager() {
}

void CommandEchoWidgetsManager::SetUIGrid(QGridLayout *layout) {
  ui_grid_ = layout;
}

void CommandEchoWidgetsManager::SetupFirmwareLayout(QGridLayout *layout) {
  firmware_grid_ = layout;
}

void CommandEchoWidgetsManager::ConfigWidgets(
    std::shared_ptr<CommandEchoWidgets> widget) {
  if (!widget) {
    return;
  }
  if (!widget->driver) {
    widget->driver = driver_;
  }
  if (!widget->echo_handler) {
    widget->echo_handler = echo_handler_;
  }
}

void CommandEchoWidgetsManager::AddWidgets(
    std::shared_ptr<CommandEchoWidgets> widget) {
  if (!widget) {
    return;
  }
  ConfigWidgets(widget);
  widgets_.push_back(widget);
}

void CommandEchoWidgetsManager::SetupFirmwareUIGrid(QGridLayout *layout) {
  if (!firmware_grid_) {
    return;
  }
  int col = 0;
  layout->addWidget(firmware_widgets_->item, 0, col++);
  layout->addWidget(firmware_widgets_->option, 0, col++);
  layout->addWidget(firmware_widgets_->browse, 0, col++);
  layout->addWidget(firmware_widgets_->button, 0, col++);
  layout->addWidget(firmware_widgets_->progress, 0, col++);
  layout->addWidget(firmware_widgets_->status, 0, col++);
}

void CommandEchoWidgetsManager::SetRespondAllButtonClick(bool respond) {
  for (auto& w : widgets_) {
    w->respond_button_click = respond;
  }
}

void CommandEchoWidgetsManager::SetupUIGrid(QGridLayout *layout) {
  qDebug() << __FUNCTION__ << __LINE__;
  auto widget_num = widgets_.size();
  qDebug() << __FUNCTION__ << __LINE__;
  constexpr int kRowMax = 15;
  qDebug() << __FUNCTION__ << __LINE__;
  auto column_num = widgets_.size() / kRowMax + 1;
  qDebug() << __FUNCTION__ << __LINE__;
  auto parent = parent_widget_;
  qDebug() << __FUNCTION__ << __LINE__;

  std::vector<QGridLayout*> layouts;
  qDebug() << __FUNCTION__ << __LINE__;
  {
    qDebug() << __FUNCTION__ << __LINE__;
    int col = 0;
    qDebug() << __FUNCTION__ << __LINE__;
    for (int i = 0; i < column_num; ++i) {
      qDebug() << __FUNCTION__ << __LINE__;
      auto sub_layout = new QGridLayout;
      qDebug() << __FUNCTION__ << __LINE__;
      layouts.push_back(sub_layout);
      qDebug() << __FUNCTION__ << __LINE__;
      if (i != 0) {
        qDebug() << __FUNCTION__ << __LINE__;
        QFrame* vline = new QFrame(parent);
        qDebug() << __FUNCTION__ << __LINE__;
        vline->setFrameShape(QFrame::VLine);
        qDebug() << __FUNCTION__ << __LINE__;
        vline->setLineWidth(1);
        qDebug() << __FUNCTION__ << __LINE__;
        layout->addWidget(vline, 0, col++);
        qDebug() << __FUNCTION__ << __LINE__;
      }
      qDebug() << __FUNCTION__ << __LINE__;
      layout->addLayout(sub_layout, 0, col++);
      qDebug() << __FUNCTION__ << __LINE__;
    }
    qDebug() << __FUNCTION__ << __LINE__;
  }
  qDebug() << __FUNCTION__ << __LINE__;

  int row = 0;
  qDebug() << __FUNCTION__ << __LINE__;
  auto id = 0;
  qDebug() << __FUNCTION__ << __LINE__;
  while (id < widgets_.size()) {
    qDebug() << __FUNCTION__ << __LINE__;
    for (int i = 0; i < column_num; ++i) {
      qDebug() << __FUNCTION__ << __LINE__;
      if (id >= widgets_.size()) {
        qDebug() << __FUNCTION__ << __LINE__;
        break;
      }
      qDebug() << __FUNCTION__ << __LINE__;
      int column = 0;
      qDebug() << __FUNCTION__ << __LINE__;
      layouts[i]->addWidget(widgets_[id]->item, row, column++);
      qDebug() << __FUNCTION__ << __LINE__;
      if (widgets_[id]->option) {
        qDebug() << __FUNCTION__ << __LINE__;
        layouts[i]->addWidget(widgets_[id]->option, row, column++);
        qDebug() << __FUNCTION__ << __LINE__;
      } else {
        qDebug() << __FUNCTION__ << __LINE__;
        ++column;
        qDebug() << __FUNCTION__ << __LINE__;
      }
      qDebug() << __FUNCTION__ << __LINE__;
      layouts[i]->addWidget(widgets_[id]->button, row, column++);
      qDebug() << __FUNCTION__ << __LINE__;
      layouts[i]->addWidget(widgets_[id]->status, row, column++);
      qDebug() << __FUNCTION__ << __LINE__;
      ++id;
      qDebug() << __FUNCTION__ << __LINE__;
    }
    qDebug() << __FUNCTION__ << __LINE__;
    ++row;
    qDebug() << __FUNCTION__ << __LINE__;
  }
  qDebug() << __FUNCTION__ << __LINE__;
}

void CommandEchoWidgetsManager::SetDriver(std::shared_ptr<Driver> driver) {
  driver_ = driver;
}

void CommandEchoWidgetsManager::SetEchoHandler(
    std::shared_ptr<CommandEchoHandler> handler) {
  echo_handler_ = handler;
}

void CommandEchoWidgetsManager::UpdateUITexts() {
  static auto func = [](std::shared_ptr<CommandEchoWidgets> widgets) {
    widgets->item->setText(which_lingual(widgets->item_lingual));
    widgets->button->setText(which_lingual(widgets->button_lingual));
    widgets->SetOptionLingual();
    widgets->status->setText(which_lingual(widgets->status_lingual));
  };
  for (auto& widgets : widgets_) {
    func(widgets);
  }

  if (firmware_widgets_) {
    func(firmware_widgets_);
  }
}

void CommandEchoWidgetsManager::Update() {
  for (auto& widgets : widgets_) {
    widgets->Update();
  }
  if (firmware_widgets_) {
    firmware_widgets_->Update();
  }
}

void CommandEchoWidgetsManager::SetParentWidget(QWidget *widget) {
  parent_widget_ = widget;
}

void CommandEchoWidgetsManager::LoadWidgets() {
  #ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
  AddWidgets(
      std::shared_ptr<SetProtocolWidgets>(new SetProtocolWidgets));
  AddWidgets(
      std::shared_ptr<WriteSerialNumberWidgets>(new WriteSerialNumberWidgets));
  #endif
  AddWidgets(
      std::shared_ptr<RequestVersionWidgets>(new RequestVersionWidgets));
  AddWidgets(
      std::shared_ptr<SerialNumberWidgets>(new SerialNumberWidgets));
  AddWidgets(
      std::shared_ptr<SetSerialBaudRateWidgets>(new SetSerialBaudRateWidgets));
  AddWidgets(
      std::shared_ptr<SetFrequencyWidgets>(new SetFrequencyWidgets));
  AddWidgets(
      std::shared_ptr<SetOutputFormatWidgets>(new SetOutputFormatWidgets));
  AddWidgets(
      std::shared_ptr<OutputSwitchWidgets>(new OutputSwitchWidgets));
  AddWidgets(
      std::shared_ptr<MeasureTriggerWidgets>(new MeasureTriggerWidgets));
  AddWidgets(
      std::shared_ptr<SetPortTypeWidgets>(new SetPortTypeWidgets));
  AddWidgets(
      std::shared_ptr<SetOutRangeValueWidgets>(new SetOutRangeValueWidgets));
  AddWidgets(
      std::shared_ptr<SetCANSendIDWidgets>(new SetCANSendIDWidgets));
  AddWidgets(
      std::shared_ptr<SetCANReceiveIDWidgets>(new SetCANReceiveIDWidgets));
  AddWidgets(
      std::shared_ptr<SetCANBaudRateWidgets>(new SetCANBaudRateWidgets));
  AddWidgets(
      std::shared_ptr<FlashSettingsWidgets>(new FlashSettingsWidgets));
  AddWidgets(
      std::shared_ptr<RestoreFactoryWidgets>(new RestoreFactoryWidgets));

#ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
  AddWidgets(
      std::shared_ptr<SetAPDWidgets>(new SetAPDWidgets));
  AddWidgets(
      std::shared_ptr<AutoGainAdjustWidgets>(new AutoGainAdjustWidgets));
  AddWidgets(
      std::shared_ptr<AdaptiveAPDWidgets>(new AdaptiveAPDWidgets));
  AddWidgets(
      std::shared_ptr<APDClosedLoopWidgets>(new APDClosedLoopWidgets));
#endif

  qDebug() << __FUNCTION__ << __LINE__;
  if (ui_grid_) {
    qDebug() << __FUNCTION__ << __LINE__;
    SetupUIGrid(ui_grid_);
    qDebug() << __FUNCTION__ << __LINE__;
  }
  qDebug() << __FUNCTION__ << __LINE__;

  if (firmware_grid_) {
    qDebug() << __FUNCTION__ << __LINE__;
    firmware_widgets_.reset(new UpgradeFirmwareWidgets);
    qDebug() << __FUNCTION__ << __LINE__;
    firmware_widgets_->set_respond_all_button =
        std::bind(
            &CommandEchoWidgetsManager::SetRespondAllButtonClick, this,
            std::placeholders::_1);
    qDebug() << __FUNCTION__ << __LINE__;
    ConfigWidgets(firmware_widgets_);
    qDebug() << __FUNCTION__ << __LINE__;
    SetupFirmwareUIGrid(firmware_grid_);
    qDebug() << __FUNCTION__ << __LINE__;
  }
  qDebug() << __FUNCTION__ << __LINE__;
}
