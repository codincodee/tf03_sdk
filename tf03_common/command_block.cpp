#include "command_block.h"
#include <QFrame>
#include <QGridLayout>
#include "utils.h"
#include "common_command_widget.h"
#include "ui_command_block.h"

CommandBlock::CommandBlock(QWidget *parent)
  : CustomWidget(parent), ui(new Ui::CommandBlock)
{
  ui->setupUi(this);
}

CommandBlock::~CommandBlock() {
  delete ui;
}

void CommandBlock::NewLayout() {

}

void CommandBlock::LoadWidgets(
    const std::vector<std::shared_ptr<CommonCommandWidget> > &widgets) {
  widgets_ = widgets;
  auto layout = ui->gridLayout;
  ClearLayout(layout);

  constexpr int kRowMax = 15;
  auto column_num = widgets_.size() / kRowMax + 1;

  std::vector<QGridLayout*> layouts;
  {
    int col = 0;
    for (int i = 0; i < column_num; ++i) {
      auto sub_layout = new QGridLayout;
      layouts.push_back(sub_layout);
      if (i != 0) {
        QFrame* vline = new QFrame(this);
        vline->setFrameShape(QFrame::VLine);
        vline->setLineWidth(1);
        layout->addWidget(vline, 0, col++);
      }
      layout->addLayout(sub_layout, 0, col++);
    }
  }

  int row = 0;
  auto id = 0;
  while (id < widgets_.size()) {
    for (int i = 0; i < column_num; ++i) {
      if (id >= widgets_.size()) {
        break;
      }
      int column = 0;
      layouts[i]->addWidget(widgets_[id]->item, row, column++);
      if (widgets_[id]->option) {
        layouts[i]->addWidget(widgets_[id]->option, row, column++);
      } else {
        ++column;
      }
      layouts[i]->addWidget(widgets_[id]->button, row, column++);
      layouts[i]->addWidget(widgets_[id]->status, row, column++);
      ++id;
    }
    ++row;
  }
}
