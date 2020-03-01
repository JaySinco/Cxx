#define OEMRESOURCE
#include <windows.h>
#include <atomic>
#include <future>
#include <map>
#include <thread>
#include "common/debugging/print.h"
#define RESOURCE(relpath) "D:\\Jaysinco\\Cxx\\product\\testbed\\resources\\"##relpath
#define LOG_LAST_ERROR(msg) LOG(ERROR) << msg << ", errno=" << GetLastError()
// 变通方法:
// 1. 其他WS_EX_TOPMOST窗口无法隐藏 => select去掉回显窗口
// 2. CTRL-ALT-DEL无法鼠标HOOK => 注册表可以禁用
// 3. 右键菜单无法隐藏 => 不断置顶窗口
// 4. 鼠标无法隐藏 => 更换系统鼠标图标

class AntiViewScreen
{
public:
    static void Lock(
        const std::string &password = "",
        bool blockMouseInput = true,
        bool blockKeyboardInput = true);

private:
    class ReplaceSystemCursor
    {
    public:
        ReplaceSystemCursor(const std::string &iconFile);
        ~ReplaceSystemCursor();

    private:
        std::map<int, HCURSOR> oldSystemCursor;
    };

    static void initScreen();
    static void updateScreen();
    static void DrawRectBorder(HDC hdc, int left, int top, int right, int bottom);
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK mouseProc(INT nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK keyboardProc(INT nCode, WPARAM wParam, LPARAM lParam);

    static HWND hwnd;
    static bool blockMouse;
    static bool blockKeyboard;
    static HHOOK mouseHook;
    static HHOOK keyboardHook;
    static std::atomic<bool> done;
    static int screenWidth;
    static int screenHeight;
    static std::string inputPwd;
    static std::string unlockCode;
};

HWND AntiViewScreen::hwnd;
bool AntiViewScreen::blockMouse;
bool AntiViewScreen::blockKeyboard;
HHOOK AntiViewScreen::mouseHook;
HHOOK AntiViewScreen::keyboardHook;
std::atomic<bool> AntiViewScreen::done;
int AntiViewScreen::screenWidth;
int AntiViewScreen::screenHeight;
std::string AntiViewScreen::inputPwd;
std::string AntiViewScreen::unlockCode;

void AntiViewScreen::Lock(
    const std::string &password,
    bool blockMouseInput,
    bool blockKeyboardInput)
{
    done = false;
    unlockCode = password;
    inputPwd.clear();
    blockMouse = blockMouseInput;
    blockKeyboard = blockKeyboardInput;
    LOG(INFO) << "lock anti-view screen...";
    ReplaceSystemCursor cursorGuard(RESOURCE("point.cur"));
    initScreen();
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);
    if (mouseHook == NULL)
    {
        LOG_LAST_ERROR("failed to set mouse hook");
    }
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, GetModuleHandle(NULL), 0);
    if (keyboardHook == NULL)
    {
        LOG_LAST_ERROR("failed to set keyboard hook");
    }
    updateScreen();
    auto background = std::async(std::launch::async, [] {
        while (!done)
        {
            BringWindowToTop(hwnd);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        LOG(INFO) << "background task stopped!";
    });
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    done = true;
    if (!UnhookWindowsHookEx(mouseHook))
    {
        LOG_LAST_ERROR("failed to unset mouse hook");
    }
    if (!UnhookWindowsHookEx(keyboardHook))
    {
        LOG_LAST_ERROR("failed to unset keyboard hook");
    }
    DestroyWindow(hwnd);
    background.wait();
    LOG(INFO) << "unlock anti-view screen!";
}

void AntiViewScreen::initScreen()
{
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    WNDCLASS wc = {0};
    wc.lpszClassName = TEXT("AntiViewScreen");
    wc.lpfnWndProc = windowProc;
    DWORD r, g, b;
    sscanf_s("#fffaf0", "#%2X%2X%2X", &r, &g, &b);
    wc.hbrBackground = CreateSolidBrush(RGB(r, g, b));
    UnregisterClass(wc.lpszClassName, GetModuleHandle(NULL));
    RegisterClass(&wc);

    hwnd = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        wc.lpszClassName, // Window class
        TEXT(""),         // Window text
        WS_POPUP,         // Window style
        // Size and position
        0, 0, screenWidth, screenHeight,
        NULL,                  // Parent window
        NULL,                  // Menu
        GetModuleHandle(NULL), // Instance handle
        NULL                   // Additional application data
    );
    // SetLayeredWindowAttributes(hwnd, NULL, BYTE(255 * alpha), LWA_ALPHA);
}

