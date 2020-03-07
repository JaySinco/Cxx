#include "screen_locker.h"
#include <future>
#include <thread>
#include "common/utility/logging.h"
#include "common/utility/string_helper.h"

using namespace cxx;

HWND ScreenLocker::hwnd;
bool ScreenLocker::blockMouse;
bool ScreenLocker::blockKeyboard;
HHOOK ScreenLocker::mouseHook;
HHOOK ScreenLocker::keyboardHook;
int ScreenLocker::screenWidth;
int ScreenLocker::screenHeight;
std::string ScreenLocker::inputPwd;
std::string ScreenLocker::unlockCode;
std::wstring ScreenLocker::hintWord;
std::wstring ScreenLocker::curFile;
std::wstring ScreenLocker::backgroundFile;
std::atomic<bool> ScreenLocker::done = true;

void ScreenLocker::Popup(
    const std::string &password_ascii,
    const std::string &hintWord_u8,
    const std::string &backgroundFile_u8,
    const std::string &cursorFile_u8,
    bool blockMouseInput,
    bool blockKeyboardInput,
    int msPinFreq)
{
    bool expected = true;
    if (!done.compare_exchange_strong(expected, false))
    {
        LOG(ERROR) << "another screen locker is already running";
        return;
    }
    LOG(INFO) << "lock screen";
    unlockCode = password_ascii;
    inputPwd.clear();
    blockMouse = blockMouseInput;
    blockKeyboard = blockKeyboardInput;
    hintWord = decodeUtf8(hintWord_u8);
    backgroundFile = decodeUtf8(backgroundFile_u8);
    curFile = decodeUtf8(cursorFile_u8);
    VLOG(1) << "password=" << unlockCode;
    VLOG(1) << "hint=" << encodeAnsi(hintWord);
    VLOG(1) << "background=" << encodeAnsi(backgroundFile);
    VLOG(1) << "cursor=" << encodeAnsi(curFile);
    initScreen();
    ReplaceSystemCursor cursorGuard(curFile);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);
    if (mouseHook == NULL)
    {
        LOG_LAST_WIN_ERROR("failed to set mouse hook");
    }
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, GetModuleHandle(NULL), 0);
    if (keyboardHook == NULL)
    {
        LOG_LAST_WIN_ERROR("failed to set keyboard hook");
    }
    updateScreen();
    auto background = std::async(std::launch::async, [=] {
        while (!done)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(msPinFreq));
            BringWindowToTop(hwnd);
        }
        LOG(INFO) << "background task stopped";
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
        LOG_LAST_WIN_ERROR("failed to unset mouse hook");
    }
    if (!UnhookWindowsHookEx(keyboardHook))
    {
        LOG_LAST_WIN_ERROR("failed to unset keyboard hook");
    }
    DestroyWindow(hwnd);
    hwnd = NULL;
    background.wait();
    LOG(INFO) << "screen unlocked";
}

void ScreenLocker::Close()
{
    if (!done && hwnd != NULL)
    {
        PostMessage(hwnd, WM_QUIT, 0, NULL);
    }
}

void ScreenLocker::initScreen()
{
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);
    WNDCLASS wc = {0};
    wc.lpszClassName = TEXT("ScreenLocker");
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

void ScreenLocker::updateScreen()
{
    InvalidateRect(hwnd, NULL, true);
    // 背景
    RECT region;
    GetClientRect(hwnd, &region);
    SIZE area = {region.right - region.left, region.bottom - region.top};
    HDC hdc = GetDC(hwnd);
    HDC hBitmapDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = (HBITMAP)LoadImageW(
        NULL, backgroundFile.c_str(),
        IMAGE_BITMAP, screenWidth, screenHeight,
        LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
    if (hBitmap == NULL)
    {
        LOG_LAST_WIN_ERROR("failed to load bitmap");
        Close();
        return;
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
    drawRectBorder(
        hBitmapDC,
        int(screenWidth / 2.5),
        screenHeight / 2 - penWidth,
        int(screenWidth / 1.5),
        screenHeight / 2 + fontHeight + penWidth / 2);
    SetBkColor(hBitmapDC, color);
    TextOutW(
        hBitmapDC,
        screenWidth / 4,
        screenHeight / 2,
        hintWord.c_str(),
        int(hintWord.size()));
    TextOutA(
        hBitmapDC,
        int(screenWidth / 2.5) + penWidth,
        screenHeight / 2,
        inputPwd.c_str(),
        int(inputPwd.size()));
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

void ScreenLocker::drawRectBorder(HDC hdc, int left, int top, int right, int bottom)
{
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, top);
    LineTo(hdc, right, bottom);
    LineTo(hdc, left, bottom);
    LineTo(hdc, left, top);
}

LRESULT CALLBACK ScreenLocker::windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK ScreenLocker::mouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
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

LRESULT CALLBACK ScreenLocker::keyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
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
                Close();
            }
            else
            {
                inputPwd.clear();
                updateScreen();
            }
            break;
        default:
            char c = MapVirtualKeyA(p->vkCode, MAPVK_VK_TO_CHAR);
            if (c && isprint(c))
            {
                inputPwd += c;
                updateScreen();
            }
            break;
        }
    }
    return blockKeyboard ? 1 : 0;
}

ScreenLocker::ReplaceSystemCursor::ReplaceSystemCursor(const std::wstring &iconFile)
{
    HCURSOR hUserCursor = LoadCursorFromFileW(iconFile.c_str());
    if (hUserCursor == NULL)
    {
        LOG_LAST_WIN_ERROR("failed to load cursor");
        Close();
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
            LOG_LAST_WIN_ERROR("failed to copy cursor, id=" << id);
            continue;
        }
        if (SetSystemCursor(CopyCursor(hUserCursor), id))
        {
            oldSystemCursor[id] = hOldCursor;
        }
        else
        {
            LOG_LAST_WIN_ERROR("failed to replace system cursor, id=" << id);
        }
    }
    if (!DestroyCursor(hUserCursor))
    {
        LOG_LAST_WIN_ERROR("failed to destroy user cursor");
    }
}

ScreenLocker::ReplaceSystemCursor::~ReplaceSystemCursor()
{
    for (const auto &it : oldSystemCursor)
    {
        if (!SetSystemCursor(it.second, it.first))
        {
            LOG_LAST_WIN_ERROR("failed to restore system cursor, id=" << it.first);
        }
    }
}
