#ifndef UTILS_H
#define UTILS_H

class QLineEdit;

void SetLineEditIntValidity(
    QLineEdit *edit, const int& min, const int& max);

void SetLineEditUShortValidity(QLineEdit *edit);

#endif // UTILS_H
