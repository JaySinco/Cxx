#include "render.h"
#include <chrono>
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

cv::Mat renderer::render_scene(const scenery& scene, const quality& qua)
{
    container word;
    camera cam;
    scene(qua, word, cam);
    return render(word, cam, qua);
}

void parallel_renderer::split(int width, int height, std::vector<small_region>& region_list) const
{
    int nx = width / unit_w_;
    int ny = height / unit_h_;
    for (int iy = 0; iy < ny; ++iy) {
        for (int ix = 0; ix < nx; ++ix) {
            region_list.push_back(
                { ix * unit_w_, iy * unit_h_, unit_w_, unit_h_ });
        }
    }
    int dx = width % unit_w_;
    int dy = height % unit_h_;
    if (dx > 0) {
        for (int iy = 0; iy < ny; ++iy) {
            region_list.push_back(
                { nx * unit_w_, iy * unit_h_, dx, unit_h_ });
        }
    }
    if (dy > 0) {
        for (int ix = 0; ix < nx; ++ix) {
            region_list.push_back(
                { ix * unit_w_, ny * unit_h_, unit_w_, dy });
        }
    }
    if (dx > 0 && dy > 0) {
        region_list.push_back(
            { nx * unit_w_, ny * unit_h_, dx, dy });
    }
}

void parallel_renderer::render_small_region(const small_region region, const container& word, const camera& cam, const quality& qua)
{
    piece_of_work ret;
    ret.rect = region;
    ret.image = cv::Mat::zeros(region.h, region.w, CV_32FC3);
    for (int y = region.y; y < region.y + region.h; ++y) {
        for (int x = region.x; x < region.x + region.w; ++x) {
            vec3 blend(0, 0, 0);
            for (int s = 0; s < qua.ns; ++s) {
                float u = (x + rand0_1()) / qua.w;
                float v = (y + rand0_1()) / qua.h;
                blend += color(cam.get_ray(u, v), word, 0);
            }
            blend /= qua.ns;
            ret.image.at<vec3>(region.h - y + region.y - 1, x - region.x)
                = sqrt(blend);
        }
    }
    queue.enqueue(ret);
}

cv::Mat parallel_renderer::render(const container& word, const camera& cam, const quality& qua)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "0%" << std::flush;
    std::vector<small_region> all_sub_region;
    split(qua.w, qua.h, all_sub_region);
    for (const auto& region : all_sub_region) {
        pool.enqueue(
            std::mem_fn(&parallel_renderer::render_small_region),
            this, region, std::cref(word), std::cref(cam), std::cref(qua));
    }
    cv::Mat output = cv::Mat::zeros(qua.h, qua.w, CV_32FC3);
    const std::string name = "raytrace";
    cv::namedWindow(name, CV_WINDOW_NORMAL);
    cv::resizeWindow(name, std::max(qua.w, 800), std::max(qua.w, 800) / qua.w * qua.h);
    for (int i = 0; i < all_sub_region.size(); ++i) {
        piece_of_work ret;
        queue.wait_dequeue(ret);
        ret.image.copyTo(output(cv::Rect(
            ret.rect.x, qua.h - ret.rect.y - ret.rect.h, ret.rect.w, ret.rect.h)));
        cv::imshow(name, output);
        cv::waitKey(1);
        std::cout << "\r" << int(float(i + 1) / all_sub_region.size() * 100) << "%" << std::flush;
    }
    auto dns = std::chrono::high_resolution_clock::now() - start;
    std::cout << "\r" << dns.count() * 1e-9 << "s elapse" << std::endl;
    cv::waitKey();
    return output;
}
