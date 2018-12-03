#ifndef COMMAND_BLOCK_H
#define COMMAND_BLOCK_H

#include "custom_widget.h"
#include <memory>
#include "export.h"

namespace Ui {
  class CommandBlock;
}

class QGridLayout;
class CommonCommandWidget;

class API CommandBlock : public CustomWidget
{
  Q_OBJECT
public:
  explicit CommandBlock(QWidget *parent = nullptr);
  ~CommandBlock();
  void LoadWidgets(
      const std::vector<CommonCommandWidget*>& widgets);
signals:

public slots:

private:
  void NewLayout();

  Ui::CommandBlock *ui;
  QGridLayout* layout_ = nullptr;
  std::vector<CommonCommandWidget*> widgets_;
};

#endif // COMMAND_BLOCK_H
