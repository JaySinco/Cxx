#include <windows.h>
#include "common/debugging/print.h"
#define RESOURCE(relpath) "D:\\Jaysinco\\Cxx\\product\\testbed\\resources\\"##relpath

// 缺陷：
// 1. 其他WS_EX_TOPMOST窗口无法隐藏 => select去掉回显窗口
// 2. CTRL-ALT-DEL无法鼠标HOOK => 注册表可以禁用
// 3. 鼠标无法隐藏
// 4. 右键菜单无法隐藏

HWND gWindow = NULL;
HHOOK gMouse = NULL;
HHOOK gKeyboard = NULL;
const int gWidth = GetSystemMetrics(SM_CXSCREEN);
const int gHeight = GetSystemMetrics(SM_CYSCREEN);
std::string gPassword;
std::string gStoredPwd = "JAYSINCO";

void DrawRectBorder(HDC hdc, int left, int top, int right, int bottom)
{
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, top);
    LineTo(hdc, right, bottom);
    LineTo(hdc, left, bottom);
    LineTo(hdc, left, top);
}

void UpdateWindow()
{
    ShowCursor(FALSE);
    InvalidateRect(gWindow, NULL, true);
    // 背景
    RECT region;
    GetClientRect(gWindow, &region);
    SIZE area = {region.right - region.left, region.bottom - region.top};
    HDC hdc = GetDC(gWindow);
    HDC hBitmapDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = (HBITMAP)LoadImage(
        NULL, TEXT(RESOURCE("background.bmp")),
        IMAGE_BITMAP, gWidth, gHeight,
        LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    if (hBitmap == NULL)
    {
        LOG(ERROR) << "failed to load bitmap, errno=" << GetLastError();
        std::exit(0);
    }
    SelectObject(hBitmapDC, hBitmap);
    // 绘图
    const int fontHeight = gWidth / 42;
    HFONT hFont = CreateFontW(
        fontHeight,            // height
        0,                     // width
        0,                     // escapenment,
        0,                     // orientation,
        0,                     // weight,
        FALSE,                 // italic,
        FALSE,                 // underline,
        FALSE,                 // strikeOut,
        DEFAULT_CHARSET,       // charset,
        OUT_CHARACTER_PRECIS,  // outPrecision,
        CLIP_CHARACTER_PRECIS, // clipPrecision,
        DEFAULT_QUALITY,       // quality,
        FF_DONTCARE,           // pitchAndFamily,
        L"华文细黑"            // facename
    );
    COLORREF color = RGB(9, 163, 153);
    HFONT hOldFont = (HFONT)SelectObject(hBitmapDC, hFont);
    const int penWidth = 3;
    HPEN hPen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hBitmapDC, hPen);
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hBitmapDC, hBrush);
    DrawRectBorder(
        hBitmapDC,
        int(gWidth / 2.5),
        gHeight / 2 - penWidth,
        int(gWidth / 1.5),
        gHeight / 2 + fontHeight + penWidth / 2);
    std::wstring context = L"请输入密码: ";
    SetBkColor(hBitmapDC, color);
    TextOutW(hBitmapDC, gWidth / 4, gHeight / 2, context.c_str(), int(context.size()));
    TextOutA(hBitmapDC, int(gWidth / 2.5) + penWidth, gHeight / 2,
             gPassword.c_str(), int(gPassword.size()));
    SelectObject(hBitmapDC, hOldBrush);
    SelectObject(hBitmapDC, hOldPen);
    SelectObject(hBitmapDC, hOldFont);
    DeleteObject(hBrush);
    DeleteObject(hPen);
    DeleteObject(hFont);
    // 混合
    BLENDFUNCTION bf;
    bf.AlphaFormat = NULL; // AC_SRC_ALPHA
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 255;
    POINT pt = {0, 0};
    UpdateLayeredWindow(gWindow, hdc, NULL, &area, hBitmapDC, &pt, 0, &bf, ULW_ALPHA);
    DeleteObject(hBitmap);
    DeleteDC(hBitmapDC);
    ReleaseDC(gWindow, hdc);
    ShowWindow(gWindow, SW_SHOW);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        VLOG(1) << "WM_PAINT reviced!";
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        ReleaseDC(hwnd, hdc);
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

    HWND win = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        wc.lpszClassName, // Window class
        TEXT(""),         // Window text
        WS_POPUP,         // Window style
        // Size and position
        0, 0, gWidth, gHeight,
        NULL,                  // Parent window
        NULL,                  // Menu
        GetModuleHandle(NULL), // Instance handle
        NULL                   // Additional application data
    );
    if (alpha >= 0)
    {
        SetLayeredWindowAttributes(win, NULL, BYTE(255 * alpha), LWA_ALPHA);
    }
    return win;
}

LRESULT CALLBACK MouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
    // SendInput发送的消息p->flags为true，不做任何处理
    if (nCode < 0 || p->flags)
    {
        return CallNextHookEx(gMouse, nCode, wParam, lParam);
    }
    VLOG(1) << "mouse: x=" << p->pt.x << ", y=" << p->pt.y;
    // if (wParam == WM_MOUSEMOVE || wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP)
    // {
    // }
    return 1;
}

LRESULT CALLBACK KeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
    if (nCode < 0 || p->flags)
    {
        return CallNextHookEx(gKeyboard, nCode, wParam, lParam);
    }
    VLOG(1) << "keyboard: vk=" << p->vkCode << ", scan=" << p->scanCode;
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        switch (p->vkCode)
        {
        case VK_ESCAPE:
            PostMessage(gWindow, WM_QUIT, 0, NULL);
            break;
        case VK_BACK:
            if (gPassword.size() > 0)
            {
                gPassword.pop_back();
                UpdateWindow();
            }
            break;
        case VK_RETURN:
            if (gPassword == gStoredPwd)
            {
                PostMessage(gWindow, WM_QUIT, 0, NULL);
            }
            else
            {
                gPassword.clear();
                UpdateWindow();
            }
            break;
        default:
            if (char c = MapVirtualKeyA(p->vkCode, MAPVK_VK_TO_CHAR))
            {
                gPassword += c;
                UpdateWindow();
            }
            break;
        }
    }
    return 1;
}

void CreateMessageLoop()
{
    LOG(INFO) << "start message loop...";
    gWindow = CreateScreenWindow("#000000", -1);
    UpdateWindow();
    gMouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);
    gKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(gMouse);
    UnhookWindowsHookEx(gKeyboard);
    DestroyWindow(gWindow);
    LOG(INFO) << "message loop end!";
}

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    CreateMessageLoop();
}