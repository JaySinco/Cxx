#include <iostream>
#include <iomanip>
#include "system_info.h"

using namespace cxx;

int main(int argc, char **argv)
{
	std::cout << "MemoryTotal\t\t" << WinInfoUtility::GetMemoryTotal() << "MB" << std::endl;
	std::cout << "MemoryAvailable\t\t" << WinInfoUtility::GetMemoryAvailable() << "MB" << std::endl;
	std::cout << "MemoryUsed\t\t" << WinInfoUtility::GetMemoryUsed() << "MB" << std::endl;
	std::cout << "MemoryLoad\t\t" << WinInfoUtility::GetMemoryLoad() << "%" << std::endl;
	std::cout << "DriveTotal/c\t\t" << WinInfoUtility::GetDriveTotalSize("c:/") << "GB" << std::endl;
	std::cout << "DriveFree/c\t\t" << WinInfoUtility::GetDriveFreeSize("c:/") << "GB" << std::endl;
	std::cout << "DriveUsed/c\t\t" << WinInfoUtility::GetDriveUsedSize("c:/") << "GB" << std::endl;
	std::cout << "DriveTotal/d\t\t" << WinInfoUtility::GetDriveTotalSize("d:/") << "GB" << std::endl;
	std::cout << "DriveFree/d\t\t" << WinInfoUtility::GetDriveFreeSize("d:/") << "GB" << std::endl;
	std::cout << "DriveUsed/d\t\t" << WinInfoUtility::GetDriveUsedSize("d:/") << "GB" << std::endl;
	std::cout << "DiskRead\t\t" << WinInfoUtility::GetDiskReadSpeed() << "Kb/s" << std::endl;
	std::cout << "DiskWrite\t\t" << WinInfoUtility::GetDiskWriteSpeed() << "Kb/s" << std::endl;
	std::cout << "CPURate\t\t\t" << WinInfoUtility::GetCPURate() << "%" << std::endl;
	std::cout << "NetDownload\t\t" << WinInfoUtility::GetNetDownloadSpeed() << "Kb/s" << std::endl;
	std::cout << "NetUpload\t\t" << WinInfoUtility::GetNetUploadSpeed() << "Kb/s" << std::endl;
	std::cout << "Dpi\t\t\t" << WinInfoUtility::GetDPI() << std::endl;
	std::pair<int, int> resolution = WinInfoUtility::GetResolution();
	std::cout << "Resolution\t\t" << resolution.first << "x" << resolution.second << std::endl;

    return 0;
}