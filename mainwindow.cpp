#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stopTimeText = ui->stopTimeText;
    startTimeText = ui->startTimeText;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_submitButton_clicked()
{
    QString stopString = stopTimeText->text();
    stopTimeText->setText("");
    startTimeText->setText(stopString);
}
