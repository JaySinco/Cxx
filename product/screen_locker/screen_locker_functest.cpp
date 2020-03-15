#include "common/utility/logging.h"
#include "screen_locker.h"
#include <chrono>
#include <thread>

using namespace cxx;

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    if (!SetProcessDPIAware()) {
        LOG_LAST_WIN_ERROR("failed to set process dpi aware");
    }
    std::thread(
        [] {
            std::this_thread::sleep_for(std::chrono::seconds(10));
            ScreenLocker::Close();
        })
        .detach();
    ScreenLocker::Popup(
        "",
        u8"请输入密码: ",
        abspath("product\\screen_locker\\resources\\background.bmp"),
        abspath("product\\screen_locker\\resources\\point.cur"),
        false,
        false,
        500);
}