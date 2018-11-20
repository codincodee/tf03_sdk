#ifndef UTILS_H
#define UTILS_H

#include "export.h"

class QLineEdit;
class QLayout;
class QComboBox;
class QStringList;
#include <QByteArray>

void API SetLineEditIntValidity(
    QLineEdit *edit, const int& min, const int& max);

void API SetLineEditUShortValidity(QLineEdit *edit);

void API ClearLayout(QLayout* layout);

inline bool API IsDevelModeSupported() {
#ifdef SUPPORT_DEVEL_MODE_PROTOCOL_
  return true;
#else
  return false;
#endif
}

inline bool API IsAPDExperimentPagUsed() {
#ifdef USE_APD_EXPERIMENT_PAGE
  return true;
#else
  return false;
#endif
}

void UpdatePortNameComboBox(QComboBox* combo, QStringList& port_list);

template<typename T>
QByteArray to_bytes(const T& value) {
  QByteArray buffer(sizeof(value), 0);
  memcpy(buffer.data(), &value, sizeof(value));
  return buffer;
}
#endif // UTILS_H
