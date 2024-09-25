#include <iostream>

#include <Windows.h>





int main()
{
    HKEY hKey;
    //      鼠标加速
    RegCreateKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
    //RegCreateKeyExA(HKEY_CURRENT_USER, "Control Panel\\Mouse", 0, 0, 0, 0x00020106, 0, &hKey, 0);
}
