#include "changevaluewindow.h"
#include "ui_changevaluewindow.h"
#include "QMessageBox"
#include "QFileDialog"

void SetRegistryValue(HKEY hRootKey, LPCTSTR lpVal, const wchar_t *value);

ChangeValueWindow::ChangeValueWindow(QWidget *parent, QString key, QString value, HKEY path) :
    QDialog(parent),
    ui(new Ui::ChangeValueWindow)
{
    this->key=key;
    this->value=value;
    this->path=path;
    ui->setupUi(this);

    ui->label->setText("Edit value for " + key);
    ui->lineEdit->setText(value);
//    ui->lineEdit->setCursorPosition(0);
}

ChangeValueWindow::~ChangeValueWindow()
{
    delete ui;
}

void ChangeValueWindow::on_pushButton_clicked()
{
    if (value != ui->lineEdit->text())
    SetRegistryValue(path, key.toStdWString().c_str(), ui->lineEdit->text().toStdWString().c_str());
    this->close();
}

void SetRegistryValue(HKEY hRootKey, LPCTSTR lpVal, const wchar_t *value)
{
    QMessageBox m;
    m.setWindowTitle("Status");

    LONG nErr = RegSetValueEx(hRootKey, lpVal, NULL, REG_SZ, (const BYTE*)value, ((DWORD)wcslen(value) + 1)*sizeof(wchar_t));
    if (nErr != ERROR_SUCCESS)
    {
        m.setText("Error!");
    } else {
        m.setText("Success!");
    }
    m.exec();
}

void ChangeValueWindow::on_pushButton_2_clicked()
{
    QString defaultDir = "";
    QString filename = QFileDialog::getExistingDirectory(
                 this,
                "Open folder",
                defaultDir,
                QFileDialog::ShowDirsOnly
                );
    defaultDir = filename;
    ui->lineEdit->setText(defaultDir);
}
