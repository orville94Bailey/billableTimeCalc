#ifndef FOLDERMAKER_H
#define FOLDERMAKER_H

#include <QMainWindow>
#include <QDir>
#include <QLineEdit>

namespace Ui {
class FolderMaker;
}

class FolderMaker : public QMainWindow
{
    Q_OBJECT

public:
    explicit FolderMaker(QWidget *parent = 0);
    ~FolderMaker();

private slots:
    void on_makeButton_clicked();

    void on_removeCheck_stateChanged(int);

    void on_folderText_returnPressed();

private:
    Ui::FolderMaker *ui;
    QDir *makeDir;
    QLineEdit *folderText;
    bool removeFlag;
};

#endif // FOLDERMAKER_H
