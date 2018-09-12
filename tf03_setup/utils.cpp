#include "utils.h"
#include <QLineEdit>
#include <QIntValidator>

void SetLineEditIntValidity(
    QLineEdit *edit, const int& min, const int& max) {
  edit->setValidator(new QIntValidator(min, max, edit));
  edit->setPlaceholderText(QString::number(min) + " - " + QString::number(max));
}
