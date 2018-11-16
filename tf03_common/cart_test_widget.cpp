#include "cart_test_widget.h"
#include "ui_cart_test_widget.h"
#include "cart_test_sheet_widget.h"
#include "cart_driver.h"
#include "utils.h"

CartTestWidget::CartTestWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CartTestWidget)
{
  ui->setupUi(this);
  sheet_ = new CartTestSheetWidget(this);
  ui->SheetVerticalLayout->addWidget(sheet_);
  ui->ConnectPushButton->setText(kConnectButtonConnect);
  timer_id_ = startTimer(100);
}

CartTestWidget::~CartTestWidget()
{
  delete ui;
}

void CartTestWidget::timerEvent(QTimerEvent *event) {
  if (event->timerId() != timer_id_) {
    return;
  }
  UpdateSerialPortCombo();
}

void CartTestWidget::SetSensor(std::shared_ptr<DriverBase> driver) {
  sensor_ = driver;
}

bool CartTestWidget::Initialize() {
  if (!sensor_) {
    return false;
  }
  cart_.reset(new CartDriver);
  return true;
}

void CartTestWidget::on_StartPushButton_clicked()
{
  auto button = ui->StartPushButton;
  if (button->text() == kStartButtonStart) {
    sheet_->Clear();
    bool ok;
    auto distance = ui->DistanceLineEdit->text().toUInt(&ok);
    if (!ok) {
      return;
    }
    cart_->SetDistance(distance);
    if (cart_->Start()) {
      button->setText(kStartButtonFinish);
    }
    return;
  }
  button->setText(kStartButtonStart);
}

void CartTestWidget::on_OpenFolderPushButton_clicked()
{

}

void CartTestWidget::on_ConnectPushButton_clicked()
{
  auto button = ui->ConnectPushButton;
  if (button->text() == kConnectButtonDisconnect) {
    cart_->Close();
    button->setText(kConnectButtonConnect);
    return;
  }
  auto port = ui->PortComboBox->currentText();
  if (port.isEmpty()) {
    return;
  }
  cart_->SetPortName(port);
  cart_->SetBaudRate(460800);
  cart_->Open();
  button->setText(kConnectButtonDisconnect);
}

void CartTestWidget::UpdateSerialPortCombo() {
  UpdatePortNameComboBox(ui->PortComboBox, port_list_);
}
