#include "rte_cart_widget.h"
#include "ui_rte_cart_widget.h"
#include "rte_cart_server.h"
#include <QThread>
#include <QDebug>

RTECartWidget::RTECartWidget(QWidget *parent) :
  CustomWidget(parent),
  ui(new Ui::RTECartWidget)
{
  ui->setupUi(this);
  ui->StartPushButton->setText(kStartButtonStart);
  ui->ContinuePushButton->setVisible(false);
}

RTECartWidget::~RTECartWidget()
{
  delete ui;
}

void RTECartWidget::SetDriverServer(std::shared_ptr<RTECartServer> server) {
  server_ = server;
  driver_ = server_->Driver();
  driver_->SetDistance(1200);
  connect(server_.get(), SIGNAL(I037Burn()), this, SLOT(OnI037Burn()));
  connect(server_.get(), SIGNAL(I037TempBurn()), this, SLOT(OnI037TempBurn()));
  connect(server_.get(), SIGNAL(Heating()), this, SLOT(OnHeating()));
  connect(server_.get(), SIGNAL(Finished()), this, SLOT(OnFinished()));
  connect(server_.get(), SIGNAL(AutoI()), this, SLOT(OnAutoI()));
  connect(server_.get(), SIGNAL(I037Back()), this, SLOT(OnI037Back()));
  connect(server_.get(), SIGNAL(I037Temp()), this, SLOT(OnI037Temp()));
}

void RTECartWidget::OnI037Burn() {
  ui->InformationLabel->setText("i037 Burn");

  qApp->processEvents();
  QThread::msleep(3000);
  if (driver_) {
    driver_->GoOn();
  }
}

void RTECartWidget::OnI037TempBurn() {
  ui->InformationLabel->setText("i037 Temp Burn");

  qApp->processEvents();
  QThread::msleep(3000);
  if (driver_) {
    driver_->GoOn();
  }
}

void RTECartWidget::OnHeating() {
  ui->InformationLabel->setText("Heating");
  ui->ContinuePushButton->setVisible(true);
  ui->StartPushButton->setVisible(false);
}

void RTECartWidget::OnFinished() {
  ui->InformationLabel->setText("Finshed");
}

void RTECartWidget::on_ContinuePushButton_clicked()
{
  ui->ContinuePushButton->setVisible(false);
  ui->StartPushButton->setVisible(true);
  if (driver_) {
    driver_->FinishedHeating();
  }
}

void RTECartWidget::on_StartPushButton_clicked()
{
  auto button = ui->StartPushButton;
  if (button->text() == kStartButtonStart) {
    ui->InformationLabel->setText("I037");
    if (server_) {
      server_->Start();
    }
    button->setText(kStartButtonStop);
  } else if (button->text() == kStartButtonStop) {
    button->setText(kStartButtonStart);
  }
}

void RTECartWidget::OnI037Back() {
  ui->InformationLabel->setText("I037 Back");
}

void RTECartWidget::OnAutoI() {
  ui->InformationLabel->setText("AutoI");
}

void RTECartWidget::OnI037Temp() {
  ui->InformationLabel->setText("I037 Temp");
}
