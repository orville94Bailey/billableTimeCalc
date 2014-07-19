#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <iostream>
#include <string>

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

    void on_submitButton_clicked();

private:
    Ui::MainWindow *ui;

    QLineEdit *stopTimeText;
    QLineEdit *startTimeText;
};

#endif // MAINWINDOW_H
