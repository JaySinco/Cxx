#include "common/utility/base.h"
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    cv::Mat image = cv::Mat::zeros(800, 800, CV_32F);
    cv::imshow("raytrace", image);
    cv::waitKey(0);
}