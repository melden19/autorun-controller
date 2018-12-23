#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include <string>
#include <vector>
#include <tchar.h>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>

#pragma comment(lib, "advapi32")

std::wstring toLower(std::wstring str);

void addAllKeysToList(std::vector<std::wstring> keys, QListWidget* list, bool purgeFirstly = false);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    HKEY path = RegistryController::OpenRegistryKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    this->path = path;
    this->keys = RegistryController::getKeys(path);

    addAllKeysToList(keys, ui->listWidget);
    ui->listWidget->setCurrentRow(0);

    m_statusLeft = new QLabel("Number of startup programs: " + QString::number(ui->listWidget->count()), this);
    m_statusLeft->setFrameStyle(QFrame::Plain);
    m_statusRight = new QLabel(QString::fromStdWString(keys.front()), this);
    m_statusRight->setFrameStyle(QFrame::Plain);
    statusBar()->addPermanentWidget(m_statusLeft, 1);
    statusBar()->addPermanentWidget(m_statusRight, 1);

    QApplication::instance()->setAttribute(Qt::AA_DontShowIconsInMenus, true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete addWindow;
    delete changeValueWindow;
    delete aboutWindow;
    delete m_statusLeft;
    delete m_statusRight;
}

void MainWindow::on_pushButton_clicked()
{
    addWindow = new AddToStartupWindow(this, ui->listWidget, path);
    addWindow->exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    QListWidget* list = ui->listWidget;
    const int index = list->currentRow();
    const QString text = list->currentItem()->text();
    qDebug() << text;

    QMessageBox mb;
    mb.setWindowTitle("Warning");
    mb.setText(QString("Are you sure that you want to delete ") + text + " from startup?");
    mb.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
    mb.setIcon(QMessageBox::Information);
    mb.setDefaultButton(QMessageBox::Ok);
    int res = mb.exec();
    if (res == QMessageBox::Ok) {
        RegistryController::DeleteRegistryKeyAndConfirm(path, text.toStdWString().c_str());
        list->removeItemWidget(list->takeItem(index));
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString value = QString::fromStdWString(RegistryController::ReadRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", item->text().toStdWString()));
    QString key = item->text();
    changeValueWindow  = new ChangeValueWindow(this, key, value, path);
    changeValueWindow ->exec();
}

void MainWindow::on_actionNew_program_triggered()
{
    addWindow = new AddToStartupWindow(this, ui->listWidget, path);
    addWindow->exec();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    aboutWindow = new About();
    aboutWindow->exec();
}

void MainWindow::on_actionManual_triggered()
{
    QString path = QDir::currentPath() + "/Manual.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    m_statusRight->setText(item->text());
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    std::vector<std::wstring> result;
    std::copy_if (keys.begin(), keys.end(), std::back_inserter(result), [arg1](std::wstring i) {return toLower(i).find(toLower(arg1.toStdWString())) != std::string::npos;});
    addAllKeysToList(result, ui->listWidget, true);
    ui->listWidget->setCurrentRow(0);
    if (result.size() != 0)
        m_statusRight->setText(QString::fromStdWString(result.front()));
    else m_statusRight->setText("No programs match search");
    m_statusLeft->setText("Number of startup programs: " + QString::number(result.size()));
}

std::wstring toLower(std::wstring str) {
    transform(
        str.begin(), str.end(),
        str.begin(),
        towlower);
    return str;
}

void addAllKeysToList(std::vector<std::wstring> keys, QListWidget* list, bool purgeFirstly) {
    if (purgeFirstly) list->clear();
    for (std::vector<std::wstring>::const_iterator i = keys.begin(); i != keys.end(); ++i)
        list->addItem(QString::fromStdWString(*i));
}
