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

    dir->setPath(dir->path()+"/"+folderName);
    dir->mkdir(dir->path());
    qDebug() << dir->path();

    QFile outFile(dir->path()+"/log.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&outFile);

    QString serviceText = serviceEdit->toPlainText();

    int i = 0;
    std::vector<QString> pieces;
    pieces.push_back("");
    while(i < serviceText.length())
    {
        pieces.at(i/80).push_back(serviceText.at(i));
        i++;
        if(i % 80 == 0)
        {

            pieces.push_back("");
        }
    }

    /*out << "--------------------\n\n";
    out << "Start: ";
    out << toString(startTime.hour());
    out << ":";
    out << toString(startTime.minute()) << "\n"
        << "Stop: " << toString(stopTime.hour()) << ":" << toString(stopTime.minute()) << "\n";

    out << "Service: \n";*/

    i = 0;
    while(i < pieces.size())
    {
        /*out << pieces.at(i) << "\n";*/
        i++;
    }

    /*out << "Member: " << memberTextEdit->text();
    out << "--------------------";
    out.flush();*/
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
