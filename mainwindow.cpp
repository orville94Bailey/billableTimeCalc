#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stopTimeEdit = ui->stopTimeEdit;
    startTimeEdit = ui->startTimeEdit;
    dateEdit = ui->dateEdit;
    billablePercentage = ui->billablePercentage;
    serviceEdit = ui->serviceEdit;
    memberTextEdit = ui->memberTextEdit;
    submitButton = ui->submitButton;
    adminCheck = ui->adminCheck;

    totalTime = 0;
    totalBillableTime = 0;

    memberTextEdit->setMaxLength(5);

    QDate *temp = new QDate();

    dir = new QDir(QDir().homePath());
    dir->setPath(QDir().homePath()+"/documents/BillableTimeCalculator");
    dir->mkdir(dir->path());

    dateEdit->setDate(temp->currentDate());

    delete temp;

    folderName = toString(QDate().currentDate().month())+"_"+toString(QDate().currentDate().day())+"_"+toString(QDate().currentDate().year());

    if(dir->exists(dir->path()+"/"+folderName+"/data"))
    {

        QFile dataFile(dir->path()+"/"+folderName+"/data");
        dataFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream dataStream(&dataFile);
        totalBillableTime = toInt(dataStream.readLine());
        totalTime = toInt(dataStream.readLine());
    }

    if(totalTime == 0)
    {
        billableTimePercentage = 0;
        billablePercentage->setText("0%");
    }
    else
    {

        billableTimePercentage = totalBillableTime/totalTime*100;
        if(billableTimePercentage < 100)
        {

            billablePercentage->setText(toString(billableTimePercentage,1)+"%");
        }
        else
        {
            billablePercentage->setText("0%");
        }
    }

    submitButton->setDisabled(true);

    //Read the total member time log
    if(dir->exists(dir->path()+"/" + folderName + "/MemberTimes.txt"))
    {

        QFile memberLog(dir->path()+"/" + folderName + "/MemberTimes.txt");
        memberLog.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&memberLog);

        std::vector<QString> lines;
        while(!in.atEnd())
        {
            qDebug() << "Read line";
            lines.push_back(in.readLine());
        }

        memberLog.close();

        QStringList temp;
        int i = 0;
        while(i<lines.size())
        {
            if(lines.at(i) != "")
            {
                temp = lines.at(i).split(" ");
                if(temp.length() > 1)
                {
                    memberNames.push_back(temp.at(0));
                    memberTimes.push_back(strTimetoSec(temp.at(1)));
                }
            }
            i++;
        }
    }

    for(int i = 0;i<memberNames.size();i++)
    {

        qDebug() << memberNames.at(i) << memberTimes.at(i);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stopTimeEdit;
    delete startTimeEdit;
    delete dateEdit;
    delete billablePercentage;
    delete serviceEdit;
    delete memberTextEdit;
    delete submitButton;
    delete adminCheck;
}

QString MainWindow::toString(int num)
{
    QString str = "";

    if(num == 0)
    {

        return "0";
    }

    while(num > 0)
    {

        int digit = num % 10;
        num = num / 10;
        QString temp = " ";
        temp[0] = static_cast<char>(digit+48);
        str = temp + str;
    }

    return str;
}

QString MainWindow::toString(float num, int decimalPlaces = 3)
{

    int mainNum = num;
    QString str = toString(mainNum);

    if(decimalPlaces == 0)
    {

        return str;
    }
    else
    {
        str += ".";
        for(int i = 0;i<decimalPlaces;i++)
        {

            num -= mainNum;
            num = num * 10;
            mainNum = num;
            str += toString(mainNum);
        }

        return str;
    }
}

int MainWindow::toInt(QString str)
{
    int num = 0;
    std::string stdstr = str.toStdString();
    for(int i = 0;i < stdstr.length();i++)
    {

        num = num*10;
        num += stdstr[i]-48;
    }

    return num;
}

bool MainWindow::canSubmit()
{
    if(startTimeEdit->time() == stopTimeEdit->time())
    {

        return false;
    }

    if(!adminCheck->isChecked() && (memberTextEdit->text() == "" || memberTextEdit->text() == " " || memberTextEdit->text() == "  "
                                    || memberTextEdit->text() == "   " || memberTextEdit->text() == "    " || memberTextEdit->text() == "     "))
    {

        return false;
    }
    return true;
}

QString MainWindow::secsToStringTime(int secs)
{

    QString time = "";
    int hours = secs/3600;
    QString strHours = toString(hours);
    if(strHours.length() < 2)
    {

        strHours = "0" + strHours;
    }
    time = strHours;
    secs = secs%3600;

    int mins = secs/60;
    time += ":";
    QString strMins = toString(mins);
    if(strMins.length() < 2)
    {

        strMins = "0" + strMins;
    }
    time += strMins;

    return time;
}

