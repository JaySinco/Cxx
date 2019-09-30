#include <Windows.h>
#include <string>
#include <sstream>
#include <thread>

#include "common/utility/system_info.h"

#define RL(x) ((double)x * GetSystemMetrics(SM_CXFULLSCREEN) * gDPI)

double gDPI = cxx::WinInfoUtility::GetDPI() / 96;
HWND gWindow = NULL;
DWORD gMessageThreadId = -1;
HHOOK gMouseHook = NULL;

struct InfoBoard
{
    std::string mousePos;
} gBoard;

HWND createHintWindow(const std::string &classname, WNDPROC windowProc,
                        const std::string &color = "#FFFFFF", double alpha = 1.0f,
                        int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
                        int xPos = CW_USEDEFAULT, int yPos = CW_USEDEFAULT)
{
    WNDCLASS wc = {0};
    const char *clsname = classname.c_str();
    wc.lpszClassName = clsname;
    wc.lpfnWndProc = windowProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    DWORD r, g, b;
    sscanf_s(color.c_str(), "#%2X%2X%2X", &r, &g, &b);
    wc.hbrBackground = CreateSolidBrush(RGB(r, g, b));
    UnregisterClass(clsname, GetModuleHandle(NULL));
    RegisterClass(&wc);

    HWND handle = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_LAYERED, // Optional window styles.
        clsname,                          // Window class
        "",                               // Window caption
        WS_POPUP,                         // Window style
        xPos,                             // Postion x
        yPos,                             // Postion y
        width,                            // Width
        height,                           // Height
        NULL,                             // Parent window
        NULL,                             // Menu
        GetModuleHandle(NULL),            // Instance handle
        NULL                              // Additional application data
    );
    SetLayeredWindowAttributes(handle, NULL, 255 * alpha, LWA_ALPHA);
    ShowWindow(handle, SW_HIDE);
    return handle;
}

LRESULT CALLBACK textWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        HFONT hFont;
        hFont = CreateFontW(
            RL(0.008),             // height
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
            L"华文细黑"             // facename
        );
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
        TextOut(hdc, RL(0.0015), RL(0.0015), gBoard.mousePos.c_str(), gBoard.mousePos.size());
        SelectObject(hdc, oldFont);
        EndPaint(hwnd, &ps);
        return 0;
    }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK mouseProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
    if (!(nCode < 0 || p->flags) && wParam == WM_MOUSEMOVE)
    {
        int x = p->pt.x / gDPI + RL(0.005);
        int y = p->pt.y / gDPI + RL(0.008);
        std::ostringstream ss;
        ss << p->pt.x << "x" << p->pt.y;
        gBoard.mousePos = ss.str();
        InvalidateRect(gWindow, NULL, TRUE);
        SetWindowPos(gWindow, HWND_TOPMOST, x, y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
    }
    return CallNextHookEx(gMouseHook, nCode, wParam, lParam);
}

void createMessageLoop()
{
    gMessageThreadId = GetCurrentThreadId();
    gMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);

    gWindow = createHintWindow("hintWindow", textWindowProc, "#FFFFFF", 0.8, RL(0.038), RL(0.01));
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(gMouseHook);
}

int main()
{
    std::thread t(createMessageLoop);
    // Sleep(3000);
    // PostThreadMessage(gMessageThreadId, WM_QUIT, 0, NULL);
    t.join();
}