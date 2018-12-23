#include "registry_controller.h"
void RegistryController::SetRegistryValue(HKEY hRootKey, LPCTSTR lpVal, const wchar_t *value)
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

void RegistryController::DeleteRegistryKeyAndConfirm(HKEY hKey, LPCTSTR value)
{
    LONG res = RegDeleteValueW(hKey, value);
    if (res != ERROR_SUCCESS) {
//        qDebug() << "error";
    } else {
//        qDebug() << "success";
    }
}

std::vector<std::wstring> RegistryController::getKeys(HKEY hKey) {
    std::vector<std::wstring> valuesList;
    DWORD    cSubKeys = 0;               // number of subkeys
    DWORD    cbMaxSubKey;              // longest subkey size
    DWORD    cValues;              // number of values for key
    DWORD    cchMaxValue;          // longest value name

    LONG retCode, i;

    TCHAR  achValue[500];
    DWORD cchValue = 500;

    retCode = RegQueryInfoKey(
        hKey,           // key handle
        nullptr,           // buffer for class name
        nullptr,           // size of class string
        nullptr,           // reserved
        &cSubKeys,      // number of subkeys
        &cbMaxSubKey,   // longest subkey size
        nullptr,           // longest class string
        &cValues,			// number of values for this key
        &cchMaxValue,			// longest value name
        nullptr,			// longest value data
        nullptr,			// security descriptor
        nullptr			// last write time
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
                nullptr,
                nullptr,
                nullptr,
                nullptr
            );
            if (retCode == ERROR_SUCCESS) {
                valuesList.push_back(achValue);
                //_tprintf(TEXT("(%d) %s\n"), i + 1, achValue);
            }
        }
    }

    return valuesList;
}

HKEY RegistryController::OpenRegistryKey(HKEY rootKey, const wchar_t* strSubKey) {
    HKEY hKey;
    LONG lError = RegOpenKeyEx(rootKey, strSubKey, NULL, KEY_ALL_ACCESS, &hKey);

    if (ERROR_FILE_NOT_FOUND == lError) {
        lError = RegCreateKeyEx(rootKey, strSubKey, NULL, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hKey, nullptr);
    }

    if (ERROR_CREATE_FAILED == lError) {
//        std::cout << "Something is wrong";
    }
    return hKey;
}

std::wstring RegistryController::ReadRegValue(HKEY root, std::wstring key, std::wstring name){
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
