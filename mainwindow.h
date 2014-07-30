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


    void on_dateEdit_dateChanged(const QDate &date);

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
    QString toString(float,int);
    int toInt(QString);
    int strTimetoSec(QString);
    QString secsToStringTime(int);
    bool canSubmit();

    std::vector<QString> memberNames;
    std::vector<int> memberTimes;

    float totalTime;
    float totalBillableTime;
    float billableTimePercentage;

    QString folderName;
};

class LogEntry
{

public:

    LogEntry();
    ~LogEntry();

    QString getMemberName();
    void setMemberName(QString);

    QString getStartTime();
    void setStartTime(QString);

    QString getStopTime();
    void setStopTime(QString);

    QString getNotes();
    void setNotes(QString);

    int getStartTimeSecs();
    void setStartTimeSecs(int);

    int getStopTimeSecs();
    void setStopTimeSecs(int);

    void readLog();



private:

    QString memberName;
    QString startTime;
    QString stopTime;
    QString notes;

    int processLine(QString);

    int startTimeSecs;
    int stopTimeSecs;

    int elapsedTimeSecs;
};

#endif // MAINWINDOW_H
