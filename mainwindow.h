#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "customlabel.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr):QMainWindow(parent)
    {
        label = new CustomLabel(this);
        resize(320, 320);
        setCentralWidget(label);
    }

    ~MainWindow()
    {

    }


private:
    CustomLabel *label; // Custom QLabel to display the image

};
#endif // MAINWINDOW_H
