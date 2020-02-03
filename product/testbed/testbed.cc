#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "common/debugging/print.h"

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    std::vector<int> vec{1, 2, 3, 4, 5};
    PRINT(vec);
}