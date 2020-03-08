#include "common/utility/logging.h"
#include "service.h"
#include <thrift/TToString.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/stdcxx.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
using namespace cxx::service;

int main(int argc, char** argv)
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    TThreadedServer server(
        stdcxx::make_shared<RemoteShellServiceProcessor>(stdcxx::make_shared<RemoteShellService>()),
        stdcxx::make_shared<TServerSocket>(9090), //port
        stdcxx::make_shared<TBufferedTransportFactory>(),
        stdcxx::make_shared<TBinaryProtocolFactory>());
    LOG(INFO) << "starting the server...";
    server.serve();
    LOG(INFO) << "done.";
}