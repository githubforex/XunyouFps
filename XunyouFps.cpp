#include <iostream>

#include <Windows.h>






using namespace std;


BOOL OptimizationMouseSpeed(HKEY h_rootKey, std::string str_subKey, std::string str_keyValueName, std::string str_keyValue, DWORD cbData = 1)
{
    HKEY hKey;
    //      关闭鼠标加速
    RegCreateKeyExA(h_rootKey, str_subKey.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);

    RegSetValueExA(hKey, str_keyValueName.c_str(), 0, REG_SZ, (LPBYTE)str_keyValue.c_str(), cbData);

    RegCloseKey(hKey);

    return TRUE;
}







int main(int argc, char* argv[])
{
    //OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSpeed", "0");

    //OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold1", "0");

    //OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold2", "0");


    OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSpeed", "1");

    OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold1", "50", 3);

    OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold2", "80", 2);
    
    //RegCreateKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, 0, 0, 0x00020106, 0, &hKey, 0);

    cout << "param count:" << argc << endl;

    for (size_t i = 0; i < argc; i++)
    {
        cout << (char*)argv[i] << endl;
    }

    return 0;
}
