#ifndef COMMON_COMMAND_WIDGET_H
#define COMMON_COMMAND_WIDGET_H

#include <QLabel>
#include <QPushButton>
#include "custom_widget.h"
#include "export.h"
#include <QElapsedTimer>

class CommandBlock;

class API CommonCommandWidget : public CustomWidget
{
  Q_OBJECT
public:
  CommonCommandWidget(QWidget* parent);
  virtual ~CommonCommandWidget();
private slots:
  void OnPushButtonClicked();
protected:
  friend class CommandBlock;
  virtual void CommonTimerCallback();
  virtual void OnButtonClicked();
  virtual void OnButtonClickedCommon();
  virtual void Update();
  virtual void OnUpdate();
  virtual bool ProceedUpdate();
  void SetCommandEchoed(const bool& success);

  QWidget* item = nullptr;
  QWidget* option = nullptr;
  QWidget* button = nullptr;
  QWidget* status = nullptr;

  QLabel* item_label = nullptr;
  QPushButton* push_button = nullptr;
  QLabel* status_label = nullptr;

  QElapsedTimer timer;
  int timeout = 1000;

  const QString kButtonTextSet = "set";
private:
  QWidget* parent_ = nullptr;
};

#endif // COMMON_WIDGET_H
