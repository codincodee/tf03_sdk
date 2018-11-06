#include "save_log_widget.h"
#include "ui_save_log_widget.h"
#include "lingual.h"
#include <QDesktopServices>
#include <QUrl>

SaveLogWidget::SaveLogWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SaveLogWidget)
{
  ui->setupUi(this);
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  this->setWindowModality(Qt::ApplicationModal);
  OnLanguageChanged();
}

SaveLogWidget::~SaveLogWidget()
{
  delete ui;
}

void SaveLogWidget::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  file_name_.clear();
  canceled_ = false;
}

void SaveLogWidget::SetPath(const QString &path) {
  path_ = path;
}

void SaveLogWidget::OnLanguageChanged() {
  ui->TipLabel->setText(
      which_lingual({"Please Type File Name: ", "请输入文件名："}));
  ui->OpenFolderPushButton->setText(which_lingual({"Open Folder", "打开文件夹"}));
  ui->OkPushButton->setText(which_lingual({"Ok", "确定"}));
  ui->CancelPushButton->setText(which_lingual({"Cancel", "取消"}));
}

void SaveLogWidget::on_OpenFolderPushButton_clicked()
{
  QDesktopServices::openUrl(QUrl::fromLocalFile(path_));
}

QString SaveLogWidget::WaitForFileName() {
  while (file_name_.isEmpty()) {
    qApp->processEvents();
    if (canceled_) {
      break;
    }
  }
  return file_name_;
}

void SaveLogWidget::on_OkPushButton_clicked()
{
  file_name_ = ui->lineEdit->text();
}

void SaveLogWidget::on_CancelPushButton_clicked()
{
  canceled_ = true;
  hide();
}
