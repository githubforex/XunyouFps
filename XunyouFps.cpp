#include <iostream>

#include <Windows.h>
#include <objbase.h>





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
/// <summary>
/// 
/// </summary>
/// <returns></returns>
BOOL OperateScrManagerWithoutCloseService(DWORD dwDesiredAccess= 0x000F003F, LPCSTR lpServiceName="dps", DWORD servicedAccess= SERVICE_CHANGE_CONFIG, DWORD dwServiceType= 0xFFFFFFFF, DWORD dwStartType= SERVICE_DISABLED)
{
    SC_HANDLE  hSCManager = OpenSCManagerA(NULL, NULL, dwDesiredAccess);

    if (!hSCManager)
    {
        return FALSE;
    }

    auto hService = OpenServiceA(hSCManager, lpServiceName, servicedAccess);            //      此处返回值有点奇怪，F12是SC_HANDLE，但是实际被识别出DWORD，可能是我的编译其报错，所以用auto替代

    if (!hService)
    {
        return FALSE;
    }

    auto result = ChangeServiceConfig((SC_HANDLE)hService, dwServiceType, dwStartType, 0xFFFFFFFF, 0, 0, 0, 0, 0, 0, 0);
    return TRUE;
}
/// <summary>
/// 
/// </summary>
/// <param name="dwCoInit"></param>
/// <returns></returns>
BOOL OperateCoApi(DWORD dwCoInit = 0x6)
{
    HRESULT hr;
    
    IUnknown* pUnknown = nullptr;

    CoInitializeEx(NULL, dwCoInit);

    //CoCreateInstance(CLSID_kuan, NULL, 0x1, IID_IUnknown, (void**)&pUnknown)
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
    else if (str_actionType.find("GameConfigStoreStop") == 0)             //          禁用全屏优化（FSP）
    {
        DWORD value2 = 2;

        DWORD value1 = 1;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(System\GameConfigStore)", "GameDVR_DXGIHonorFSEWindowsCompatible", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(System\GameConfigStore)", "GameDVR_FSEBehavior", REG_DWORD, reinterpret_cast<BYTE*>(&value2), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(System\GameConfigStore)", "GameDVR_HonorUserFSEBehaviorMode", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);
    }
    else if (str_actionType.find("GameConfigStoreStart") == 0)             //          全屏优化（FSP）
    {
        DWORD value0 = 0;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(System\GameConfigStore)", "GameDVR_DXGIHonorFSEWindowsCompatible", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(System\GameConfigStore)", "GameDVR_FSEBehavior");

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(System\GameConfigStore)", "GameDVR_HonorUserFSEBehaviorMode", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);
    }
    else if (str_actionType.find("GameBarStop") == 0)             //          关闭GameBar
    {
        DWORD value0 = 0;

        DWORD value3 = 3;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "AutoGameModeEnabled", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "GamePanelStartupTipIndex", REG_DWORD, reinterpret_cast<BYTE*>(&value3), 4);
        
        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "ShowStartupPanel", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);
        
        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "UseNexusForGameBarEnabled", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);
        
        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "AllowAutoGameMode", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\GameDVR)", "AppCaptureEnabled", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Policies\Microsoft\Windows\GameDVR)", "AllowGameDVR", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\PolicyManager\default\ApplicationManagement\AllowGameDVR)", "value", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 4);
    }
    else if (str_actionType.find("GameBarStart") == 0)             //          开启GameBar
    {
        DWORD value1 = 1;

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "AutoGameModeEnabled");

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "GamePanelStartupTipIndex");

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "ShowStartupPanel");

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "UseNexusForGameBarEnabled");

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\GameBar)", "AllowAutoGameMode");

        OptimizationSecurityAndMaintenance(HKEY_CURRENT_USER, R"("SOFTWARE\Microsoft\Windows\CurrentVersion\GameDVR)", "AppCaptureEnabled");

        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"("SOFTWARE\Policies\Microsoft\Windows\GameDVR)", "AllowGameDVR");

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\PolicyManager\default\ApplicationManagement\AllowGameDVR)", "value", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 4);
    }
    else if (str_actionType.find("OverlayTestModeStart") == 0)             //          关闭多平面叠加（MPO）
    {
        DWORD value5 = 5;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\Dwm)", "OverlayTestMode", REG_DWORD, reinterpret_cast<BYTE*>(&value5), 4);
    }
    else if (str_actionType.find("OverlayTestModeStop") == 0)             //          多平面叠加（MPO）
    {
        OptimizationSecurityAndMaintenance(HKEY_LOCAL_MACHINE, R"("SOFTWARE\Microsoft\Windows\Dwm)", "OverlayTestMode");
    }
    else if (str_actionType.find("WSearchStart") == 0)             //          关闭Windows索引
    {
        DWORD value4 = 4;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Services\WSearch)", "Start", REG_DWORD, reinterpret_cast<BYTE*>(&value4), 4);
    }
    else if (str_actionType.find("WSearchStop") == 0)             //          Windows索引
    {
        DWORD value2 = 2;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Services\WSearch)", "Start", REG_DWORD, reinterpret_cast<BYTE*>(&value2), 4);
    }
    else if (str_actionType.find("SysMainStart") == 0)             //          关闭Sysmain
    {
        DWORD value4 = 4;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Services\SysMain)", "Start", REG_DWORD, reinterpret_cast<BYTE*>(&value4), 4);
    }
    else if (str_actionType.find("SysMainStop") == 0)             //          开启Sysmain
    {
        DWORD value2 = 2;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SYSTEM\CurrentControlSet\Services\SysMain)", "Start", REG_DWORD, reinterpret_cast<BYTE*>(&value2), 4);
    }
    else if (str_actionType.find("DpsStart") == 0)             //          禁用Windows诊断
    {
        OperateScrManagerWithoutCloseService();
    }
    else if (str_actionType.find("DpsStop") == 0)             //          允许Windows诊断
    {
        OperateScrManagerWithoutCloseService(0x000F003F, "dps", SERVICE_CHANGE_CONFIG, 0xFFFFFFFF, SERVICE_AUTO_START);
    }
    else if (str_actionType.find("InstallUninstallLanguageStart") == 0)             //          安装语言
    {
        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\International\Synchronize Language Settings" /ENABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\LanguageComponentsInstaller\Installation" /ENABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\LanguageComponentsInstaller\ReconcileLanguageResources" /ENABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\LanguageComponentsInstaller\Uninstallation" /ENABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\MUI\LPRemove" /ENABLE)", NULL, NULL);
    }
    else if (str_actionType.find("InstallUninstallLanguageStop") == 0)             //           卸载语言
    {
        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\International\Synchronize Language Settings" /DISABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\LanguageComponentsInstaller\Installation" /DISABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\LanguageComponentsInstaller\ReconcileLanguageResources" /DISABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\LanguageComponentsInstaller\Uninstallation" /DISABLE)", NULL, NULL);

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\MUI\LPRemove" /DISABLE)", NULL, NULL);
    }
    else if (str_actionType.find("DeviceCensusStart") == 0)             //          网络摄像头遥测
    {
        std::string str_value = R"(%windir%\System32\taskkill.exe)";

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\Device Information\Device" /ENABLE)", NULL, NULL);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\DeviceCensus.exe)", "Debugger", REG_SZ, (LPBYTE)str_value.c_str(), 0x1E);
    }
    else if (str_actionType.find("DeviceCensusStop") == 0)             //           网络摄像头遥测
    {
        std::string str_value = R"(%windir%\System32\taskkill.exe)";

        ShellExecute(NULL, "open", "cmd.exe", R"(/c schtasks /Change /TN "\Microsoft\Windows\Device Information\Device" /DISABLE)", NULL, NULL);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\DeviceCensus.exe)", "Debugger", REG_SZ, (LPBYTE)str_value.c_str(), 0x1E);        
    }
    else if (str_actionType.find("DeviceCensusStart") == 0)             //          退出Windows隐私同意
    {
        DWORD   value0 = 0x0;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Personalization\Settings)", "AcceptedPrivacyPolicy", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 0x4);
    }
    else if (str_actionType.find("DeviceCensusStop") == 0)             //           退出Windows隐私同意
    {
        DWORD   value1 = 0x1;

        OptimizationMouseSpeed(HKEY_CURRENT_USER, R"(SOFTWARE\Microsoft\Personalization\Settings)", "AcceptedPrivacyPolicy", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 0x4);
    }
    else if (str_actionType.find("DeviceCensusStart") == 0)             //          禁用WiFi感知
    {
        DWORD   value0 = 0x0;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\PolicyManager\default\WiFi\AllowWiFiHotSpotReporting)", "value", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 0x4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\PolicyManager\default\WiFi\AllowAutoConnectToWiFiSenseHotspots)", "value", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 0x4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\WcmSvc\wifinetworkmanager\config)", "AutoConnectAllowedOEM", REG_DWORD, reinterpret_cast<BYTE*>(&value0), 0x4);
    }
    else if (str_actionType.find("DeviceCensusStop") == 0)             //           禁用WiFi感知
    {
        DWORD   value1 = 0x1;

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\PolicyManager\default\WiFi\AllowWiFiHotSpotReporting)", "value", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 0x4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\PolicyManager\default\WiFi\AllowAutoConnectToWiFiSenseHotspots)", "value", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 0x4);

        OptimizationMouseSpeed(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\WcmSvc\wifinetworkmanager\config)", "AutoConnectAllowedOEM", REG_DWORD, reinterpret_cast<BYTE*>(&value1), 0x4);
    }
    else
    {

    }

    return 0;
}
