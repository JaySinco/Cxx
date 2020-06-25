#include "camera.h"
#include "common/utility/base.h"
#include "material.h"
#include "ray.h"
#include "shape.h"
#include <limits>
#include <opencv2/highgui/highgui.hpp>

vec3 color(const ray& r, const hitable& word, int depth)
{
    hit_record rec;
    if (word.hit(r, 10e-3, std::numeric_limits<float>::max(), rec)) {
        ray r_out;
        vec3 fade;
        if (depth < 50 && rec.mat->scatter(r, rec, fade, r_out)) {
            return fade.mul(color(r_out, word, depth + 1));
        } else {
            return vec3(0, 0, 0);
        }
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

    cv::Mat image = cv::Mat::zeros(100, 200, CV_32FC3);
    int nx = image.size().width;
    int ny = image.size().height;
    int ns = 100;

    camera cam;
    hitable_list word;
    word.put<sphere>(vec3(0, 0, -1), 0.5, std::make_shared<diffuse>(vec3(0.3, 0.3, 0.8)));
    word.put<sphere>(vec3(0, -100.5, -1), 100, std::make_shared<diffuse>(vec3(0.0, 0.8, 0.8)));
    word.put<sphere>(vec3(1, 0, -1), 0.5, std::make_shared<metal>(vec3(0.2, 0.6, 0.8), 1.0));
    word.put<sphere>(vec3(-1, 0, -1), 0.5, std::make_shared<metal>(vec3(0.8, 0.8, 0.8), 0.3));

    for (int y = 0; y < ny; ++y) {
        for (int x = 0; x < nx; ++x) {
            vec3 blend(0, 0, 0);
            for (int s = 0; s < ns; ++s) {
                float u = (x + rand0_1()) / nx;
                float v = (y + rand0_1()) / ny;
                blend += color(cam.get_ray(u, v), word, 0);
            }
            blend /= ns;
            blend[0] = std::sqrt(blend[0]);
            blend[1] = std::sqrt(blend[1]);
            blend[2] = std::sqrt(blend[2]);
            image.at<vec3>(ny - y - 1, x) = blend;
        }
    }

    const std::string name = "raytrace";
    cv::namedWindow(name, CV_WINDOW_NORMAL);
    cv::imshow(name, image);
    cv::resizeWindow(name, nx * 6, ny * 6);
    cv::waitKey(0);
}