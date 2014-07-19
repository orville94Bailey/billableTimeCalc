#include "foldermaker.h"
#include "ui_foldermaker.h"

FolderMaker::FolderMaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FolderMaker)
{
    ui->setupUi(this);
    makeDir = new QDir();
    folderText = ui->folderText;
}

FolderMaker::~FolderMaker()
{
    delete ui;
    delete makeDir;
    delete folderText;
}

void FolderMaker::on_makeButton_clicked()
{
    if(folderText->text() != "")
    {
        if(removeFlag == false)
        {
            makeDir->mkpath(folderText->text());
        }
        else
        {
            makeDir->setPath(folderText->text());
            makeDir->removeRecursively();
        }
    }
}

void FolderMaker::on_removeCheck_stateChanged(int state)
{
    if(state == 0)
    {
        removeFlag = false;
    }
    else
    {
        removeFlag = true;
    }
}

void FolderMaker::on_folderText_returnPressed()
{
    if(folderText->text() != "")
    {
        makeDir->mkpath(folderText->text());
    }
}
