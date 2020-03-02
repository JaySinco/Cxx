#define OEMRESOURCE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <atomic>
#include <map>
#include <codecvt>
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
        bool blockKeyboardInput = false);

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
    static std::atomic<bool> done;
    static int screenWidth;
    static int screenHeight;
    static std::string inputPwd;
    static std::string unlockCode;
    static std::wstring curFile;
    static std::wstring backgroundFile;
    static std::wstring hintWord;
};

inline std::wstring utf8ToWstring(const std::string &str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> strConv;
    return strConv.from_bytes(str);
}

inline std::ostream &operator<<(std::ostream &out, const wchar_t *str)
{
    size_t len;
    _locale_t locale = _create_locale(LC_ALL, "chs");
    _wcstombs_s_l(&len, NULL, 0, str, _TRUNCATE, locale);
    char *buf = (char *)malloc(len + 1);
    buf[len] = 0;
    size_t converted;
    _wcstombs_s_l(&converted, buf, len, str, _TRUNCATE, locale);
    out << buf;
    free(buf);
    _free_locale(locale);
    return out;
}

inline std::ostream &operator<<(std::ostream &out, const std::wstring &str)
{
    return operator<<(out, str.c_str());
}