#include "cart_test_sheet.h"
#include "cart_driver.h"

CartTestSheet::CartTestSheet(QWidget *parent)
  : CartTestSheetWidget(parent)
{

}

void CartTestSheet::Update(const std::list<CartStep> &steps) {
  std::list<std::pair<int, int>> left;
  std::list<std::pair<int, int>> right;
  for (auto& step : steps) {
    if (step.measure) {
      if (step.first_half) {
        left.push_back({step.position, step.measure->dist});
      } else {
        right.push_back({step.position, step.measure->dist});
      }
    }
  }
  if (!left.empty()) {
    UpdateLeftWing(left);
  }
  if (!right.empty()) {
    UpdateRightWing(right);
  }
}
