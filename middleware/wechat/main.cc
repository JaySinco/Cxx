#define UNICODE
#include <windows.h>
#include <TlHelp32.h>
#include <direct.h>
#include <sstream>
#include <locale>
#include <iostream>
#include <thread>
#include <vector>
#include "message.h"
#define QUOT(x) (std::string("\"")+x+"\"")

DWORD findPidByProcessName(const wchar_t* processName) {
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 pe32 = { 0 };
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hProcess, &pe32) == TRUE) {
        do {
            if (wcscmp(processName, pe32.szExeFile) == 0) {
                return pe32.th32ProcessID;
            }
        } while (Process32Next(hProcess, &pe32));
    }
    return 0;
}

bool isInjected(DWORD dwProcessid) {
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessid);
    if (hModuleSnap == INVALID_HANDLE_VALUE) {
        std::cout << "can't create snap shot!" << std::endl;
        return false;
    }
    if (!Module32First(hModuleSnap, &me32)) {
        std::cout << "can't get first module!" << std::endl;
        CloseHandle(hModuleSnap);
        return false;
    }
    do {
        if (wcscmp(me32.szModule, L"WeChatHelper.dll") == 0)
            return true;
    } while (Module32Next(hModuleSnap, &me32));
    return false;
}

DWORD openWechat() {
    HKEY hKEY;
	LPCTSTR data_set = L"Software\\Tencent\\WeChat";
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, data_set, 0, KEY_READ, &hKEY)) {
		wchar_t dwValue[MAX_PATH];
		DWORD dwSize = MAX_PATH;
		DWORD dwType = REG_SZ;
		if (ERROR_SUCCESS != RegQueryValueEx(hKEY, L"InstallPath", 0, &dwType, (LPBYTE)&dwValue, &dwSize)) {
			std::cout << "failed to query wechat install path from regedit" << std::endl;
            return NULL;
		}
		else {
            std::wstring wxpath = dwValue;
            wxpath.append(L"\\WeChat.exe");
            std::wcout << L"wxpath=\"" << wxpath << L"\"" << std::endl;
			STARTUPINFO si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            si.dwFlags = STARTF_USESHOWWINDOW;
            si.wShowWindow = TRUE;
            CreateProcess(wxpath.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
            HWND hWechatMainForm = NULL;
            while (NULL == hWechatMainForm) {
                hWechatMainForm = FindWindow(L"WeChatLoginWndForPC", NULL);
                Sleep(500);
            }
            return pi.dwProcessId;
		}
	}
	RegCloseKey(hKEY);
}

bool injectDll() {
    char currentPath[_MAX_PATH];
    _getcwd(currentPath, _MAX_PATH);
    std::ostringstream dllpathStream;
    dllpathStream << currentPath << "\\" << "WeChatHelper.dll";
    std::string dllpath = dllpathStream.str();
    std::cout << "dllpath=" << QUOT(dllpath) << std::endl;
    DWORD dwPid = findPidByProcessName(L"WeChat.exe");
    if (dwPid == 0) {
        dwPid = openWechat();
    }
    if (!isInjected(dwPid)) {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
        if (hProcess == NULL) {
            std::cout << "failed to OpenProcess!" << std::endl;
            return false;
        }
        LPVOID pAddress = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (pAddress == NULL) {
            std::cout << "failed to VirtualAllocEx!" << std::endl;
            return false;
        }
        if (WriteProcessMemory(hProcess, pAddress, dllpath.c_str(), MAX_PATH, NULL) == 0) {
            std::cout << "failed to WriteProcessMemory!" << std::endl;
            return false;
        }
        FARPROC pLoadLibraryAddress = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        if (pLoadLibraryAddress == NULL) {
            std::cout << "failed to GetProcAddress for LoadLibraryA!" << std::endl;
            return false;
        }
        HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibraryAddress, pAddress, 0, NULL);
        if (hRemoteThread == NULL) {
            std::cout << "failed to CreateRemoteThread!" << std::endl;
            return false;
        }
        CloseHandle(hRemoteThread);
        CloseHandle(hProcess);
    }
    else {
        std::cout << "dll already injected!" << std::endl;
        return true;
    }
    return true;
}

struct UserInfo
{
	wchar_t UserId[80];
	wchar_t UserNumber[80];
	wchar_t UserRemark[80];
	wchar_t UserNickName[80];
};

bool gAlreadyLogin = false;
bool gJustLogin = false;
std::vector<UserInfo> gUserInfo;
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    if (Message == WM_COPYDATA) {
        COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
        switch (pCopyData->dwData) {
        case WM_Login:
            gJustLogin = true;
            break;
        case WM_AlreadyLogin:
            gAlreadyLogin = true;
            break;
        case WM_GetFriendList:
            gUserInfo.push_back(*(UserInfo*)pCopyData->lpData);
            break;
        }
    }
    return DefWindowProc(hWnd, Message, wParam, lParam);
}

void createWechatWindow(const std::wstring &title) {
    WNDCLASS wnd;
    wnd.style = CS_VREDRAW | CS_HREDRAW;
    wnd.lpfnWndProc = WndProc;
    wnd.cbClsExtra = NULL;
    wnd.cbWndExtra = NULL;
    wnd.hInstance = GetModuleHandle(NULL);
    wnd.hIcon = NULL;
    wnd.hCursor = NULL;
    wnd.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wnd.lpszMenuName = NULL;
    wnd.lpszClassName = L"WX_HELPER";
    RegisterClass(&wnd);
    HWND hWnd = CreateWindow(
        wnd.lpszClassName,
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        10, 10, 500, 300, 
        NULL,             
        NULL,          
        GetModuleHandle(NULL),       
        NULL            
    );
    ShowWindow(hWnd, SW_HIDE);
    UpdateWindow(hWnd);
    MSG  msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int main(int argc, char *argv[]) {
    std::locale::global(std::locale(""));
    std::wcout << L"wcout中文测试" << std::endl;
    std::thread login(createWechatWindow, L"Login");
    std::thread helper(createWechatWindow, L"微信助手");
    std::cout << injectDll();
    while (!gJustLogin && !gAlreadyLogin) {
        Sleep(500);
    }
    if (gAlreadyLogin) {
        std::cout << "already login!" << std::endl;
        exit(-1);
    }
    Sleep(3000);
    for (const auto &user: gUserInfo) {
        std::wcout << "friend: " << user.UserNickName << std::endl;
    }
    login.join();
    helper.join();
    /*auto pWnd = FindWindow(NULL, L"WeChatHelper");
    std::cout << pWnd;
    COPYDATASTRUCT logout;
    logout.dwData = WM_Logout;
    logout.cbData = 0;
    logout.lpData = NULL;
    SendMessage(pWnd, WM_COPYDATA, NULL, (LPARAM)&logout);*/
    return 0;
}
