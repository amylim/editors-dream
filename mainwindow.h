#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "heap.h"
#include "tree.h"
#include "nodet.h"
#include <QTreeWidget>
#include <QStringList>
#include <QListWidget>
#include <QTime>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void printA(QString x);
    void printB(QString x);
    void printC(QString x);
    void printD(QString x);
    void printE(QString x);
    void printF(QString x);
    void printG(QString x);
    void printH(QString x);
    void printI(QString x);
    void printJ(QString x);
    void printK(QString x);
    void printL(QString x);
    void printM(QString x);
    void printN(QString x);
    void printO(QString x);
    void printP(QString x);
    void printQ(QString x);
    void printR(QString x);
    void printS(QString x);
    void printT(QString x);
    void printU(QString x);
    void printV(QString x);
    void printW(QString x);
    void printX(QString x);
    void printY(QString x);
    void printZ(QString x);
    void processWithTree(QString textFile);
    void processWithHeap(QString textFile);
    void printSummary(QString x);
    void getPhrase(char *phrase, char x, int &size);
    void sortAscendingNode(Node *x, int size);
    void getNode(Node *x, string str, int count, int &size);
    void findMostFreq(Node *x, int size, string str, int count);
    void sortAscendingNode(NodeT *x, int size);
    void getNode(NodeT *x, string str, int count, int &size);
    void findMostFreq(NodeT *x, int size, string str, int count);
    void increaseFreq(int *array, int index);

public slots:
    void openFile();

private:
    Ui::MainWindow *ui;
    void (MainWindow::*tabs[27])(QString);
};

#endif // MAINWINDOW_H
