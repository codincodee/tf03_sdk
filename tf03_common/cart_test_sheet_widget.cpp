#include "cart_test_sheet_widget.h"
#include "ui_cart_test_sheet_widget.h"

CartTestSheetWidget::CartTestSheetWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CartTestSheetWidget)
{
  ui->setupUi(this);
}

CartTestSheetWidget::~CartTestSheetWidget()
{
  delete ui;
}

void CartTestSheetWidget::Clear() {}