void AntiViewScreen::updateScreen()
{
    InvalidateRect(hwnd, NULL, true);
    // 背景
    RECT region;
    GetClientRect(hwnd, &region);
    SIZE area = {region.right - region.left, region.bottom - region.top};
    HDC hdc = GetDC(hwnd);
    HDC hBitmapDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = (HBITMAP)LoadImage(
        NULL, TEXT(RESOURCE("background.bmp")),
        IMAGE_BITMAP, screenWidth, screenHeight,
        LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    if (hBitmap == NULL)
    {
        LOG_LAST_ERROR("failed to load bitmap");
    }
    SelectObject(hBitmapDC, hBitmap);
    // 绘图
    const int fontHeight = screenWidth / 42;
    HFONT hFont = CreateFontW(
        fontHeight,            // height
        0,                     // width
        0,                     // escapenment,
        0,                     // orientation,
        600,                   // weight,
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
    const int penWidth = 5;
    HPEN hPen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hBitmapDC, hPen);
    HBRUSH hBrush = CreateSolidBrush(color);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hBitmapDC, hBrush);
    DrawRectBorder(
        hBitmapDC,
        int(screenWidth / 2.5),
        screenHeight / 2 - penWidth,
        int(screenWidth / 1.5),
        screenHeight / 2 + fontHeight + penWidth / 2);
    std::wstring context = L"请输入密码: ";
    SetBkColor(hBitmapDC, color);
    TextOutW(hBitmapDC, screenWidth / 4, screenHeight / 2, context.c_str(), int(context.size()));
    TextOutA(hBitmapDC, int(screenWidth / 2.5) + penWidth, screenHeight / 2,
             inputPwd.c_str(), int(inputPwd.size()));
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
    UpdateLayeredWindow(hwnd, hdc, NULL, &area, hBitmapDC, &pt, 0, &bf, ULW_ALPHA);
    DeleteObject(hBitmap);
    DeleteDC(hBitmapDC);
    ReleaseDC(hwnd, hdc);
    ShowWindow(hwnd, SW_SHOW);
}

void AntiViewScreen::DrawRectBorder(HDC hdc, int left, int top, int right, int bottom)
{
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, top);
    LineTo(hdc, right, bottom);
    LineTo(hdc, left, bottom);
    LineTo(hdc, left, top);
}

LRESULT CALLBACK AntiViewScreen::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        ReleaseDC(hwnd, hdc);
        return 0;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK AntiViewScreen::mouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
    // don't intercept when p->flags=true
    if (nCode < 0 || p->flags)
    {
        return CallNextHookEx(mouseHook, nCode, wParam, lParam);
    }
    VLOG(1) << "mouse: x=" << p->pt.x << ", y=" << p->pt.y;
    // if (wParam == WM_MOUSEMOVE || wParam == WM_LBUTTONUP || wParam == WM_RBUTTONUP)
    // {
    // }
    return blockMouse ? 1 : 0;
}

LRESULT CALLBACK AntiViewScreen::keyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
    // don't intercept when p->flags=true
    if (nCode < 0 || p->flags)
    {
        return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
    }
    VLOG(1) << "keyboard: vk=" << p->vkCode << ", scan=" << p->scanCode;
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        switch (p->vkCode)
        {
        case VK_BACK:
            if (inputPwd.size() > 0)
            {
                inputPwd.pop_back();
                updateScreen();
            }
            break;
        case VK_RETURN:
            if (inputPwd == unlockCode)
            {
                PostMessage(hwnd, WM_QUIT, 0, NULL);
            }
            else
            {
                inputPwd.clear();
                updateScreen();
            }
            break;
        default:
            if (char c = MapVirtualKeyA(p->vkCode, MAPVK_VK_TO_CHAR))
            {
                inputPwd += c;
                updateScreen();
            }
            break;
        }
    }
    return blockKeyboard ? 1 : 0;
}

AntiViewScreen::ReplaceSystemCursor::ReplaceSystemCursor(const std::string &iconFile)
{
    HCURSOR hUserCursor = LoadCursorFromFileA(iconFile.c_str());
    if (hUserCursor == NULL)
    {
        LOG_LAST_ERROR("failed to load cursor, path=" << iconFile);
        return;
    }
    int allCursorId[] = {
        OCR_APPSTARTING,
        OCR_NORMAL,
        OCR_CROSS,
        OCR_HAND,
        OCR_IBEAM,
        OCR_NO,
        OCR_SIZEALL,
        OCR_SIZENESW,
        OCR_SIZENS,
        OCR_SIZENWSE,
        OCR_SIZEWE,
        OCR_UP,
        OCR_WAIT,
    };
    for (auto id : allCursorId)
    {
        HCURSOR hOldCursor = CopyCursor(LoadCursor(NULL, MAKEINTRESOURCE(id)));
        if (hOldCursor == NULL)
        {
            LOG_LAST_ERROR("failed to copy cursor, id=" << id);
            continue;
        }
        if (SetSystemCursor(CopyCursor(hUserCursor), id))
        {
            oldSystemCursor[id] = hOldCursor;
        }
        else
        {
            LOG_LAST_ERROR("failed to replace system cursor, id=" << id);
        }
    }
    if (!DestroyCursor(hUserCursor))
    {
        LOG_LAST_ERROR("failed to destroy user cursor");
    }
}

AntiViewScreen::ReplaceSystemCursor::~ReplaceSystemCursor()
{
    for (const auto &it : oldSystemCursor)
    {
        if (!SetSystemCursor(it.second, it.first))
        {
            LOG_LAST_ERROR("failed to restore system cursor, id=" << it.first);
        }
    }
}

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    if (!SetProcessDPIAware())
    {
        LOG_LAST_ERROR("failed to set process dpi aware");
    }
    AntiViewScreen::Lock("", false, false);
}