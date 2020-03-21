#include "uiautomation_helper.h"
#include <iostream>

int main(int argc, char* argv[])
{
    TRY_BEGIN
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    UIAInitializer uiaInit;
    auto elemList = findElementByProp({
        { UIA_NamePropertyId, L"Program Manager" },
        { UIA_ClassNamePropertyId, L"Progman" },
    });
    for (const auto elem : elemList) {
        std::cout << elem << std::endl;
    }
    CATCH_ALL
}