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

    QDate *temp = new QDate();

    dir = new QDir(QDir().homePath());
    qDebug() << QDir().homePath();
    dir->setPath(QDir().homePath()+"/documents/BillableTimeCalculator");
    dir->mkdir(dir->path());

    dateEdit->setDate(temp->currentDate());

    std::setprecision(3);
    billablePercentage->setText("");
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
}


void MainWindow::on_submitButton_clicked()
{
    QTime startTime = startTimeEdit->time();
    QTime stopTime = stopTimeEdit->time();

    startTimeEdit->setTime(stopTime); //Move the stop time to the start time display


    QDate enteredDate = dateEdit->date();

    //Generate the current date's folder
    QString folderName = toString(enteredDate.month())+"_"+toString(enteredDate.day())+"_"+toString(enteredDate.year());

    dir->mkdir(dir->path()+"/"+folderName);

    //Open the log file for data recording
    QFile outFile(dir->path()+"/"+folderName+"/log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream out(&outFile);

    QString serviceText = serviceEdit->toPlainText();

    QStringList serviceWords = serviceText.split(" ");

    std::vector<QString> pieces;
    pieces.push_back("");

    int i = 0;
    int currentPiece = 0;


    if(memberTextEdit->text().toUpper()!="ADMIN")
    {
        totalBillableTime+=startTime.secsTo(stopTime);
    }

    totalTime+=startTime.secsTo(stopTime);

    billableTimePercentage = totalBillableTime/totalTime*100;

    billablePercentage->setText(toString(billableTimePercentage) + "%");

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
        << "Start: " << startTime.toString("h:m") << "\n"
        << "Stop: " << stopTime.toString("h:m") << "\n"
        << "Service: \n";

    i = 0;
    while(i < pieces.size())
    {
        out << pieces.at(i) << "\n";
        i++;
    }

    out << "Member: " << memberTextEdit->text() << "\n";
    out << "--------------------\n";

    outFile.close();

    //Clear displays
    memberTextEdit->setText("");
    serviceEdit->clear();
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
