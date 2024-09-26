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
BOOL OptimizationMouseSpeed(HKEY h_rootKey, std::string str_subKey, std::string str_keyValueName, DWORD d_reserved, BYTE* byte_keyValue, DWORD cbData = 1)
{
    HKEY hKey;
    //      关闭鼠标加速
    RegCreateKeyExA(h_rootKey, str_subKey.c_str(), 0, 0, 0, 0x20106, 0, &hKey, 0);

    RegSetValueExA(hKey, str_keyValueName.c_str(), 0, d_reserved, byte_keyValue, cbData);

    RegCloseKey(hKey);

    return TRUE;
}
/// <summary>
/// 关闭烦人的安全通知
/// </summary>
/// <returns></returns>
BOOL OptimizationSecurityAndMaintenance(HKEY _hkey, std::string str_subKey, LPCSTR lpValueName = "Enabled")
{
    HKEY hKey;
    
    RegOpenKeyEx(_hkey, str_subKey.c_str(), 0, 0x20106, &hKey);

    RegDeleteValue(hKey, lpValueName);

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

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSpeed", REG_SZ, (LPBYTE)"0");

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold1", REG_SZ, (LPBYTE)"0");

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold2", REG_SZ, (LPBYTE)"0");
    }
    else if(str_actionType.find("MouseSpeedStart") == 0)                    //      开启鼠标加速
    {
        cout << "开启鼠标加速" << endl;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseSpeed", REG_SZ, (LPBYTE)"1");

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold1", REG_SZ, (LPBYTE)"50", 3);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, "Control Panel\\Mouse", "MouseThreshold2", REG_SZ, (LPBYTE)"80", 2);
    }
    else if (str_actionType.find("SecurityAndMaintenanceStop") == 0)                    //      关闭烦人的安全通知
    {
        //      首先判断指定字段、注册表路径是否存在
        //      不存在需要创建

        DWORD value = 1;

        DWORD value0 = 0;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Notifications\Settings\Windows.SystemToast.SecurityAndMaintenance)", "Enabled", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Notifications\Settings\Windows.SystemToast.SecurityAndMaintenance)", "Enabled", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Policies\Microsoft\Windows Defender Security Center\Notifications)", "DisableNotifications", REG_DWORD, reinterpret_cast<BYTE*>(&value), 4);
        
        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows Defender Security Center\Notifications)", "DisableNotifications", REG_DWORD, reinterpret_cast<BYTE*>(&value), 4);
    }
    else if (str_actionType.find("SecurityAndMaintenanceStart") == 0)                    //      开启烦人的安全通知
    {
        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Notifications\Settings\Windows.SystemToast.SecurityAndMaintenance)");
        
        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Notifications\Settings\Windows.SystemToast.SecurityAndMaintenance)");

        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Policies\Microsoft\Windows Defender Security Center\Notifications)", "DisableNotifications");

        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows Defender Security Center\Notifications)", "DisableNotifications");
    }
    else if (str_actionType.find("DriverSearchingStop") == 0)                           //      关闭Windows自动更新程序
    {
        DWORD value3 = 3;

        DWORD value1 = 1;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\DriverSearching)", "SearchOrderConfig", REG_DWORD, reinterpret_cast<BYTE*>(&value3), 4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate)", "ExcludeWUDriversInQualityUpdate", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);
    }
    else if (str_actionType.find("DriverSearchingStart") == 0)
    {
        DWORD value1 = 1;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\DriverSearching)", "SearchOrderConfig", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate)", "ExcludeWUDriversInQualityUpdate");
    }
    else if (str_actionType.find("BackgroundAccessApplicationsStart") == 0)              //          禁用UWP程序后台
    {
        DWORD value1 = 1;

        DWORD value0 = 0;

        DWORD value4 = 4;
        //      Software\Microsoft\Windows\CurrentVersion\BackgroundAccessApplications
        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\BackgroundAccessApplications)", "GlobalUserDisabled", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\Search)", "BackgroundAppGlobalToggle", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Services\embeddedmode)", "Start", REG_DWORD, reinterpret_cast<BYTE*>(&value4), 4);
    }
    else if (str_actionType.find("BackgroundAccessApplicationsStop") == 0)             //          取消禁用UWP程序后台          
    {
        DWORD value3 = 3;

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\BackgroundAccessApplications)", "GlobalUserDisabled");

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(Software\Microsoft\Windows\CurrentVersion\Search)", "BackgroundAppGlobalToggle");

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Services\embeddedmode)", "Start", REG_DWORD, reinterpret_cast<BYTE*>(&value3), 4);
    }
    else if (str_actionType.find("MapStop") == 0)             //          关闭自动更新地图
    {
        DWORD value0 = 0;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SYSTEM\Maps)", "AutoUpdateEnabled", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);
    }
    else if (str_actionType.find("MapStart") == 0)             //          开启自动更新地图
    {
        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"(SYSTEM\Maps)", "AutoUpdateEnabled");
    }
    else
    {

    }

    return 0;
}
