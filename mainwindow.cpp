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

    memberTextEdit->setMaxLength(2);

    QDate *temp = new QDate();

    dir = new QDir(QDir().homePath());
    dir->setPath(QDir().homePath()+"/documents/BillableTimeCalculator");
    dir->mkdir(dir->path());

    dateEdit->setDate(temp->currentDate());

    billablePercentage->setText("0%");

    submitButton->setDisabled(true);
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

bool MainWindow::canSubmit()
{
    if(startTimeEdit->time() == stopTimeEdit->time())
    {

        return false;
    }

    if(!adminCheck->isChecked() && (memberTextEdit->text() == "" || memberTextEdit->text() == " " || memberTextEdit->text() == "  "))
    {

        return false;
    }
    return true;
}

void MainWindow::on_submitButton_clicked()
{
    QTime startTime = startTimeEdit->time();
    QTime stopTime = stopTimeEdit->time();

    startTimeEdit->setTime(stopTime); //Move the stop time to the start time display


    QDate enteredDate = dateEdit->date();

    //Generate the current date's folder
    folderName = toString(enteredDate.month())+"_"+toString(enteredDate.day())+"_"+toString(enteredDate.year());

    dir->mkdir(dir->path()+"/"+folderName);

    //Open the log file for data recording
    QFile outFile(dir->path()+"/"+folderName+"/log.txt");
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

    billableTimePercentage = totalBillableTime/totalTime*100;

    QString billableDisplayString = "";

    if(billableTimePercentage < 100)
    {

        billableDisplayString = toString(billableTimePercentage) + "." + toString(static_cast<int>(billableTimePercentage*10)/100) + "%";
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
    out << "--------------------\n";

    out << "Billable Percentage: " << billablePercentage->text();

    outFile.close();

    //Clear displays
    memberTextEdit->setText("");
    serviceEdit->clear();
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
