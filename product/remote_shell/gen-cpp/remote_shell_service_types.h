/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef remote_shell_service_TYPES_H
#define remote_shell_service_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <thrift/stdcxx.h>


namespace cxx { namespace service {

class ComputerInfo;

class CpuInfo;

class MemoryInfo;

class DiskInfo;

class NetworkInfo;

class ShellRtn;

typedef struct _ComputerInfo__isset {
  _ComputerInfo__isset() : osName(false), userName(false) {}
  bool osName :1;
  bool userName :1;
} _ComputerInfo__isset;

class ComputerInfo : public virtual ::apache::thrift::TBase {
 public:

  ComputerInfo(const ComputerInfo&);
  ComputerInfo& operator=(const ComputerInfo&);
  ComputerInfo() : osName(), userName() {
  }

  virtual ~ComputerInfo() throw();
  std::string osName;
  std::string userName;

  _ComputerInfo__isset __isset;

  void __set_osName(const std::string& val);

  void __set_userName(const std::string& val);

  bool operator == (const ComputerInfo & rhs) const
  {
    if (!(osName == rhs.osName))
      return false;
    if (!(userName == rhs.userName))
      return false;
    return true;
  }
  bool operator != (const ComputerInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ComputerInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(ComputerInfo &a, ComputerInfo &b);

std::ostream& operator<<(std::ostream& out, const ComputerInfo& obj);

typedef struct _CpuInfo__isset {
  _CpuInfo__isset() : rate(false) {}
  bool rate :1;
} _CpuInfo__isset;

class CpuInfo : public virtual ::apache::thrift::TBase {
 public:

  CpuInfo(const CpuInfo&);
  CpuInfo& operator=(const CpuInfo&);
  CpuInfo() : rate(0) {
  }

  virtual ~CpuInfo() throw();
  double rate;

  _CpuInfo__isset __isset;

  void __set_rate(const double val);

  bool operator == (const CpuInfo & rhs) const
  {
    if (!(rate == rhs.rate))
      return false;
    return true;
  }
  bool operator != (const CpuInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CpuInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(CpuInfo &a, CpuInfo &b);

std::ostream& operator<<(std::ostream& out, const CpuInfo& obj);

typedef struct _MemoryInfo__isset {
  _MemoryInfo__isset() : total(false), used(false) {}
  bool total :1;
  bool used :1;
} _MemoryInfo__isset;

class MemoryInfo : public virtual ::apache::thrift::TBase {
 public:

  MemoryInfo(const MemoryInfo&);
  MemoryInfo& operator=(const MemoryInfo&);
  MemoryInfo() : total(0), used(0) {
  }

  virtual ~MemoryInfo() throw();
  double total;
  double used;

  _MemoryInfo__isset __isset;

  void __set_total(const double val);

  void __set_used(const double val);

  bool operator == (const MemoryInfo & rhs) const
  {
    if (!(total == rhs.total))
      return false;
    if (!(used == rhs.used))
      return false;
    return true;
  }
  bool operator != (const MemoryInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MemoryInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(MemoryInfo &a, MemoryInfo &b);

std::ostream& operator<<(std::ostream& out, const MemoryInfo& obj);

typedef struct _DiskInfo__isset {
  _DiskInfo__isset() : total(false), free(false), readSpeed(false), writeSpeed(false) {}
  bool total :1;
  bool free :1;
  bool readSpeed :1;
  bool writeSpeed :1;
} _DiskInfo__isset;

class DiskInfo : public virtual ::apache::thrift::TBase {
 public:

  DiskInfo(const DiskInfo&);
  DiskInfo& operator=(const DiskInfo&);
  DiskInfo() : total(0), free(0), readSpeed(0), writeSpeed(0) {
  }

  virtual ~DiskInfo() throw();
  double total;
  double free;
  double readSpeed;
  double writeSpeed;

  _DiskInfo__isset __isset;

  void __set_total(const double val);

  void __set_free(const double val);

  void __set_readSpeed(const double val);

  void __set_writeSpeed(const double val);

  bool operator == (const DiskInfo & rhs) const
  {
    if (!(total == rhs.total))
      return false;
    if (!(free == rhs.free))
      return false;
    if (!(readSpeed == rhs.readSpeed))
      return false;
    if (!(writeSpeed == rhs.writeSpeed))
      return false;
    return true;
  }
  bool operator != (const DiskInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DiskInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(DiskInfo &a, DiskInfo &b);

std::ostream& operator<<(std::ostream& out, const DiskInfo& obj);

typedef struct _NetworkInfo__isset {
  _NetworkInfo__isset() : downloadSpeed(false), uploadSpeed(false) {}
  bool downloadSpeed :1;
  bool uploadSpeed :1;
} _NetworkInfo__isset;

class NetworkInfo : public virtual ::apache::thrift::TBase {
 public:

  NetworkInfo(const NetworkInfo&);
  NetworkInfo& operator=(const NetworkInfo&);
  NetworkInfo() : downloadSpeed(0), uploadSpeed(0) {
  }

  virtual ~NetworkInfo() throw();
  double downloadSpeed;
  double uploadSpeed;

  _NetworkInfo__isset __isset;

  void __set_downloadSpeed(const double val);

  void __set_uploadSpeed(const double val);

  bool operator == (const NetworkInfo & rhs) const
  {
    if (!(downloadSpeed == rhs.downloadSpeed))
      return false;
    if (!(uploadSpeed == rhs.uploadSpeed))
      return false;
    return true;
  }
  bool operator != (const NetworkInfo &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const NetworkInfo & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(NetworkInfo &a, NetworkInfo &b);

std::ostream& operator<<(std::ostream& out, const NetworkInfo& obj);

typedef struct _ShellRtn__isset {
  _ShellRtn__isset() : success(false), standardOutput(false) {}
  bool success :1;
  bool standardOutput :1;
} _ShellRtn__isset;

class ShellRtn : public virtual ::apache::thrift::TBase {
 public:

  ShellRtn(const ShellRtn&);
  ShellRtn& operator=(const ShellRtn&);
  ShellRtn() : success(0), standardOutput() {
  }

  virtual ~ShellRtn() throw();
  bool success;
  std::string standardOutput;

  _ShellRtn__isset __isset;

  void __set_success(const bool val);

  void __set_standardOutput(const std::string& val);

  bool operator == (const ShellRtn & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    if (!(standardOutput == rhs.standardOutput))
      return false;
    return true;
  }
  bool operator != (const ShellRtn &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ShellRtn & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(ShellRtn &a, ShellRtn &b);

std::ostream& operator<<(std::ostream& out, const ShellRtn& obj);

}} // namespace

#endif
