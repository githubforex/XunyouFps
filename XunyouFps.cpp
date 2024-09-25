#include <iostream>

#include <Windows.h>






using namespace std;

/// <summary>
/// 关闭鼠标加速
/// </summary>
/// <param name="h_rootKey"></param>
/// <param name="str_subKey"></param>
/// <param name="str_keyValueName"></param>
/// <param name="str_keyValue"></param>
/// <param name="cbData"></param>
/// <returns></returns>
BOOL OptimizationMouseSpeed(HKEY h_rootKey, std::string str_subKey, std::string str_keyValueName, std::string str_keyValue, DWORD cbData = 1)
{
    HKEY hKey;
    //      关闭鼠标加速
    RegCreateKeyExA(h_rootKey, str_subKey.c_str(), 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);

    RegSetValueExA(hKey, str_keyValueName.c_str(), 0, REG_SZ, (LPBYTE)str_keyValue.c_str(), cbData);

    RegCloseKey(hKey);

    return TRUE;
}
/// <summary>
/// 关闭烦人的安全通知
/// </summary>
/// <returns></returns>
BOOL OptimizationSecurityAndMaintenance(HKEY hkey, std::string str_subKey, std::string field)
{
    HKEY hKey;

    RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\Microsoft\Windows\CurrentVersion\Notifications\Settings\Windows.SystemToast.SecurityAndMaintenance", &hKey);

    RegDeleteValue(hKey, "Enabled");

    RegCloseKey(hKey);

    return TRUE;
}







int main(int argc, char* argv[])
{
    cout << "param count:" << argc << endl;

    std::string str_actionType = (char*)argv[1];

    if (str_actionType.find("MouseSpeedStop") == 0)                         //      关闭鼠标加速
    {
        cout << "关闭鼠标加速" << endl;
        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSpeed", "0");

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold1", "0");

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold2", "0");
    }
    else if(str_actionType.find("MouseSpeedStart") == 0)                    //      开启鼠标加速
    {
        cout << "开启鼠标加速" << endl;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSpeed", "1");

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold1", "50", 3);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold2", "80", 2);         
    }
    else
    {

    }

    return 0;
}
