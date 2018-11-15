#include "utils.h"
#include <QLineEdit>
#include <QIntValidator>
#include <QLayout>
#include <QLayoutItem>
#include <QSerialPortInfo>
#include <QComboBox>

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

void UpdatePortNameComboBox(QComboBox* combo, QStringList& port_list) {
  auto ports = QSerialPortInfo::availablePorts();
  auto update = [combo, &port_list, ports](){
    combo->clear();
    for (auto& port : ports) {
      combo->addItem(port.portName());
    }
    port_list.clear();
    for (auto& port : ports) {
      port_list.push_back(port.portName());
    }
  };
  if (ports.size() != port_list.size()) {
    update();
    return;
  }
  bool should_update = false;
  for (auto& port : ports) {
    if (!port_list.contains(port.portName())) {
      should_update = true;
    }
  }
  if (should_update) {
    update();
  }
}
