#include "addtostartupwindow.h"
#include "ui_addtostartupwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

void SetRegistryValues(HKEY hRootKey, LPCTSTR lpVal, const wchar_t *value);

AddToStartupWindow::AddToStartupWindow(QWidget *parent, QListWidget *list, HKEY path) :
    QDialog(parent),
    ui(new Ui::AddToStartupWindow)
{
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
    SetRegistryValues(path, key.toStdWString().c_str(), value.toStdWString().c_str());
    this->list->addItem(key);
    this->close();
    qDebug() << key << " " << value;
}

void SetRegistryValues(HKEY hRootKey, LPCTSTR lpVal, const wchar_t *value)
{
    QMessageBox m;
    m.setWindowTitle("Status");

    LONG nErr = RegSetValueEx(hRootKey, lpVal, NULL, REG_SZ, (const BYTE*)value, ((DWORD)wcslen(value) + 1)*sizeof(wchar_t));
    if (nErr != ERROR_SUCCESS)
    {
        m.setText("Error!");
    } else {
        m.setText("Value added to the registry!");
    }
    m.exec();
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
