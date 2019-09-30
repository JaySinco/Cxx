#include <Windows.h>
#include <Pdh.h>
#include <sstream>
#include "system_info.h"

#define DIV 1024
#define INTERVAL 100

namespace cxx {

uint64_t WinInfoUtility::GetMemoryTotal() {
    MEMORYSTATUSEX mStatus;
    mStatus.dwLength = sizeof(mStatus);
    GlobalMemoryStatusEx(&mStatus);
    return mStatus.ullTotalPhys / DIV / DIV;
}

uint64_t WinInfoUtility::GetMemoryAvailable() {
    MEMORYSTATUSEX mStatus;
    mStatus.dwLength = sizeof(mStatus);
    GlobalMemoryStatusEx(&mStatus);
    return mStatus.ullAvailPhys / DIV / DIV;
}

uint64_t WinInfoUtility::GetMemoryUsed() {
    MEMORYSTATUSEX mStatus;
    mStatus.dwLength = sizeof(mStatus);
    GlobalMemoryStatusEx(&mStatus);
    return (mStatus.ullTotalPhys - mStatus.ullAvailPhys) / DIV / DIV;
}

uint64_t WinInfoUtility::GetMemoryLoad() {
    MEMORYSTATUSEX mStatus;
    mStatus.dwLength = sizeof(mStatus);
    GlobalMemoryStatusEx(&mStatus);
    return mStatus.dwMemoryLoad;
}

uint64_t WinInfoUtility::GetDriveTotalSize(const std::string &c) {
    DWORDLONG C_FreeBytesAvailable = 0;
    DWORDLONG C_TotalNumberOfBytes = 0;
    DWORDLONG C_TotalNumberOfFreeBytes = 0;
    GetDiskFreeSpaceEx(LPCSTR(c.c_str()), (PULARGE_INTEGER)&C_FreeBytesAvailable, (PULARGE_INTEGER)&C_TotalNumberOfBytes,
                                     (PULARGE_INTEGER)&C_TotalNumberOfFreeBytes);
    return C_TotalNumberOfBytes / DIV / DIV / DIV; // GB
}

uint64_t WinInfoUtility::GetDriveFreeSize(const std::string &c) {
    DWORDLONG C_FreeBytesAvailable = 0;
    DWORDLONG C_TotalNumberOfBytes = 0;
    DWORDLONG C_TotalNumberOfFreeBytes = 0;
    GetDiskFreeSpaceEx(LPCSTR(c.c_str()), (PULARGE_INTEGER)&C_FreeBytesAvailable, (PULARGE_INTEGER)&C_TotalNumberOfBytes,
                                     (PULARGE_INTEGER)&C_TotalNumberOfFreeBytes);
    return C_TotalNumberOfFreeBytes / DIV / DIV / DIV; // GB
}

uint64_t WinInfoUtility::GetDriveUsedSize(const std::string &c) {
    DWORDLONG C_FreeBytesAvailable = 0;
    DWORDLONG C_TotalNumberOfBytes = 0;
    DWORDLONG C_TotalNumberOfFreeBytes = 0;
    GetDiskFreeSpaceEx(LPCSTR(c.c_str()), (PULARGE_INTEGER)&C_FreeBytesAvailable, (PULARGE_INTEGER)&C_TotalNumberOfBytes,
                                     (PULARGE_INTEGER)&C_TotalNumberOfFreeBytes);
    return (C_TotalNumberOfBytes - C_TotalNumberOfFreeBytes) / DIV / DIV / DIV; // GB
}

double WinInfoUtility::CalculateIOSpeedInPdh(const char *CounterPath) {
    HQUERY query;
    PDH_STATUS status;
    status = PdhOpenQuery(NULL, NULL, &query);
    HCOUNTER counter;
    counter = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));
    status = PdhAddCounter(query, LPCSTR(CounterPath), NULL, &counter);
    PdhCollectQueryData(query);
    Sleep(INTERVAL);
    PdhCollectQueryData(query);
    PDH_FMT_COUNTERVALUE pdhValue;
    DWORD dwValue;
    status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, &dwValue, &pdhValue);
    PdhCloseQuery(query);
    double speed = pdhValue.doubleValue;
    return speed;
}

double WinInfoUtility::GetDiskReadSpeed(const std::string &driver) {
    std::stringstream ss1;
    ss1 << "\\PhysicalDisk(" << driver << ")\\Disk Read Bytes/sec";
    std::string ReadSpeedCounterPath = ss1.str();
    double ReadSpeed = CalculateIOSpeedInPdh(ReadSpeedCounterPath.c_str());
    return ReadSpeed / DIV; //'KB'
}

double WinInfoUtility::GetDiskWriteSpeed(const std::string &driver) {
    std::stringstream ss1;
    ss1 << "\\PhysicalDisk(" << driver << ")\\Disk Write Bytes/sec";
    std::string WriteSpeedCounterPath = ss1.str();
    double WriteSpeed = CalculateIOSpeedInPdh(WriteSpeedCounterPath.c_str());
    return WriteSpeed / DIV; //'KB'
}

int WinInfoUtility::GetCPURate() {
    HQUERY query;
    HCOUNTER counter;
    PDH_STATUS status;
    int CpuUsedRate;
    status = PdhOpenQuery(NULL, NULL, &query);
    counter = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));
    std::string counterPath = "\\Processor Information(_Total)\\% Processor Time";
    status = PdhAddCounter(query, LPCSTR(counterPath.c_str()), NULL, &counter);
    PDH_FMT_COUNTERVALUE pdhValue;
    DWORD dwValue;
    PdhCollectQueryData(query);
    Sleep(INTERVAL);
    PdhCollectQueryData(query);
    status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100, &dwValue, &pdhValue);
    CpuUsedRate = (int)pdhValue.doubleValue;
    PdhCloseQuery(query);
    return CpuUsedRate;
}

double WinInfoUtility::GetNetDownloadSpeed() {
    std::string ReadSpeedCounterPath = "\\Network Interface(*)\\Bytes Received/sec";
    double ReadSpeed = CalculateIOSpeedInPdh(ReadSpeedCounterPath.c_str());
    return ReadSpeed / DIV; //'KB'
}

double WinInfoUtility::GetNetUploadSpeed() {
    std::string WriteSpeedCounterPath = "\\Network Interface(*)\\Bytes Sent/sec";
    double WriteSpeed = CalculateIOSpeedInPdh(WriteSpeedCounterPath.c_str());
    return WriteSpeed / DIV; //'KB'
}

std::pair<int, int> WinInfoUtility::GetResolution() {
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    return std::pair<int, int>(width, height);
}

double WinInfoUtility::GetDPI() {
    double dpi = 0;
    HKEY hKEY;
    LPCTSTR data_set = "Control Panel\\Desktop\\WindowMetrics";
    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, data_set, 0, KEY_READ, &hKEY))
    {
        DWORD dwValue;
        DWORD dwSize = sizeof(DWORD);
        DWORD dwType = REG_DWORD;
        if (ERROR_SUCCESS == RegQueryValueEx(hKEY, "AppliedDPI", 0, &dwType, (LPBYTE)&dwValue, &dwSize))
        {
            dpi = dwValue;
        }
    }
    RegCloseKey(hKEY);
    return dpi;
}

}
