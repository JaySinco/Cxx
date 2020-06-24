#include "common/utility/base.h"
#include "shape.h"
#include <chrono>
#include <limits>
#include <opencv2/highgui/highgui.hpp>
#include <thread>

vec3 color(const ray& r, const hitable& word)
{
    hit_record rec;
    if (word.hit(r, 10e-4, std::numeric_limits<float>::max(), rec)) {
        vec3 N = rec.normal;
        return 0.5 * vec3(N[2] + 1, N[1] + 1, N[0] + 1);
    }
    float t = 0.5 * (cv::normalize(r.dir())[1] + 1);
    return (1 - t) * vec3(1, 1, 1) + t * vec3(1, 0.7, 0.5);
}

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    cv::Mat image = cv::Mat::zeros(400, 800, CV_32FC3);
    int nx = image.size().width;
    int ny = image.size().height;

    vec3 low_left(-2, -1, -1);
    vec3 horizonal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0, 0, 0);

    hitable_list word;
    word.put<sphere>(vec3(0, 0, -1), 0.5);
    word.put<sphere>(vec3(0, -100.5, -1), 100);

    for (int y = 0; y < ny; ++y) {
        for (int x = 0; x < nx; ++x) {
            float u = (float)x / nx;
            float v = (float)y / ny;
            ray r(origin, low_left + u * horizonal + v * vertical);
            image.at<vec3>(ny - y - 1, x) = color(r, word);
        }
    }

    cv::imshow("raytrace", image);
    cv::waitKey(0);
}