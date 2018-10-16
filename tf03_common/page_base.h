#ifndef PAGE_BASE_H
#define PAGE_BASE_H

#include "export.h"
#include <memory>
#include <QObject>

class CommandEchoHandler;
class QLabel;
class QWidget;
class QFont;
struct MeasureBasic;
struct MeasureDevel;
class Driver;

class API PageBase : public QObject
{
 public:
  PageBase();
  virtual ~PageBase();
  virtual bool Initialize();
  void SetCommandEchoHandler(std::shared_ptr<CommandEchoHandler> echo_handler);
  void SetDriver(std::shared_ptr<Driver> driver);
  virtual void Update();
  virtual void OnMeasured(const MeasureBasic& measure);
  virtual void OnMeasured(const MeasureDevel& measure);
  static QFont GetSmallFont();
  static QFont GetCommonFont();
  static QFont GetLargeBoldFont();
  static void SetWidgetFontSmall(QWidget *widget);
  static void SetWidgetFontCommon(QWidget* widget);
  static void SetWidgetFontLargeBold(QWidget* widget);
 protected:
   std::shared_ptr<CommandEchoHandler> EchoHandler();
   std::shared_ptr<Driver> GetDriver();
 private:
  std::shared_ptr<CommandEchoHandler> echo_handler_;
  std::shared_ptr<Driver> driver_;
};

#endif // PAGE_BASE_H
