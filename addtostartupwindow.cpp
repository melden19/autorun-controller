#include "addtostartupwindow.h"
#include "ui_addtostartupwindow.h"
#include <QDebug>

AddToStartupWindow::AddToStartupWindow(QWidget *parent, QListWidget *list) :
    QDialog(parent),
    ui(new Ui::AddToStartupWindow)
{
    this->list = list;
    ui->setupUi(this);
}

AddToStartupWindow::~AddToStartupWindow()
{
    delete list;
    delete ui;
}

void AddToStartupWindow::on_pushButton_clicked()
{
    QString key = ui->lineEdit1->text();
    QString value = ui->lineEdit2->text();
    this->list->addItem(key);
    this->close();
    qDebug() << key << " " << value;
}
