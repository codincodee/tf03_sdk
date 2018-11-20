#include "cart_test_sheet_widget.h"
#include "ui_cart_test_sheet_widget.h"
#include <queue>
#include <QPlainTextEdit>
#include <QScrollBar>

CartTestSheetWidget::CartTestSheetWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CartTestSheetWidget)
{
  ui->setupUi(this);
  ui->FirstHalfDistancePlainTextEdit->verticalScrollBar()->setVisible(false);
  ui->SecondHalfDistancePlainTextEdit->verticalScrollBar()->setVisible(false);
  connect(
      ui->FirstHalfPositionPlainTextEdit->verticalScrollBar(),
      &QScrollBar::valueChanged,
      [this](int i){
        ui->FirstHalfDistancePlainTextEdit->
            verticalScrollBar()->setValue(i);});
  connect(
      ui->SecondHalfPositionPlainTextEdit->verticalScrollBar(),
      &QScrollBar::valueChanged,
      [this](int i){
        ui->SecondHalfDistancePlainTextEdit->
            verticalScrollBar()->setValue(i);});
}

CartTestSheetWidget::~CartTestSheetWidget()
{
  delete ui;
}

void CartTestSheetWidget::Clear() {
  ui->FirstHalfDistancePlainTextEdit->clear();
  ui->FirstHalfPositionPlainTextEdit->clear();
  ui->SecondHalfDistancePlainTextEdit->clear();
  ui->SecondHalfPositionPlainTextEdit->clear();
  while (!left_wing_.empty()) {
    left_wing_.pop();
  }
  while (!right_wing_.empty()) {
    right_wing_.pop();
  }
}

void CartTestSheetWidget::UpdateLeftWing(
    const std::list<std::pair<int, int> > &steps) {
  UpdateWing(
      steps,
      ui->FirstHalfPositionPlainTextEdit,
      ui->FirstHalfDistancePlainTextEdit,
      left_wing_);
}

void CartTestSheetWidget::UpdateRightWing(
    const std::list<std::pair<int, int> > &steps) {
  UpdateWing(
      steps,
      ui->SecondHalfPositionPlainTextEdit,
      ui->SecondHalfDistancePlainTextEdit,
      right_wing_);
}

void CartTestSheetWidget::UpdateWing(
    const std::list<std::pair<int, int> > &steps,
    QPlainTextEdit *position, QPlainTextEdit *distance,
    std::queue<std::pair<int, int>>& que) {
  for (auto& step : steps) {
    que.push(step);
    while (que.size() > display_queue_size_) {
      que.pop();
    }
  }
  position->clear();
  distance->clear();

  auto q = que;
  while (!q.empty()) {
    auto step = q.front();
    q.pop();
    auto cursor = position->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(QString::number(step.first) + "\n");

    cursor = distance->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.insertText(QString::number(step.second) + "\n");
  }
}
