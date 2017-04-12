#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
  QCoreApplication::addLibraryPath(".");
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication a(argc, argv);
#if defined(__linux__)
  a.setStyle(QStyleFactory::create("Fusion"));
#endif
  MainWindow w;
  w.show();

  return a.exec();
}
