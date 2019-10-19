#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include "client.h"

using namespace cxx;

int main(int argc, char *argv[]) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    wx::Client robot;
    robot.login();
    curl_global_cleanup();
    return 0;
}
