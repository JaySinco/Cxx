#include "camera.h"
#include "common/utility/base.h"
#include "material.h"
#include "ray.h"
#include "shape.h"
#include <iostream>
#include <limits>
#include <opencv2/highgui/highgui.hpp>

vec3 color(const ray& r, const container& word, int depth)
{
    hit_record rec;
    if (word.hit(r, 10e-3, std::numeric_limits<float>::max(), rec)) {
        ray r_out;
        vec3 decay;
        if (depth < 50 && rec.m->scatter(r, rec, decay, r_out)) {
            return mul(decay, color(r_out, word, depth + 1));
        } else {
            return RGB(0, 0, 0);
        }
    }
    float t = 0.5 * (cv::normalize(r.dir())[1] + 1);
    return (1 - t) * RGB(1, 1, 1) + t * RGB(0.5, 0.7, 1);
}

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    cv::Mat image = cv::Mat::zeros(100, 200, CV_32FC3);
    int nx = image.size().width;
    int ny = image.size().height;
    int ns = 100;

    camera cam;
    container word;
    auto s1 = create<sphere>(vec3(0, 0, -1), 0.5);
    s1->made_of<diffuse>(RGB(0.8, 0.3, 0.3));
    word.put(s1);
    auto s2 = create<sphere>(vec3(0, -100.5, -1), 100);
    s2->made_of<diffuse>(RGB(0.8, 0.8, 0));
    word.put(s2);
    auto s3 = create<sphere>(vec3(1, 0, -1), 0.5);
    s3->made_of<metal>(RGB(0.8, 0.6, 0.2), 1.0);
    word.put(s3);
    auto s4 = create<sphere>(vec3(-1, 0, -1), 0.5);
    s4->made_of<metal>(RGB(0.8, 0.8, 0.8), 0.3);
    word.put(s4);

    const std::string name = "raytrace";
    cv::namedWindow(name, CV_WINDOW_NORMAL);
    if (nx < 800 || ny < 400) {
        cv::resizeWindow(name, 800, 400);
    } else {
        cv::resizeWindow(name, nx, ny);
    }
    cv::Mat blend = cv::Mat::zeros(ny, nx, CV_32FC3);
    std::cout << "0%" << std::flush;
    for (int s = 0; s < ns; ++s) {
        for (int y = 0; y < ny; ++y) {
            for (int x = 0; x < nx; ++x) {
                float u = (x + (s == 0 ? 0 : rand0_1())) / nx;
                float v = (y + (s == 0 ? 0 : rand0_1())) / ny;
                int r = ny - y - 1, c = x;
                vec3& sum = blend.at<vec3>(r, c);
                sum += color(cam.get_ray(u, v), word, 0);
                image.at<vec3>(r, c) = sqrt(sum / (s + 1));
            }
        }
        std::cout << "\b\b\b" << int(float(s + 1) / ns * 100) << "%" << std::flush;
        cv::imshow(name, image);
        cv::waitKey(30);
    }
    cv::waitKey();
    std::cout << std::endl;
}