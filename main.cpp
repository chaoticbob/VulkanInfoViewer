#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
  QCoreApplication::addLibraryPath(".");
#if defined(_WIN32)
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
