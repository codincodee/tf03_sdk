#include "cart_test_widget.h"
#include "ui_cart_test_widget.h"
#include "cart_test_sheet.h"
#include "cart_driver.h"
#include "utils.h"
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

CartTestWidget::CartTestWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CartTestWidget)
{
  ui->setupUi(this);
  sheet_ = new CartTestSheet(this);
  ui->SheetVerticalLayout->addWidget(sheet_);
  ui->ConnectPushButton->setText(kConnectButtonConnect);
  ui->DistanceLineEdit->setValidator(new QIntValidator(0, 10000, this));
  ui->DistanceLineEdit->setText(QString::number(1000));
  ui->LogFileNameLineEdit->setText(kArchiveFileNameDefault);
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
  if (ui->StartPushButton->text() == kStartButtonStart) {
    return;
  }
  auto steps = cart_->GetStepBuffer();
  if (steps) {
    if (!steps->empty()) {
      sheet_->Update(*steps);
    }
  }
}

void CartTestWidget::SetSensor(std::shared_ptr<DriverBase> driver) {
  sensor_ = driver;
}

bool CartTestWidget::Initialize() {
  if (!sensor_) {
    return false;
  }
  cart_.reset(new CartDriver);
  cart_->RegisterMeasureCallback([this](){return sensor_->LastMeasureStatic();});
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
  QMessageBox box;
  box.setWindowTitle("Warning");
  box.setText("Please reset the cart before proceeding.");
  box.setWindowModality(Qt::WindowModal);
  box.addButton(QMessageBox::Ok);
  box.addButton(QMessageBox::Cancel);
  box.setButtonText(QMessageBox::Ok, "I know");
  if (box.exec() != QMessageBox::Ok) {
    return;
  }
  auto full = cart_->GetFullSteps();
  Archive(full);
  button->setText(kStartButtonStart);
}

void CartTestWidget::on_OpenFolderPushButton_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(ArchiveFolder()));
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

bool CartTestWidget::Archive(const std::list<CartStep> &full) {
  QFile file(ArchiveFilePath());
  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate| QIODevice::Text)) {
    return false;
  }
  QTextStream stream(&file);
  Archive(full, stream);
  file.close();
  return true;
}

bool CartTestWidget::Archive(
    const std::list<CartStep> &full, QTextStream &stream) {
  for (auto& step : full) {
    stream
        << step.position << " "
        << (step.measure ? QString::number(step.measure->dist) : "") << "\n";
  }
  return true;
}

QString CartTestWidget::ArchiveFolder() {
  return archive_folder_;
}

QString CartTestWidget::ArchiveFilePath() {
  auto file_name = ui->LogFileNameLineEdit->text();
  return
      archive_folder_ + "/" +
      (file_name.isEmpty() ?  kArchiveFileNameDefault : file_name) +
      ".txt";
}
