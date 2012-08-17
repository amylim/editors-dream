/***************************************************************
  Author: Amy Lim
  Program: CS 8 - Test 2: Editor's Dream
  Date: May 31, 2011
  **************************************************************/

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
