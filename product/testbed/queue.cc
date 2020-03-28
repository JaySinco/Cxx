#include "common/utility/base.h"
#include <concurrentqueue.h>
#include <gtest/gtest.h>

using namespace cxx;

TEST(ConcurrentQueue, HelloWorld)
{
    moodycamel::ConcurrentQueue<int> q;
    int dequeued[100] = { 0 };
    std::thread threads[20];
    for (int i = 0; i != 10; ++i) {
        threads[i] = std::thread(
            [&](int i) {
                for (int j = 0; j != 10; ++j) {
                    q.enqueue(i * 10 + j);
                }
            },
            i);
    }
    for (int i = 10; i != 20; ++i) {
        threads[i] = std::thread(
            [&]() {
                int item;
                for (int j = 0; j != 20; ++j) {
                    if (q.try_dequeue(item)) {
                        ++dequeued[item];
                    }
                }
            });
    }
    for (int i = 0; i != 20; ++i) {
        threads[i].join();
    }
    int item;
    while (q.try_dequeue(item)) {
        ++dequeued[item];
    }
    for (int i = 0; i != 100; ++i) {
        ASSERT_EQ(dequeued[i], 1);
    }
}

int main(int argc, char* argv[])
{
    TRY_BEGIN
    testing::InitGoogleTest(&argc, argv);
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    RUN_ALL_TESTS();
    google::ShutdownGoogleLogging();
    CATCH_ALL
}