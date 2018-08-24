#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) /*:
    QMainWindow(parent),
    ui(new Ui::MainWindow)*/
{
//    ui->setupUi(this);
    connect(this,updateTableSignal,this,updateTableSlot);
    view->setModel(&models);
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete view;
}

void MainWindow::updateTableSlot()
{
    view->setUpdatesEnabled(false);
    view->setUpdatesEnabled(true);
}
