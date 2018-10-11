#ifndef UTILS_H
#define UTILS_H

#include "export.h"

class QLineEdit;
class QLayout;

void API SetLineEditIntValidity(
    QLineEdit *edit, const int& min, const int& max);

void API SetLineEditUShortValidity(QLineEdit *edit);

void API ClearLayout(QLayout* layout);
#endif // UTILS_H
