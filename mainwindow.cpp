#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include <string>
#include <vector>
#include <tchar.h>
#include <QDebug>
#include <QMessageBox>

#pragma comment(lib, "advapi32")

std::wstring ReadRegValue(HKEY root, std::wstring key, std::wstring name)
{
    HKEY hKey;
    if (RegOpenKeyEx(root, key.c_str(), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
        throw "Could not open registry key";

    DWORD type;
    DWORD cbData;
    if (RegQueryValueEx(hKey, name.c_str(), NULL, &type, NULL, &cbData) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        throw "Could not read registry value";
    }

    if (type != REG_SZ)
    {
        RegCloseKey(hKey);
        throw "Incorrect registry value type";
    }

    std::wstring value(cbData / sizeof(wchar_t), L'\0');
    if (RegQueryValueEx(hKey, name.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&value[0]), &cbData) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        throw "Could not read registry value";
    }

    RegCloseKey(hKey);

    size_t firstNull = value.find_first_of(L'\0');
    if (firstNull != std::string::npos)
        value.resize(firstNull);

    return value;
}

HKEY OpenRegistryKey(HKEY rootKey, const wchar_t* strSubKey) {
    HKEY hKey;
    LONG lError = RegOpenKeyEx(rootKey, strSubKey, NULL, KEY_ALL_ACCESS, &hKey);

    if (ERROR_FILE_NOT_FOUND == lError) {
        lError = RegCreateKeyEx(rootKey, strSubKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
    }

    if (ERROR_CREATE_FAILED == lError) {
//        std::cout << "Something is wrong";
    }
    return hKey;
}

std::vector<std::wstring> getKeys(HKEY hKey) {
    std::vector<std::wstring> valuesList;
    TCHAR    achKey[500];		// buffer for subkey name
    DWORD    cbName;                   // size of name string
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
    DWORD    cchClassName = MAX_PATH;  // size of class string
    DWORD    cSubKeys = 0;               // number of subkeys
    DWORD    cbMaxSubKey;              // longest subkey size
    DWORD    cchMaxClass;              // longest class string
    DWORD    cValues;              // number of values for key
    DWORD    cchMaxValue;          // longest value name
    DWORD    cbMaxValueData;       // longest value data
    DWORD    cbSecurityDescriptor; // size of security descriptor
    FILETIME ftLastWriteTime;      // last write time

    DWORD i, retCode;

    TCHAR  achValue[500];
    DWORD cchValue = 500;

    retCode = RegQueryInfoKey(
        hKey,           // key handle
        NULL,           // buffer for class name
        NULL,           // size of class string
        NULL,           // reserved
        &cSubKeys,      // number of subkeys
        &cbMaxSubKey,   // longest subkey size
        NULL,           // longest class string
        &cValues,			// number of values for this key
        &cchMaxValue,			// longest value name
        NULL,			// longest value data
        NULL,			// security descriptor
        NULL			// last write time
    );

    if (cValues) {
        printf("\nNumber of values: %d\n", cValues);

        for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++)
        {
            cchValue = 500;
            achValue[0] = '\0';
            retCode = RegEnumValue(
                hKey,
                i,
                achValue,
                &cchValue,
                NULL,
                NULL,
                NULL,
                NULL
            );
            if (retCode == ERROR_SUCCESS) {
                valuesList.push_back(achValue);
                //_tprintf(TEXT("(%d) %s\n"), i + 1, achValue);
            }
        }
    }

    return valuesList;
}

void addAllKeysToList(std::vector<std::wstring> keys, QListWidget* list) {
    for (std::vector<std::wstring>::const_iterator i = keys.begin(); i != keys.end(); ++i)
        list->addItem(QString::fromStdWString(*i));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    HKEY hKey = OpenRegistryKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
//    HKEY hKey = OpenRegistryKey(HKEY_CURRENT_USER, L"SOFTWARE\\MyProject");
    HKEY path = OpenRegistryKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
    this->path = path;
    std::vector<std::wstring> keys = getKeys(path);
    ui->setupUi(this);
    addAllKeysToList(keys, ui->listWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    addWindow = new AddToStartupWindow(this, ui->listWidget);
    addWindow->exec();

//    ui->listWidget->addItem("Some value");
}

void MainWindow::on_pushButton_2_clicked()
{
    QListWidget* list = ui->listWidget;
    const int index = list->currentRow();
    const QString text = list->currentItem()->text();

    QMessageBox mb;
    mb.setWindowTitle("Warning");
    mb.setText(QString("Are you sure that you want to delete ") + text + " from startup?");
    mb.setStandardButtons(QMessageBox::Ok | QMessageBox::No);
    mb.setIcon(QMessageBox::Information);
    mb.setDefaultButton(QMessageBox::Ok);
    int res = mb.exec();
    if (res == QMessageBox::Ok) {
        qDebug() << index;
        list->removeItemWidget(list->takeItem(index));
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString value = QString::fromStdWString(ReadRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", item->text().toStdWString()));
    QString key = item->text();
    changeValueWindow  = new ChangeValueWindow(this, key, value, path);
    changeValueWindow ->exec();
//    qDebug() << item->text() << ReadRegValue(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", item->text().toStdWString());
}
