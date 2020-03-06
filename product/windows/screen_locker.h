#pragma once
#define OEMRESOURCE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <atomic>
#include <map>
#define LOG_LAST_ERROR(msg) LOG(ERROR) << msg << ", errno=" << GetLastError()

class ScreenLocker
{
public:
    static void Popup(
        const std::string &password_ascii = "",
        const std::string &hintWord_u8 = u8"请输入密码: ",
        const std::string &backgroundFile_u8 = u8"background.bmp",
        const std::string &cursorFile_u8 = u8"point.cur",
        bool blockMouseInput = false,
        bool blockKeyboardInput = false,
        int msPinFreq = 500);

    static void Close();

private:
    class ReplaceSystemCursor
    {
    public:
        ReplaceSystemCursor(const std::wstring &iconFile);
        ~ReplaceSystemCursor();

    private:
        std::map<int, HCURSOR> oldSystemCursor;
    };

    static void initScreen();
    static void updateScreen();
    static void drawRectBorder(HDC hdc, int left, int top, int right, int bottom);
    static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK mouseProc(INT nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK keyboardProc(INT nCode, WPARAM wParam, LPARAM lParam);

    static HWND hwnd;
    static bool blockMouse;
    static bool blockKeyboard;
    static HHOOK mouseHook;
    static HHOOK keyboardHook;
    static int screenWidth;
    static int screenHeight;
    static std::string inputPwd;
    static std::string unlockCode;
    static std::wstring curFile;
    static std::wstring backgroundFile;
    static std::wstring hintWord;
    static std::atomic<bool> done;
};
