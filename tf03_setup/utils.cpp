#include "utils.h"
#include <QLineEdit>
#include <QIntValidator>

void SetLineEditIntValidity(
    QLineEdit *edit, const int& min, const int& max) {
  edit->setValidator(new QIntValidator(min, max, edit));
  edit->setPlaceholderText(QString::number(min) + " - " + QString::number(max));
}

void SetLineEditUShortValidity(QLineEdit *edit) {
  SetLineEditIntValidity(edit, 0, pow(2, (sizeof(unsigned short) * 8)) - 1);
}
