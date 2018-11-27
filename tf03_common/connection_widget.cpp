#include "connection_widget.h"
#include "ui_connection_widget.h"
#include "driver_base.h"
#include "utils.h"
#include <QDebug>

ConnectionWidget::ConnectionWidget(QWidget *parent) :
  CustomWidget(parent),
  ui(new Ui::ConnectionWidget)
{
  ui->setupUi(this);
  ui->ConnectPushButton->setText(kConnectButtonConnect);
}

void ConnectionWidget::SetDriver(std::shared_ptr<DriverBase> driver) {
  CustomWidget::SetDriver(driver);
  if (!driver) {
    return;
  }
  auto combo = ui->BaudRateComboBox;
  combo->clear();
  auto rates = driver->DriverBaudRates();
  for (auto& rate : rates) {
    combo->addItem(QString::number(rate));
  }
}

ConnectionWidget::~ConnectionWidget()
{
  delete ui;
}

void ConnectionWidget::on_ConnectPushButton_clicked()
{
  if (!driver) {
    return;
  }
  auto button = ui->ConnectPushButton;
  if (button->text() == kConnectButtonConnect) {
    auto port = ui->SerialPortComboBox->currentText();
    bool ok;
    auto baud = ui->BaudRateComboBox->currentText().toInt(&ok);
    if (!port.isEmpty() && ok) {
      driver->SetPortName(port);
      driver->SetBaudRate(baud);
      if (driver->Open()) {
        button->setText(kConnectButtonDisconnect);
      }
    }
  } else if (button->text() == kConnectButtonDisconnect) {
    if (driver->Close()) {
      button->setText(kConnectButtonConnect);
    }
  }
}

void ConnectionWidget::CommonTimerCallback() {
  UpdatePortNameComboBox(ui->SerialPortComboBox, port_list_);
}
