#include "addtostartupwindow.h"
#include "ui_addtostartupwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

AddToStartupWindow::AddToStartupWindow(QWidget *parent, QListWidget *list, HKEY path) :
    QDialog(parent),
    ui(new Ui::AddToStartupWindow)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->path = path;
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
    RegistryController::SetRegistryValue(path, key.toStdWString().c_str(), value.toStdWString().c_str());
    this->list->addItem(key);
    this->close();
    qDebug() << key << " " << value;
}

void AddToStartupWindow::on_pushButton_2_clicked()
{
    QString defaultDir = "C:";
    QString selfilter = tr("EXE (*.exe)");
    QString filename = QFileDialog::getOpenFileName(
                 this,
                "Choose file",
                defaultDir,
                tr("All files (*.*);;EXE (*.exe)"),
                &selfilter
                );
    defaultDir = filename.replace("/", "\\");
    ui->lineEdit2->setText(defaultDir);
}