void MainWindow::on_submitButton_clicked()
{
    QTime startTime = startTimeEdit->time();
    QTime stopTime = stopTimeEdit->time();

    startTimeEdit->setTime(stopTime); //Move the stop time to the start time display

    //Generate the current date's folder

    dir->mkdir(dir->path()+"/"+folderName);

    //Open the log file for data recording
    QFile outFile(dir->path()+"/"+folderName+"/serviceLog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&outFile);

    QString serviceText = serviceEdit->toPlainText();
    QString memberText = memberTextEdit->text();

    QStringList serviceWords = serviceText.split(" ");

    std::vector<QString> pieces;
    pieces.push_back("");

    int i = 0;
    int currentPiece = 0;

    if(!adminCheck->isChecked())
    {

        totalBillableTime+=startTime.secsTo(stopTime);
    }
    else
    {

        memberText = "ADMIN";
    }

    memberText = memberText.toUpper();

    totalTime+=startTime.secsTo(stopTime);

    //log the total time and billable time for the day in a data file
    QFile dataFile(dir->path()+"/"+folderName+"/data");
    dataFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream dataStream(&dataFile);
    dataStream << totalBillableTime << "\n"
               << totalTime << "\n";

    dataFile.close();

    billableTimePercentage = totalBillableTime/totalTime*100;

    qDebug() << billableTimePercentage << endl << totalBillableTime << totalTime;

    QString billableDisplayString = "";

    if(billableTimePercentage < 100)
    {

        billableDisplayString = toString(billableTimePercentage,1)+"%";
    }
    else
    {

        billableDisplayString = "100%";
    }

    billablePercentage->setText(billableDisplayString);

    //Split the service text into lines of ~80 characters, preserving whole words
    while(i < serviceWords.size())
    {
        if(pieces.at(currentPiece).length()+serviceWords.at(i).length() <= 80)
        {

            pieces.at(currentPiece) = pieces.at(currentPiece) + serviceWords.at(i) + " ";
        }
        else
        {
            if(pieces.at(currentPiece) == "")
            {

                pieces.at(currentPiece) = serviceWords.at(i);
            }
            currentPiece++;
            pieces.push_back("");
        }
        i++;
    }

    //Log the entered data
    out << "--------------------\n"
        << "Start: " << startTime.toString("h:mm") << "\n"
        << "Stop: " << stopTime.toString("h:mm") << "\n"
        << "Service: \n";

    i = 0;
    while(i < pieces.size())
    {
        out << pieces.at(i) << "\n";
        i++;
    }

    out << "Member: " << memberText << "\n";
    out << "Billable Percentage: " << billablePercentage->text() << "\n";
    out << "--------------------\n";

    outFile.close();

    //Update the member time log
    bool foundMember = false;
    for(int i = 0;i<memberNames.size();i++)
    {

        if(memberNames.at(i) == memberText)
        {

            foundMember = true;
            memberTimes[i] += startTime.secsTo(stopTime);
        }
    }

    if(!foundMember)
    {

        memberNames.push_back(memberText);
        memberTimes.push_back(startTime.secsTo(stopTime));
    }

    for(int i = 0;i<memberNames.size();i++)
    {

        qDebug() << memberNames.at(i) << memberTimes.at(i);
    }

    QFile memberFile(dir->path()+"/"+folderName+"/MemberTimes.txt");
    memberFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream memberStream(&memberFile);
    for(int i = 0; i < memberNames.size();i++)
    {

        qDebug() << secsToStringTime(memberTimes.at(i));
        memberStream << memberNames.at(i) << " " << secsToStringTime(memberTimes.at(i)) << "\n";
    }

    memberFile.close();

    //Clear displays and reset the admin check box
    memberTextEdit->setText("");
    serviceEdit->clear();
    adminCheck->setChecked(false);
}

void MainWindow::on_startTimeEdit_timeChanged(const QTime &time)
{
    stopTimeEdit->setMinimumTime(startTimeEdit->time());
    if(canSubmit())
    {

        submitButton->setDisabled(false);
    }
    else
    {

        submitButton->setDisabled(true);
    }
}

void MainWindow::on_adminCheck_toggled(bool checked)
{
    memberTextEdit->setText("");
    if(canSubmit())
    {

        submitButton->setDisabled(false);
    }
    else
    {

        submitButton->setDisabled(true);
    }
}

void MainWindow::on_stopTimeEdit_timeChanged(const QTime &time)
{
    if(canSubmit())
    {

        submitButton->setDisabled(false);
    }
    else
    {

        submitButton->setDisabled(true);
    }
}

void MainWindow::on_memberTextEdit_textChanged(const QString &arg1)
{
    if(canSubmit())
    {

        submitButton->setDisabled(false);
    }
    else
    {

        submitButton->setDisabled(true);
    }
}

int MainWindow::strTimetoSec(QString strTime)
{
    QString hours = "";
    QString mins = "";

    int numHours = 0;
    int numMins = 0;

    QStringList timeSections = strTime.split(":");

    hours = timeSections.at(0);
    mins = timeSections.at(1);

    numHours = toInt(hours);
    numMins = toInt(mins);

    return numHours*3600 + numMins*60;
}

void MainWindow::on_dateEdit_dateChanged(const QDate &date)
{

    folderName = toString(date.month())+"_"+toString(date.day())+"_"+toString(date.year());
}

////////////////////
//LogEntry methods//
////////////////////

LogEntry::LogEntry() : memberName(""), startTime(""),stopTime(""),notes(""),startTimeSecs(0),stopTimeSecs(0),elapsedTimeSecs(0)
{

}

LogEntry::~LogEntry()
{


}

QString LogEntry::getMemberName()
{

    return memberName;
}

void LogEntry::setMemberName(QString newName)
{

    memberName = newName;
}

QString LogEntry::getStartTime()
{

    return startTime;
}

void LogEntry::setStartTime(QString newTime)
{

    startTime = newTime;
}

QString LogEntry::getStopTime()
{

    return stopTime;
}

void LogEntry::setStopTime(QString newTime)
{

    stopTime = newTime;
}

int LogEntry::getStartTimeSecs()
{

    return startTimeSecs;
}

void LogEntry::setStartTimeSecs(int newTime)
{

    startTimeSecs = newTime;
    elapsedTimeSecs = stopTimeSecs - startTimeSecs;
}

int LogEntry::getStopTimeSecs()
{

    return stopTimeSecs;
}

void LogEntry::setStopTimeSecs(int newTime)
{

    stopTimeSecs = newTime;
    elapsedTimeSecs = stopTimeSecs - startTimeSecs;
}

void LogEntry::readLog()
{

}
