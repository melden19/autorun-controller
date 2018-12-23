#include "changevaluewindow.h"
#include "ui_changevaluewindow.h"
#include "QMessageBox"
#include "QFileDialog"

ChangeValueWindow::ChangeValueWindow(QWidget *parent, QString key, QString value, HKEY path) :
    QDialog(parent),
    ui(new Ui::ChangeValueWindow)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->key=key;
    this->value=value;
    this->path=path;
    ui->setupUi(this);

    ui->label->setText("Edit path to .exe file for " + key + ":");
    ui->lineEdit->setText(value);
}

ChangeValueWindow::~ChangeValueWindow()
{
    delete ui;
}

void ChangeValueWindow::on_pushButton_clicked()
{
    if (value != ui->lineEdit->text())
    RegistryController::SetRegistryValue(path, key.toStdWString().c_str(), ui->lineEdit->text().toStdWString().c_str());
    this->close();
}

void ChangeValueWindow::on_pushButton_2_clicked()
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
    ui->lineEdit->setText(defaultDir);
}
