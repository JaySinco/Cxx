#define UNICODE
#include <windows.h>
#include <TlHelp32.h>
#include <direct.h>
#include <sstream>
#include <iostream>
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

bool injectDll() {
    char currentPath[_MAX_PATH];
    _getcwd(currentPath, _MAX_PATH);
    std::ostringstream dllpathStream;
    dllpathStream << currentPath << "\\" << "WeChatHelper.dll";
    std::string dllpath = dllpathStream.str();
    std::cout << "dllpath=" << QUOT(dllpath) << std::endl;
    DWORD dwPid = findPidByProcessName(L"WeChat.exe");
    if (dwPid == 0) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = TRUE;
        CreateProcess(L"D:\\ProgramFiles\\WeChat\\WeChat.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        HWND hWechatMainForm = NULL;
        while (NULL == hWechatMainForm) {
            hWechatMainForm = FindWindow(L"WeChatLoginWndForPC", NULL);
            Sleep(500);
        }
        dwPid = pi.dwProcessId;
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

#define WM_AlreadyLogin 21
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    if (Message == WM_COPYDATA) {
        COPYDATASTRUCT *pCopyData = (COPYDATASTRUCT*)lParam;
        switch (pCopyData->dwData) {
        case WM_AlreadyLogin:
            std::cout << "already login!!";
            break;
        }
    }
    return DefWindowProc(hWnd, Message, wParam, lParam);
}

void RegisterWindow() {
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
    wnd.lpszClassName = L"LoginGGtt";
    RegisterClass(&wnd);
    HWND hWnd = CreateWindow(
        L"LoginGGtt",
        L"Login",
        WS_OVERLAPPEDWINDOW,
        10, 10, 500, 300, 
        NULL,             
        NULL,          
        GetModuleHandle(NULL),       
        NULL            
    );

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    MSG  msg = {};

    while (GetMessage(&msg, 0, 0, 0))
    {

        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }
}

#define WM_Logout 2
int main(int argc, char *argv[]) {
    std::cout << injectDll();
    //RegisterWindow();
    /*auto pWnd = FindWindow(NULL, L"WeChatHelper");
    std::cout << pWnd;
    COPYDATASTRUCT logout;
    logout.dwData = WM_Logout;
    logout.cbData = 0;
    logout.lpData = NULL;
    SendMessage(pWnd, WM_COPYDATA, NULL, (LPARAM)&logout);*/
    return 0;
}
