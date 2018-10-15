#ifndef PAGE_BASE_H
#define PAGE_BASE_H

#include "export.h"
#include <memory>

class CommandEchoHandler;
class QLabel;
class QWidget;
class QFont;

class API PageBase
{
 public:
  PageBase();
  virtual ~PageBase();
  void SetCommandEchoHandler(std::shared_ptr<CommandEchoHandler> echo_handler);
  virtual void Update();
  void SetLabelFontCommon(QLabel* label);
  static void SetWidgetFontCommon(QWidget* widget);
  static QFont GetCommonFont();
  static QFont GetLargeBoldFont();
  static void SetWidgetFontLargeBold(QWidget* widget);
 protected:
   std::shared_ptr<CommandEchoHandler> EchoHandler();
 private:
  std::shared_ptr<CommandEchoHandler> echo_handler_;
};

#endif // PAGE_BASE_H
