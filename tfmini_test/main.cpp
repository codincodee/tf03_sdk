#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <thread>

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  win.show();
  return app.exec();
}
