#ifndef SYSINFO_UTILITY_WIN_H
#define SYSINFO_UTILITY_WIN_H

#include <string>

using uint64_t = unsigned long long;

class WinInfoUtility
{

  public:
    //Memory
    static uint64_t GetMemoryTotal();
    static uint64_t GetMemoryAvailable();
    static uint64_t GetMemoryUsed();
    static uint64_t GetMemoryLoad();
    //Disk
    static uint64_t GetDriveTotalSize(const std::string &c = "c:/");
    static uint64_t GetDriveFreeSize(const std::string &c = "c:/");
    static uint64_t GetDriveUsedSize(const std::string &c = "c:/");
    static double GetDiskReadSpeed(const std::string &driver = "_Total");
    static double GetDiskWriteSpeed(const std::string &driver = "_Total");
    //CPU
    static int GetCPURate();
    //NetWork
    static double GetNetDownloadSpeed();
    static double GetNetUploadSpeed();
    // Screen
    static std::pair<int, int> GetResolution();
    static double GetDPI();

  private:
    static double CalculateIOSpeedInPdh(const char *CounterPath);
};

#endif