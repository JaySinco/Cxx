#include "sysinfo_utility_win.h"

#include <iostream>
#include <iomanip>

int main(int argc, char **argv)
{
    std::cout << "/*----" << std::setw(30) << "       Memory info in MB "
              << "-----*/" << std::endl;
    std::cout << std::left << std::setw(10) << "M_total" << std::left << std::setw(10) << "M_free" << std::left << std::setw(10) << "M_used" << std::left << std::setw(10) << "M_load" << std::endl;
    uint64_t memory_total = WinInfoUtility::GetMemoryTotal();
    uint64_t memory_free = WinInfoUtility::GetMemoryAvailable();
    uint64_t memory_used = WinInfoUtility::GetMemoryUsed();
    uint64_t memory_load = WinInfoUtility::GetMemoryLoad();
    std::cout << std::left << std::setw(10) << memory_total << std::left << std::setw(10) << memory_free << std::left << std::setw(10) << memory_used << std::left << std::setw(10) << memory_load << "\n"
              << std::endl;

    std::cout << "/*----" << std::setw(30) << "       Disk info  "
              << "-----*/" << std::endl;
    std::cout << std::left << std::setw(10) << "C_total(GB)" << std::left << std::setw(10) << "C_free" << std::left << std::setw(10) << "C_used" << std::left << std::setw(15) << "C_read(Kb/s)" << std::left << std::setw(15) << "C_write" << std::endl;
    uint64_t disk_total = WinInfoUtility::GetDriveTotalSize();
    uint64_t disk_free = WinInfoUtility::GetDriveFreeSize();
    uint64_t disk_used = WinInfoUtility::GetDriveUsedSize();
    double disk_read = WinInfoUtility::GetDiskReadSpeed();
    double disk_write = WinInfoUtility::GetDiskWriteSpeed();
    std::cout << std::left << std::setw(10) << disk_total << std::left << std::setw(10) << disk_free << std::left << std::setw(10) << disk_used << std::left << std::setw(15) << disk_read << std::left << std::setw(15) << disk_write << "\n"
              << std::endl;

    std::cout << std::left << std::setw(10) << "D_total(GB)" << std::left << std::setw(10) << "D_free" << std::left << std::setw(10) << "D_used" << std::left << std::setw(15) << "D_read(Kb/s)" << std::left << std::setw(15) << "D_write" << std::endl;
    disk_total = WinInfoUtility::GetDriveTotalSize("d:/");
    disk_free = WinInfoUtility::GetDriveFreeSize("d:/");
    disk_used = WinInfoUtility::GetDriveUsedSize("d:/");
    disk_read = WinInfoUtility::GetDiskReadSpeed();
    disk_write = WinInfoUtility::GetDiskWriteSpeed();
    std::cout << std::left << std::setw(10) << disk_total << std::left << std::setw(10) << disk_free << std::left << std::setw(10) << disk_used << std::left << std::setw(15) << disk_read << std::left << std::setw(15) << disk_write << "\n"
              << std::endl;

    std::cout << "/*----" << std::setw(30) << "       CPU info  "
              << "-----*/" << std::endl;
    std::cout << std::left << std::setw(10) << "CPU Rate %" << std::endl;
    uint64_t cpu_rate = WinInfoUtility::GetCPURate();
    std::cout << std::left << std::setw(10) << cpu_rate << "\n"
              << std::endl;

    std::cout << "/*----" << std::setw(30) << "       Net info  "
              << "-----*/" << std::endl;
    std::cout << std::left << std::setw(15) << "Download(Kb/s)" << std::left << std::setw(15) << "Upload" << std::endl;
    double download_speed = WinInfoUtility::GetNetDownloadSpeed();
    double upload_speed = WinInfoUtility::GetNetUploadSpeed();
    std::cout << std::left << std::setw(15) << download_speed << std::left << std::setw(15) << upload_speed << "\n"
              << std::endl;

    std::cout << "/*----" << std::setw(30) << "       Screen info  "
              << "-----*/" << std::endl;
    std::cout << "dpi: " << WinInfoUtility::GetDPI() << std::endl;
    std::pair<int, int> resolution = WinInfoUtility::GetResolution();
    std::cout << "resolution: " << resolution.first << "*" << resolution.second << "\n"
              << std::endl;

    return 0;
}