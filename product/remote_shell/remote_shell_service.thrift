namespace cpp cxx.service

struct ComputerInfo {
    1: string osName
    2: string userName
}

struct CpuInfo {
    1: double rate  // <1
}

struct MemoryInfo {
    1: double total // MB
    2: double used  // MB
}

struct DiskInfo {
    1: double total  // GB
    2: double free   // GB
    3: double readSpeed  // KB/s
    4: double writeSpeed // KB/s
}

struct NetworkInfo {
    1: double downloadSpeed // KB/s
    2: double uploadSpeed   // KB/s
}

struct ShellRtn {
    1: bool success
    2: string standardOutput
}

service RemoteShellService {
    ComputerInfo GetComputerInfo()
    CpuInfo GetCpuInfo()
    MemoryInfo GetMemoryInfo()
    DiskInfo GetDiskInfo()
    NetworkInfo GetNetworkInfo()
    ShellRtn Execute(1: string cmdWithArgs)
}
