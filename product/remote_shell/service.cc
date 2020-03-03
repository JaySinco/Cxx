#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <comutil.h>
#include <Lmcons.h>
#include <WtsApi32.h>
#include <Pdh.h>
#include <array>
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include "service.h"
#define MB (1024 * 1024)
#define GB (1024 * 1024 * 1024)

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
        LOG(ERROR) << "failed to exec GetUserName:" << GetLastError();
        return "unknow";
    }
}

double GetCPURate()
{
    HQUERY query;
    PDH_STATUS status = PdhOpenQuery(NULL, NULL, &query);
    HCOUNTER counter = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));
    std::string counterPath = "\\Processor Information(_Total)\\% Processor Time";
    status = PdhAddCounterA(query, LPCSTR(counterPath.c_str()), NULL, &counter);
    PDH_FMT_COUNTERVALUE pdhValue;
    DWORD dwValue;
    PdhCollectQueryData(query);
    Sleep(100);
    PdhCollectQueryData(query);
    status = PdhGetFormattedCounterValue(
        counter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &dwValue, &pdhValue);
    PdhCloseQuery(query);
    return pdhValue.doubleValue;
}

void GetMemoryStat(cxx::service::MemoryInfo &info)
{
    MEMORYSTATUSEX mStatus;
    mStatus.dwLength = sizeof(mStatus);
    GlobalMemoryStatusEx(&mStatus);
    info.total = double(mStatus.ullTotalPhys) / GB;
    info.used = double(mStatus.ullTotalPhys - mStatus.ullAvailPhys) / GB;
}

double CalculateIOSpeedInPdh(const char *counterPath)
{
    HQUERY query;
    PDH_STATUS status;
    status = PdhOpenQuery(NULL, NULL, &query);
    HCOUNTER counter;
    counter = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));
    status = PdhAddCounterA(query, LPCSTR(counterPath), NULL, &counter);
    PdhCollectQueryData(query);
    Sleep(100);
    PdhCollectQueryData(query);
    PDH_FMT_COUNTERVALUE pdhValue;
    DWORD dwValue;
    status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
    PdhCloseQuery(query);
    double speed = pdhValue.doubleValue;
    return speed;
}

void GetDiskStat(cxx::service::DiskInfo &info, const std::string &driver)
{
    DWORDLONG freeBytesAvailable = 0;
    DWORDLONG totalNumberOfBytes = 0;
    DWORDLONG totalNumberOfFreeBytes = 0;
    GetDiskFreeSpaceExA(
        LPCSTR(driver.c_str()),
        (PULARGE_INTEGER)&freeBytesAvailable,
        (PULARGE_INTEGER)&totalNumberOfBytes,
        (PULARGE_INTEGER)&totalNumberOfFreeBytes);
    info.total = double(totalNumberOfBytes) / GB;
    info.free = double(totalNumberOfFreeBytes) / GB;
    std::stringstream ss1;
    ss1 << "\\PhysicalDisk(_Total)\\Disk Read Bytes/sec";
    std::string readSpeedCounterPath = ss1.str();
    double readSpeed = CalculateIOSpeedInPdh(readSpeedCounterPath.c_str());
    info.readSpeed = readSpeed / MB;
    std::stringstream ss2;
    ss2 << "\\PhysicalDisk(_Total)\\Disk Write Bytes/sec";
    std::string writeSpeedCounterPath = ss2.str();
    double writeSpeed = CalculateIOSpeedInPdh(writeSpeedCounterPath.c_str());
    info.writeSpeed = writeSpeed / MB;
}

void GetNetworkStat(cxx::service::NetworkInfo &info)
{
    std::string readSpeedCounterPath = "\\Network Interface(*)\\Bytes Received/sec";
    double readSpeed = CalculateIOSpeedInPdh(readSpeedCounterPath.c_str());
    info.downloadSpeed = readSpeed / MB;
    std::string writeSpeedCounterPath = "\\Network Interface(*)\\Bytes Sent/sec";
    double writeSpeed = CalculateIOSpeedInPdh(writeSpeedCounterPath.c_str());
    info.uploadSpeed = writeSpeed / MB;
}

void Exec(cxx::service::ShellRtn &rtn, const char *cmd)
{
    std::array<char, 128> buffer;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe)
    {
        LOG(ERROR) << "failed to popen command: " << cmd;
        rtn.success = false;
        return;
    }
    while (fgets(buffer.data(), int(buffer.size()), pipe.get()) != nullptr)
    {
        rtn.standardOutput += buffer.data();
    }
    rtn.success = true;
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
    _return.rate = GetCPURate();
}

void RemoteShellService::GetMemoryInfo(MemoryInfo &_return)
{
    GetMemoryStat(_return);
}

void RemoteShellService::GetDiskInfo(DiskInfo &_return, const std::string &driver)
{
    GetDiskStat(_return, driver);
}

void RemoteShellService::GetNetworkInfo(NetworkInfo &_return)
{
    GetNetworkStat(_return);
}

void RemoteShellService::Execute(ShellRtn &_return, const std::string &cmdWithArgs)
{
    Exec(_return, cmdWithArgs.c_str());
}

} // namespace service
} // namespace cxx