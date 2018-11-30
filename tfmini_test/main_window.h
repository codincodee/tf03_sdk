#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>

class TFMiniDriver;

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_DebugPushButton_clicked();

private:
  Ui::MainWindow *ui;

  std::shared_ptr<TFMiniDriver> driver_;
};

#endif // MAIN_WINDOW_H
