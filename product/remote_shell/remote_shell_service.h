#pragma once
#include "gen-cpp/RemoteShellService.h"

namespace cxx
{

namespace service
{

class RemoteShellService : public RemoteShellServiceIf
{
public:
    RemoteShellService();
    ~RemoteShellService() override;
    void GetComputerInfo(ComputerInfo &_return) override;
    void GetCpuInfo(CpuInfo &_return) override;
    void GetMemoryInfo(MemoryInfo &_return) override;
    void GetDiskInfo(DiskInfo &_return) override;
    void GetNetworkInfo(NetworkInfo &_return) override;
    void Execute(ShellRtn &_return, const std::string &cmdWithArgs) override;
};

} // namespace service
} // namespace cxx