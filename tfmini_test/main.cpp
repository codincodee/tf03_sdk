#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <thread>
#include <tf03_common/tf_algorithm_pack.h>

int main(int argc, char* argv[])
{
  TFAlgorithmPack pack;
  pack.Test();
  QApplication app(argc, argv);
  MainWindow win;
  win.show();
  return app.exec();
}
