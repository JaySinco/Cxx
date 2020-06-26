#include "common/utility/base.h"
#include "render.h"
#include <opencv2/imgcodecs.hpp>

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    int width = 200;
    int height = 150;
    int render_unit_width = 20;
    int render_unit_height = float(height) / width * render_unit_width;
    int sample = 30;

    parallel_renderer render(2, render_unit_width, render_unit_height);

    cv::Mat output = render.render_scene(random_sphere, { width, height, sample });
    // cv::Mat image;
    // output.convertTo(image, CV_8UC3, 255);
    // cv::imwrite("random_sphere.jpg", image);
}