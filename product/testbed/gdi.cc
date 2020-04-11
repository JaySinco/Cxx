#include "common/utility/base.h"
#include <iomanip>
#include <iostream>
#include <map>
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace cxx;

#define ID_CHECKBTN 0
#define ID_CHECKBOX 1

struct {
    int iStyle;
    TCHAR* szText;
    int id;
} gBtnList[] = {
    { BS_CHECKBOX, TEXT("CHECKBOX"), ID_CHECKBOX },
    { BS_PUSHBUTTON, TEXT("CHECK"), ID_CHECKBTN },
};

LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int cxChar = LOWORD(GetDialogBaseUnits());
    static int cyChar = HIWORD(GetDialogBaseUnits());
    switch (message) {
    case WM_CREATE: {
        int i = 0;
        for (const auto& ctl : gBtnList) {
            HWND hbtn = CreateWindow(TEXT("button"), ctl.szText,
                WS_CHILD | WS_VISIBLE | ctl.iStyle,
                0, cyChar * (2 + 2 * i),
                20 * cxChar, 7 * cyChar / 4,
                hwnd, (HMENU)ctl.id,
                ((LPCREATESTRUCT)lParam)->hInstance, NULL);
            SendMessage(hbtn, WM_SETFONT, (LPARAM)GetStockObject(SYSTEM_FIXED_FONT), true);
            ++i;
        }
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
        TCHAR szBuffer[10];
        TextOut(hdc, 0, 0, szBuffer, wsprintf(szBuffer, TEXT("%s"), TEXT("最是流年似旧曲")));
        TextOut(hdc, 0, cyChar, szBuffer, wsprintf(szBuffer, TEXT("%s"), TEXT("暮雨纷纷又清明")));
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DRAWITEM:
    case WM_COMMAND: {
        std::cout << (message == WM_DRAWITEM ? "WM_DRAWITEM" : "WM_COMMAND") << " [W]0x"
                  << std::hex << std::setw(4) << std::setfill('0') << wParam << " [L]0x"
                  << std::hex << std::setw(4) << std::setfill('0') << lParam
                  << std::endl;
        int id = LOWORD(wParam);
        if (id == ID_CHECKBTN) {
            HWND hbox = GetDlgItem(hwnd, ID_CHECKBOX);
            int iCheck = SendMessage(hbox, BM_GETCHECK, NULL, NULL);
            SendMessage(hbox, BM_SETCHECK, (WPARAM)!iCheck, NULL);
            SetWindowText(GetDlgItem(hwnd, ID_CHECKBTN), iCheck ? TEXT("CHECK") : TEXT("UNCHECK"));
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    AttachConsole(ATTACH_PARENT_PROCESS);
    FILE* pf;
    freopen_s(&pf, "CONIN$", "r+t", stdin);
    freopen_s(&pf, "CONOUT$", "w+t", stdout);
    std::cout << std::endl;

    static TCHAR szAppName[] = TEXT("GDI_t");
    WNDCLASS wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = szAppName;
    wc.lpfnWndProc = windowProc;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    UnregisterClass(wc.lpszClassName, GetModuleHandle(NULL));
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, szAppName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}