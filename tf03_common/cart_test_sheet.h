#ifndef CART_TEST_SHEET_H
#define CART_TEST_SHEET_H

#include "cart_test_sheet_widget.h"

struct CartStep;

class API CartTestSheet : public CartTestSheetWidget
{
public:
  CartTestSheet(QWidget *parent = 0);
  void Update(const std::list<CartStep>& steps);
};

#endif // CART_TEST_SHEET_H
