#ifndef SAVE_LOG_WIDGET_H
#define SAVE_LOG_WIDGET_H

#include <QWidget>

namespace Ui {
  class SaveLogWidget;
}

class SaveLogWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SaveLogWidget(QWidget *parent = 0);
  void SetPath(const QString& path);
  void OnLanguageChanged();
  QString WaitForFileName();
  ~SaveLogWidget();

protected:
  void showEvent(QShowEvent* event) override;

private slots:
  void on_OpenFolderPushButton_clicked();

  void on_OkPushButton_clicked();

  void on_CancelPushButton_clicked();

private:
  Ui::SaveLogWidget *ui;
  QString path_;
  QString file_name_;
  bool canceled_ = false;
};

#endif // SAVE_LOG_WIDGET_H
