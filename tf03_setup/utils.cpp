#include "utils.h"
#include <QLineEdit>
#include <QIntValidator>
#include <QLayout>
#include <QLayoutItem>

void SetLineEditIntValidity(
    QLineEdit *edit, const int& min, const int& max) {
  edit->setValidator(new QIntValidator(min, max, edit));
  edit->setPlaceholderText(QString::number(min) + " - " + QString::number(max));
}

void SetLineEditUShortValidity(QLineEdit *edit) {
  SetLineEditIntValidity(edit, 0, pow(2, (sizeof(unsigned short) * 8)) - 1);
}

void ClearLayout(QLayout *layout) {
  QLayoutItem* child;
  while ( layout->count() != 0 ) {
      child = layout->takeAt ( 0 );
      if ( child->layout() != 0 ) {
          ClearLayout ( child->layout() );
      } else if ( child->widget() != 0 ) {
          delete child->widget();
      }
      delete child;
  }
}
