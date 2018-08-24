#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QDebug>

extern QStandardItemModel model;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public:
    QTableView *view = new QTableView;
    QStandardItemModel models;
signals:
    void updateTableSignal();
public slots:
    void updateTableSlot();
};

#endif // MAINWINDOW_H
