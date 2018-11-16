#ifndef CART_TEST_SHEET_WIDGET_H
#define CART_TEST_SHEET_WIDGET_H

#include <QWidget>
#include "export.h"
#include <queue>

class QPlainTextEdit;

namespace Ui {
  class CartTestSheetWidget;
}

class API CartTestSheetWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CartTestSheetWidget(QWidget *parent = 0);
  void Clear();
  void UpdateLeftWing(const std::list<std::pair<int, int>>& steps);
  void UpdateRightWing(const std::list<std::pair<int, int>>& steps);
  ~CartTestSheetWidget();

private:
  void UpdateWing(
      const std::list<std::pair<int, int>>& steps,
      QPlainTextEdit* position, QPlainTextEdit* distance,
      std::queue<std::pair<int, int>>& que);
  Ui::CartTestSheetWidget *ui;
  std::queue<std::pair<int, int>> left_wing_;
  std::queue<std::pair<int, int>> right_wing_;
  int display_queue_size_ = 30;
};

#endif // CART_TEST_SHEET_WIDGET_H
