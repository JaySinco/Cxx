#include <windows.h>
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <thread>

HWND gWindow = NULL;
HHOOK gMouse = NULL;
HHOOK gKeyboard = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT: {
        VLOG(1) << "WM_PAINT reviced!";
        HDC hdc;
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        HPEN hNewPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hNewPen);
        MoveToEx(hdc, 0, 0, NULL);
        LineTo(hdc, 50, 50);
        SelectObject(hdc, hOldPen);
        DeleteObject(hOldPen);
        DeleteObject(hNewPen);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

HWND CreateScreenWindow(const std::string &color = "#fffaf0", const double alpha = 0.8)
{
    WNDCLASS wc = {0};
    wc.lpszClassName = TEXT("SCREEN_WINDOW");
    wc.lpfnWndProc = WindowProc;
    wc.hCursor = LoadCursor(NULL, IDC_HELP);
    DWORD r, g, b;
    sscanf_s(color.c_str(), "#%2X%2X%2X", &r, &g, &b);
    wc.hbrBackground = CreateSolidBrush(RGB(r, g, b));
    UnregisterClass(wc.lpszClassName, GetModuleHandle(NULL));
    RegisterClass(&wc);
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    HWND win = CreateWindowEx(WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
                             wc.lpszClassName,      // Window class
                             TEXT(""),              // Window text
                             WS_POPUP,              // Window style
                             // Size and position
                             0, 0, width/2, height,
                             NULL,                  // Parent window
                             NULL,                  // Menu
                             GetModuleHandle(NULL), // Instance handle
                             NULL                   // Additional application data
    );
    SetLayeredWindowAttributes(win, NULL, BYTE(255 * alpha), LWA_ALPHA);
    return win;
}

LRESULT CALLBACK MouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
    // sendInput发送的消息p->flags为true，不做任何处理   
    if (nCode < 0 || p->flags)
    {
        return CallNextHookEx(gMouse, nCode, wParam, lParam);
    }
    VLOG(1) << "mouse: x=" << p->pt.x << ", y=" << p->pt.y;
    if (wParam == WM_MOUSEMOVE)
    {
        return CallNextHookEx(gMouse, nCode, wParam, lParam);
    }

    if (wParam == WM_LBUTTONUP)
    {
        return CallNextHookEx(gMouse, nCode, wParam, lParam);
    }

    if (wParam == WM_RBUTTONUP)
    {
        return CallNextHookEx(gMouse, nCode, wParam, lParam);
    }
    return CallNextHookEx(gMouse, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
    if (nCode < 0 || p->flags)
    {
        return CallNextHookEx(gKeyboard, nCode, wParam, lParam);
    }
    VLOG(1) << "keyboard: vk=" << p->vkCode << ", scan=" << p->scanCode;
    // InvalidateRect(gWindow, NULL, TRUE);
    return CallNextHookEx(gKeyboard, nCode, wParam, lParam);
}

void CreateMessageLoop()
{
    LOG(INFO) << "start message loop...";
    gWindow = CreateScreenWindow();
    ShowWindow(gWindow, SW_SHOW);
    gMouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
    gKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))       
    {
        TranslateMessage (&msg);       
        DispatchMessage (&msg);
    } 
    UnhookWindowsHookEx(gMouse);
    UnhookWindowsHookEx(gKeyboard);
    DestroyWindow(gWindow);
}

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    std::thread t(CreateMessageLoop);
    // std::thread background([]{
    //     SetWindowPos(gWindow, HWND_TOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    // });
    // background.detach();
    t.join();
}
