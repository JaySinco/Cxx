/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef RemoteShellService_H
#define RemoteShellService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "remote_shell_service_types.h"

namespace cxx { namespace service {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class RemoteShellServiceIf {
 public:
  virtual ~RemoteShellServiceIf() {}
  virtual void GetComputerInfo(ComputerInfo& _return) = 0;
  virtual void GetCpuInfo(CpuInfo& _return) = 0;
  virtual void GetMemoryInfo(MemoryInfo& _return) = 0;
  virtual void GetDiskInfo(DiskInfo& _return) = 0;
  virtual void GetNetworkInfo(NetworkInfo& _return) = 0;
  virtual void Execute(ShellRtn& _return, const std::string& cmdWithArgs) = 0;
};

class RemoteShellServiceIfFactory {
 public:
  typedef RemoteShellServiceIf Handler;

  virtual ~RemoteShellServiceIfFactory() {}

  virtual RemoteShellServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(RemoteShellServiceIf* /* handler */) = 0;
};

class RemoteShellServiceIfSingletonFactory : virtual public RemoteShellServiceIfFactory {
 public:
  RemoteShellServiceIfSingletonFactory(const ::apache::thrift::stdcxx::shared_ptr<RemoteShellServiceIf>& iface) : iface_(iface) {}
  virtual ~RemoteShellServiceIfSingletonFactory() {}

  virtual RemoteShellServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(RemoteShellServiceIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<RemoteShellServiceIf> iface_;
};

class RemoteShellServiceNull : virtual public RemoteShellServiceIf {
 public:
  virtual ~RemoteShellServiceNull() {}
  void GetComputerInfo(ComputerInfo& /* _return */) {
    return;
  }
  void GetCpuInfo(CpuInfo& /* _return */) {
    return;
  }
  void GetMemoryInfo(MemoryInfo& /* _return */) {
    return;
  }
  void GetDiskInfo(DiskInfo& /* _return */) {
    return;
  }
  void GetNetworkInfo(NetworkInfo& /* _return */) {
    return;
  }
  void Execute(ShellRtn& /* _return */, const std::string& /* cmdWithArgs */) {
    return;
  }
};


class RemoteShellService_GetComputerInfo_args {
 public:

  RemoteShellService_GetComputerInfo_args(const RemoteShellService_GetComputerInfo_args&);
  RemoteShellService_GetComputerInfo_args& operator=(const RemoteShellService_GetComputerInfo_args&);
  RemoteShellService_GetComputerInfo_args() {
  }

  virtual ~RemoteShellService_GetComputerInfo_args() throw();

