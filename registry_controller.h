#ifndef REGISTRY_CONTROLLER_H
#define REGISTRY_CONTROLLER_H

#include "Windows.h"
#include <QMessageBox>
#include <QListWidget>

class RegistryController {
    public:
    static std::vector<std::wstring> keys;
    static void SetRegistryValue(HKEY hRootKey, LPCTSTR lpVal, const wchar_t *value);
    static void DeleteRegistryKeyAndConfirm(HKEY hKey, LPCTSTR value);
    static HKEY OpenRegistryKey(HKEY rootKey, const wchar_t* strSubKey);
    static std::vector<std::wstring> getKeys(HKEY hKey);
    static std::wstring ReadRegValue(HKEY root, std::wstring key, std::wstring name);
};

#endif // REGISTRY_CONTROLLER_H
