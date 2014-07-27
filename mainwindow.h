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
#include <QPushButton>
#include <QCheckBox>
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

    void on_startTimeEdit_timeChanged(const QTime &time);

    void on_adminCheck_toggled(bool checked);

    void on_stopTimeEdit_timeChanged(const QTime &time);

    void on_memberTextEdit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    //pointers for UI elements
    QTimeEdit *stopTimeEdit;
    QTimeEdit *startTimeEdit;
    QDateEdit *dateEdit;
    QTextEdit *serviceEdit;
    QLineEdit *memberTextEdit;
    QLabel *billablePercentage;
    QPushButton *submitButton;
    QCheckBox *adminCheck;

    QDir *dir;

    QString toString(int);
    bool canSubmit();

    float totalTime;
    float totalBillableTime;
    float billableTimePercentage;

    QString folderName;
};

#endif // MAINWINDOW_H