  bool operator == (const RemoteShellService_GetComputerInfo_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteShellService_GetComputerInfo_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetComputerInfo_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteShellService_GetComputerInfo_pargs {
 public:


  virtual ~RemoteShellService_GetComputerInfo_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetComputerInfo_result__isset {
  _RemoteShellService_GetComputerInfo_result__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetComputerInfo_result__isset;

class RemoteShellService_GetComputerInfo_result {
 public:

  RemoteShellService_GetComputerInfo_result(const RemoteShellService_GetComputerInfo_result&);
  RemoteShellService_GetComputerInfo_result& operator=(const RemoteShellService_GetComputerInfo_result&);
  RemoteShellService_GetComputerInfo_result() {
  }

  virtual ~RemoteShellService_GetComputerInfo_result() throw();
  ComputerInfo success;

  _RemoteShellService_GetComputerInfo_result__isset __isset;

  void __set_success(const ComputerInfo& val);

  bool operator == (const RemoteShellService_GetComputerInfo_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteShellService_GetComputerInfo_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetComputerInfo_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetComputerInfo_presult__isset {
  _RemoteShellService_GetComputerInfo_presult__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetComputerInfo_presult__isset;

class RemoteShellService_GetComputerInfo_presult {
 public:


  virtual ~RemoteShellService_GetComputerInfo_presult() throw();
  ComputerInfo* success;

  _RemoteShellService_GetComputerInfo_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class RemoteShellService_GetCpuInfo_args {
 public:

  RemoteShellService_GetCpuInfo_args(const RemoteShellService_GetCpuInfo_args&);
  RemoteShellService_GetCpuInfo_args& operator=(const RemoteShellService_GetCpuInfo_args&);
  RemoteShellService_GetCpuInfo_args() {
  }

  virtual ~RemoteShellService_GetCpuInfo_args() throw();

  bool operator == (const RemoteShellService_GetCpuInfo_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteShellService_GetCpuInfo_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetCpuInfo_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteShellService_GetCpuInfo_pargs {
 public:


  virtual ~RemoteShellService_GetCpuInfo_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetCpuInfo_result__isset {
  _RemoteShellService_GetCpuInfo_result__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetCpuInfo_result__isset;

class RemoteShellService_GetCpuInfo_result {
 public:

  RemoteShellService_GetCpuInfo_result(const RemoteShellService_GetCpuInfo_result&);
  RemoteShellService_GetCpuInfo_result& operator=(const RemoteShellService_GetCpuInfo_result&);
  RemoteShellService_GetCpuInfo_result() {
  }

  virtual ~RemoteShellService_GetCpuInfo_result() throw();
  CpuInfo success;

  _RemoteShellService_GetCpuInfo_result__isset __isset;

  void __set_success(const CpuInfo& val);

  bool operator == (const RemoteShellService_GetCpuInfo_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteShellService_GetCpuInfo_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetCpuInfo_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetCpuInfo_presult__isset {
  _RemoteShellService_GetCpuInfo_presult__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetCpuInfo_presult__isset;

class RemoteShellService_GetCpuInfo_presult {
 public:


  virtual ~RemoteShellService_GetCpuInfo_presult() throw();
  CpuInfo* success;

  _RemoteShellService_GetCpuInfo_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class RemoteShellService_GetMemoryInfo_args {
 public:

  RemoteShellService_GetMemoryInfo_args(const RemoteShellService_GetMemoryInfo_args&);
  RemoteShellService_GetMemoryInfo_args& operator=(const RemoteShellService_GetMemoryInfo_args&);
  RemoteShellService_GetMemoryInfo_args() {
  }

  virtual ~RemoteShellService_GetMemoryInfo_args() throw();

  bool operator == (const RemoteShellService_GetMemoryInfo_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteShellService_GetMemoryInfo_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetMemoryInfo_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteShellService_GetMemoryInfo_pargs {
 public:


  virtual ~RemoteShellService_GetMemoryInfo_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetMemoryInfo_result__isset {
  _RemoteShellService_GetMemoryInfo_result__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetMemoryInfo_result__isset;

class RemoteShellService_GetMemoryInfo_result {
 public:

  RemoteShellService_GetMemoryInfo_result(const RemoteShellService_GetMemoryInfo_result&);
  RemoteShellService_GetMemoryInfo_result& operator=(const RemoteShellService_GetMemoryInfo_result&);
  RemoteShellService_GetMemoryInfo_result() {
  }

  virtual ~RemoteShellService_GetMemoryInfo_result() throw();
  MemoryInfo success;

  _RemoteShellService_GetMemoryInfo_result__isset __isset;

  void __set_success(const MemoryInfo& val);

  bool operator == (const RemoteShellService_GetMemoryInfo_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteShellService_GetMemoryInfo_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetMemoryInfo_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetMemoryInfo_presult__isset {
  _RemoteShellService_GetMemoryInfo_presult__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetMemoryInfo_presult__isset;

class RemoteShellService_GetMemoryInfo_presult {
 public:


  virtual ~RemoteShellService_GetMemoryInfo_presult() throw();
  MemoryInfo* success;

  _RemoteShellService_GetMemoryInfo_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class RemoteShellService_GetDiskInfo_args {
 public:

  RemoteShellService_GetDiskInfo_args(const RemoteShellService_GetDiskInfo_args&);
  RemoteShellService_GetDiskInfo_args& operator=(const RemoteShellService_GetDiskInfo_args&);
  RemoteShellService_GetDiskInfo_args() {
  }

  virtual ~RemoteShellService_GetDiskInfo_args() throw();

  bool operator == (const RemoteShellService_GetDiskInfo_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteShellService_GetDiskInfo_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetDiskInfo_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteShellService_GetDiskInfo_pargs {
 public:


  virtual ~RemoteShellService_GetDiskInfo_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetDiskInfo_result__isset {
  _RemoteShellService_GetDiskInfo_result__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetDiskInfo_result__isset;

class RemoteShellService_GetDiskInfo_result {
 public:

  RemoteShellService_GetDiskInfo_result(const RemoteShellService_GetDiskInfo_result&);
  RemoteShellService_GetDiskInfo_result& operator=(const RemoteShellService_GetDiskInfo_result&);
  RemoteShellService_GetDiskInfo_result() {
  }

  virtual ~RemoteShellService_GetDiskInfo_result() throw();
  DiskInfo success;

  _RemoteShellService_GetDiskInfo_result__isset __isset;

  void __set_success(const DiskInfo& val);

  bool operator == (const RemoteShellService_GetDiskInfo_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteShellService_GetDiskInfo_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetDiskInfo_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetDiskInfo_presult__isset {
  _RemoteShellService_GetDiskInfo_presult__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetDiskInfo_presult__isset;

class RemoteShellService_GetDiskInfo_presult {
 public:


  virtual ~RemoteShellService_GetDiskInfo_presult() throw();
  DiskInfo* success;

  _RemoteShellService_GetDiskInfo_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class RemoteShellService_GetNetworkInfo_args {
 public:

  RemoteShellService_GetNetworkInfo_args(const RemoteShellService_GetNetworkInfo_args&);
  RemoteShellService_GetNetworkInfo_args& operator=(const RemoteShellService_GetNetworkInfo_args&);
  RemoteShellService_GetNetworkInfo_args() {
  }

  virtual ~RemoteShellService_GetNetworkInfo_args() throw();

  bool operator == (const RemoteShellService_GetNetworkInfo_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const RemoteShellService_GetNetworkInfo_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetNetworkInfo_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteShellService_GetNetworkInfo_pargs {
 public:


  virtual ~RemoteShellService_GetNetworkInfo_pargs() throw();

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetNetworkInfo_result__isset {
  _RemoteShellService_GetNetworkInfo_result__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetNetworkInfo_result__isset;

class RemoteShellService_GetNetworkInfo_result {
 public:

  RemoteShellService_GetNetworkInfo_result(const RemoteShellService_GetNetworkInfo_result&);
  RemoteShellService_GetNetworkInfo_result& operator=(const RemoteShellService_GetNetworkInfo_result&);
  RemoteShellService_GetNetworkInfo_result() {
  }

  virtual ~RemoteShellService_GetNetworkInfo_result() throw();
  NetworkInfo success;

  _RemoteShellService_GetNetworkInfo_result__isset __isset;

  void __set_success(const NetworkInfo& val);

  bool operator == (const RemoteShellService_GetNetworkInfo_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteShellService_GetNetworkInfo_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_GetNetworkInfo_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_GetNetworkInfo_presult__isset {
  _RemoteShellService_GetNetworkInfo_presult__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_GetNetworkInfo_presult__isset;

class RemoteShellService_GetNetworkInfo_presult {
 public:


  virtual ~RemoteShellService_GetNetworkInfo_presult() throw();
  NetworkInfo* success;

  _RemoteShellService_GetNetworkInfo_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _RemoteShellService_Execute_args__isset {
  _RemoteShellService_Execute_args__isset() : cmdWithArgs(false) {}
  bool cmdWithArgs :1;
} _RemoteShellService_Execute_args__isset;

class RemoteShellService_Execute_args {
 public:

  RemoteShellService_Execute_args(const RemoteShellService_Execute_args&);
  RemoteShellService_Execute_args& operator=(const RemoteShellService_Execute_args&);
  RemoteShellService_Execute_args() : cmdWithArgs() {
  }

  virtual ~RemoteShellService_Execute_args() throw();
  std::string cmdWithArgs;

  _RemoteShellService_Execute_args__isset __isset;

  void __set_cmdWithArgs(const std::string& val);

  bool operator == (const RemoteShellService_Execute_args & rhs) const
  {
    if (!(cmdWithArgs == rhs.cmdWithArgs))
      return false;
    return true;
  }
  bool operator != (const RemoteShellService_Execute_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_Execute_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class RemoteShellService_Execute_pargs {
 public:


  virtual ~RemoteShellService_Execute_pargs() throw();
  const std::string* cmdWithArgs;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_Execute_result__isset {
  _RemoteShellService_Execute_result__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_Execute_result__isset;

class RemoteShellService_Execute_result {
 public:

  RemoteShellService_Execute_result(const RemoteShellService_Execute_result&);
  RemoteShellService_Execute_result& operator=(const RemoteShellService_Execute_result&);
  RemoteShellService_Execute_result() {
  }

  virtual ~RemoteShellService_Execute_result() throw();
  ShellRtn success;

  _RemoteShellService_Execute_result__isset __isset;

  void __set_success(const ShellRtn& val);

  bool operator == (const RemoteShellService_Execute_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const RemoteShellService_Execute_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const RemoteShellService_Execute_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _RemoteShellService_Execute_presult__isset {
  _RemoteShellService_Execute_presult__isset() : success(false) {}
  bool success :1;
} _RemoteShellService_Execute_presult__isset;

class RemoteShellService_Execute_presult {
 public:


  virtual ~RemoteShellService_Execute_presult() throw();
  ShellRtn* success;

  _RemoteShellService_Execute_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class RemoteShellServiceClient : virtual public RemoteShellServiceIf {
 public:
  RemoteShellServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  RemoteShellServiceClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void GetComputerInfo(ComputerInfo& _return);
  void send_GetComputerInfo();
  void recv_GetComputerInfo(ComputerInfo& _return);
  void GetCpuInfo(CpuInfo& _return);
  void send_GetCpuInfo();
  void recv_GetCpuInfo(CpuInfo& _return);
  void GetMemoryInfo(MemoryInfo& _return);
  void send_GetMemoryInfo();
  void recv_GetMemoryInfo(MemoryInfo& _return);
  void GetDiskInfo(DiskInfo& _return);
  void send_GetDiskInfo();
  void recv_GetDiskInfo(DiskInfo& _return);
  void GetNetworkInfo(NetworkInfo& _return);
  void send_GetNetworkInfo();
  void recv_GetNetworkInfo(NetworkInfo& _return);
  void Execute(ShellRtn& _return, const std::string& cmdWithArgs);
  void send_Execute(const std::string& cmdWithArgs);
  void recv_Execute(ShellRtn& _return);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class RemoteShellServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<RemoteShellServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (RemoteShellServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_GetComputerInfo(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetCpuInfo(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetMemoryInfo(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetDiskInfo(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_GetNetworkInfo(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_Execute(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  RemoteShellServiceProcessor(::apache::thrift::stdcxx::shared_ptr<RemoteShellServiceIf> iface) :
    iface_(iface) {
    processMap_["GetComputerInfo"] = &RemoteShellServiceProcessor::process_GetComputerInfo;
    processMap_["GetCpuInfo"] = &RemoteShellServiceProcessor::process_GetCpuInfo;
    processMap_["GetMemoryInfo"] = &RemoteShellServiceProcessor::process_GetMemoryInfo;
    processMap_["GetDiskInfo"] = &RemoteShellServiceProcessor::process_GetDiskInfo;
    processMap_["GetNetworkInfo"] = &RemoteShellServiceProcessor::process_GetNetworkInfo;
    processMap_["Execute"] = &RemoteShellServiceProcessor::process_Execute;
  }

  virtual ~RemoteShellServiceProcessor() {}
};

class RemoteShellServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  RemoteShellServiceProcessorFactory(const ::apache::thrift::stdcxx::shared_ptr< RemoteShellServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr< RemoteShellServiceIfFactory > handlerFactory_;
};

class RemoteShellServiceMultiface : virtual public RemoteShellServiceIf {
 public:
  RemoteShellServiceMultiface(std::vector<apache::thrift::stdcxx::shared_ptr<RemoteShellServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~RemoteShellServiceMultiface() {}
 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<RemoteShellServiceIf> > ifaces_;
  RemoteShellServiceMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<RemoteShellServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void GetComputerInfo(ComputerInfo& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetComputerInfo(_return);
    }
    ifaces_[i]->GetComputerInfo(_return);
    return;
  }

  void GetCpuInfo(CpuInfo& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetCpuInfo(_return);
    }
    ifaces_[i]->GetCpuInfo(_return);
    return;
  }

  void GetMemoryInfo(MemoryInfo& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetMemoryInfo(_return);
    }
    ifaces_[i]->GetMemoryInfo(_return);
    return;
  }

  void GetDiskInfo(DiskInfo& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetDiskInfo(_return);
    }
    ifaces_[i]->GetDiskInfo(_return);
    return;
  }

  void GetNetworkInfo(NetworkInfo& _return) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->GetNetworkInfo(_return);
    }
    ifaces_[i]->GetNetworkInfo(_return);
    return;
  }

  void Execute(ShellRtn& _return, const std::string& cmdWithArgs) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->Execute(_return, cmdWithArgs);
    }
    ifaces_[i]->Execute(_return, cmdWithArgs);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class RemoteShellServiceConcurrentClient : virtual public RemoteShellServiceIf {
 public:
  RemoteShellServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  RemoteShellServiceConcurrentClient(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void GetComputerInfo(ComputerInfo& _return);
  int32_t send_GetComputerInfo();
  void recv_GetComputerInfo(ComputerInfo& _return, const int32_t seqid);
  void GetCpuInfo(CpuInfo& _return);
  int32_t send_GetCpuInfo();
  void recv_GetCpuInfo(CpuInfo& _return, const int32_t seqid);
  void GetMemoryInfo(MemoryInfo& _return);
  int32_t send_GetMemoryInfo();
  void recv_GetMemoryInfo(MemoryInfo& _return, const int32_t seqid);
  void GetDiskInfo(DiskInfo& _return);
  int32_t send_GetDiskInfo();
  void recv_GetDiskInfo(DiskInfo& _return, const int32_t seqid);
  void GetNetworkInfo(NetworkInfo& _return);
  int32_t send_GetNetworkInfo();
  void recv_GetNetworkInfo(NetworkInfo& _return, const int32_t seqid);
  void Execute(ShellRtn& _return, const std::string& cmdWithArgs);
  int32_t send_Execute(const std::string& cmdWithArgs);
  void recv_Execute(ShellRtn& _return, const int32_t seqid);
 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

}} // namespace

#endif
