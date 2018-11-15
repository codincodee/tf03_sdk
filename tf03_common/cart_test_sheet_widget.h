#ifndef CART_TEST_SHEET_WIDGET_H
#define CART_TEST_SHEET_WIDGET_H

#include <QWidget>
#include "export.h"

namespace Ui {
  class CartTestSheetWidget;
}

class API CartTestSheetWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CartTestSheetWidget(QWidget *parent = 0);
  void Clear();
  ~CartTestSheetWidget();

private:
  Ui::CartTestSheetWidget *ui;
};

#endif // CART_TEST_SHEET_WIDGET_H
