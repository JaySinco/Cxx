#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <comutil.h>
#include <Lmcons.h>
#include <WtsApi32.h>
#include "common/log/logging.h"
#include "remote_shell_service.h"

namespace
{
std::string GetOsName()
{
    std::string osName = "unknow";
    OSVERSIONINFO osver = {sizeof(OSVERSIONINFO)};
    if (GetVersionEx(&osver))
    {
        if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
            osName = "Windows 2000";
        else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
            osName = "Windows XP";
        else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
            osName = "Windows 2003";
        else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
            osName = "windows vista";
        else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
            osName = "windows 7";
        else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
            osName = "Windows 10";
    }
    return osName;
}

std::string GetCurrentUserName()
{
    DWORD maxLen = UNLEN + 1;
    char username[UNLEN + 1];
    if (GetUserNameA(username, &maxLen))
    {
        return username;
    }
    else
    {
        LOG(ERROR) << "GetUserName failed, error=" << GetLastError();
        return "unknow";
    }
}

} // namespace

namespace cxx
{

namespace service
{

RemoteShellService::RemoteShellService()
{
}

RemoteShellService::~RemoteShellService()
{
}

void RemoteShellService::GetComputerInfo(ComputerInfo &_return)
{
    _return.osName = GetOsName();
    _return.userName = GetCurrentUserName();
}

void RemoteShellService::GetCpuInfo(CpuInfo &_return)
{
}

void RemoteShellService::GetMemoryInfo(MemoryInfo &_return)
{
}

void RemoteShellService::GetDiskInfo(DiskInfo &_return)
{
}

void RemoteShellService::GetNetworkInfo(NetworkInfo &_return)
{
}

void RemoteShellService::Execute(ShellRtn &_return, const std::string &cmdWithArgs)
{
}

} // namespace service
} // namespace cxx