#ifndef COMMAND_ECHO_WIDGETS_MANAGER_H
#define COMMAND_ECHO_WIDGETS_MANAGER_H

#include <memory>
#include <unordered_map>
#include "lingual.h"
#include <QObject>

class Driver;
class QGridLayout;
struct CommandEchoWidgets;
class CommandEchoHandler;
struct UpgradeFirmwareWidgets;

class CommandEchoWidgetsManager : QObject {
 public:
  CommandEchoWidgetsManager();
  void SetUIGrid(QGridLayout *layout);
  void SetDriver(std::shared_ptr<Driver> driver);
  void SetEchoHandler(std::shared_ptr<CommandEchoHandler> handler);
  void LoadWidgets();

  void AddWidgets(std::shared_ptr<CommandEchoWidgets> widget);
  void UpdateUITexts();
  void Update();
  void SetParentWidget(QWidget* widget);
  void SetupFirmwareLayout(QGridLayout* layout);
 private:
  void ConfigWidgets(std::shared_ptr<CommandEchoWidgets> widget);
  void SetupUIGrid(QGridLayout* layout);
  void SetupFirmwareUIGrid(QGridLayout* layout);
  void SetRespondAllButtonClick(bool respond);
  std::vector<std::shared_ptr<CommandEchoWidgets>> widgets_;
  QGridLayout* ui_grid_ = nullptr;
  QGridLayout* firmware_grid_ = nullptr;
  std::shared_ptr<UpgradeFirmwareWidgets> firmware_widgets_;
  std::shared_ptr<Driver> driver_;
  std::shared_ptr<CommandEchoHandler> echo_handler_;
  QWidget* parent_widget_;
};
#endif // COMMAND_ECHO_WIDGETS_MANAGER_H
