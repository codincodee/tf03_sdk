#ifndef UTILS_H
#define UTILS_H

class QLineEdit;
class QLayout;

void SetLineEditIntValidity(
    QLineEdit *edit, const int& min, const int& max);

void SetLineEditUShortValidity(QLineEdit *edit);

void ClearLayout(QLayout* layout);
#endif // UTILS_H
