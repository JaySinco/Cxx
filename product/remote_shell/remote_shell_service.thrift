namespace cpp cxx.service

struct ComputerInfo {
    1: string osName
    2: string userName
}

struct CpuInfo {
    1: double rate  // <100
}

struct MemoryInfo {
    1: double total // GB
    2: double used  // GB
}

struct DiskInfo {
    1: double total  // GB
    2: double free   // GB
    3: double readSpeed  // MB/s
    4: double writeSpeed // MB/s
}

struct NetworkInfo {
    1: double downloadSpeed // MB/s
    2: double uploadSpeed   // MB/s
}

struct ShellRtn {
    1: bool success
    2: string standardOutput
}

service RemoteShellService {
    ComputerInfo GetComputerInfo()
    CpuInfo GetCpuInfo()
    MemoryInfo GetMemoryInfo()
    DiskInfo GetDiskInfo(1: string driver)
    NetworkInfo GetNetworkInfo()
    ShellRtn Execute(1: string cmdWithArgs)
}
