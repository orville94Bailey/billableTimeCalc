#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTimeEdit>
#include <QDateEdit>
#include <QtDebug>
#include <QLabel>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>

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

    //pointers for UI elements
    QTimeEdit *stopTimeEdit;
    QTimeEdit *startTimeEdit;
    QDateEdit *dateEdit;
    QTextEdit *serviceEdit;
    QLineEdit *memberTextEdit;
    QLabel *billablePercentage;

    QDir *dir;

    QString toString(int);

    float totalTime=0;
    float totalBillableTime=0;
    float billableTimePercentage=0;
};

#endif // MAINWINDOW_H
