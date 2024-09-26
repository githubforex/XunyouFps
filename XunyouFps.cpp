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
    else if (str_actionType.find("WindowsStoreStop") == 0)             //          禁用自动更新商店
    {
        DWORD value2 = 2;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Policies\Microsoft\WindowsStore)", "AutoDownload", REG_DWORD, reinterpret_cast<BYTE*>(&value2), 4);
    }
    else if (str_actionType.find("WindowsStoreStart") == 0)             //          自动更新商店
    {
        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Policies\Microsoft\WindowsStore)", "AutoDownload");
    }
    else if (str_actionType.find("PersonalizeStop") == 0)             //          禁用Windows透明度
    {
        DWORD value0 = 0;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize)", "EnableTransparency", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);
        }
    else if (str_actionType.find("PersonalizeStart") == 0)             //          使用Windows透明度
    {
        DWORD value1 = 1;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Themes\Personalize)", "EnableTransparency", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);
    }
    else if (str_actionType.find("VisualEffectsStop") == 0)             //          Windows视觉效果
    {
        DWORD value1 = 1;

        DWORD value0 = 0;

        DWORD value2 = 0x2;

        BYTE bData[0x24] = { 0x24, 0x00, 0x00, 0x00,
                            0x3E, 0xA8, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x01, 0x00, 0x00, 0x00,
                            0x13, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x73, 0x00, 0x00, 0x00 };

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "TaskbarAnimations", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "IconsOnly", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "ListviewShadow", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "ListviewAlphaSelect", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Control Panel\Desktop\WindowMetrics)", "MinAnimate", REG_SZ, (LPBYTE)"0", 1);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Control Panel\Desktop)", "DragFullWindows", REG_SZ, (LPBYTE)"0", 1);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\DWM)", "EnableAeroPeek", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Control Panel\Desktop)", "FontSmoothing", REG_SZ, (LPBYTE)"0", 1);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer)", "ShellState", REG_BINARY, bData, 0x24);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\VisualEffects)", "VisualFXSetting", REG_DWORD, reinterpret_cast<BYTE*>(&value2), 4);
    }
    else if (str_actionType.find("VisualEffectsStart") == 0)             //         Windows视觉效果
    {
        DWORD value1 = 1;

        DWORD value0 = 0;

        DWORD value24 = 0x24;

        BYTE bData[0x24] = { 0x24, 0x00, 0x00, 0x00,
                            0x37, 0x28, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x01, 0x00, 0x00, 0x00,
                            0x13, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00,
                            0x6A, 0x00, 0x00, 0x00 };

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "TaskbarAnimations", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "IconsOnly", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "ListviewShadow", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Advanced)", "ListviewAlphaSelect", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Control Panel\Desktop\WindowMetrics)", "MinAnimate", REG_SZ, (LPBYTE)"1", 1);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Control Panel\Desktop)", "DragFullWindows", REG_SZ, (LPBYTE)"1", 1);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\DWM)", "EnableAeroPeek", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(Control Panel\Desktop)", "FontSmoothing", REG_SZ, (LPBYTE)"1", 1);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer)", "ShellState", REG_BINARY, bData, 0x24);

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\VisualEffects)", "VisualFXSetting");
    }
    else
    {

    }

    return 0;
}
