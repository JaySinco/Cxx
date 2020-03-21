#include "common/utility/base.h"
#include "common/utility/string_helper.h"
#include <algorithm>
#include <execution>
#include <functional>
#include <future>
#include <iostream>
#include <sstream>
#include <vector>

using namespace cxx;
using namespace std::literals;
using namespace std::placeholders;

int main(int argc, char* argv[])
{
    TRY_BEGIN
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    CATCH_ALL
}