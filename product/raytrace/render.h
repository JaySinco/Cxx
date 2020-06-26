#pragma once
#include "blockingconcurrentqueue.h"
#include "common/utility/threadpool.h"
#include "material.h"
#include "ray.h"
#include "scenery.h"

vec3 color(const ray& r, const container& word, int depth);

class renderer {
public:
    virtual ~renderer() {};
    virtual cv::Mat render(const container& word, const camera& cam, const quality& qua) = 0;
    cv::Mat render_scene(const scenery& scene, const quality& qua);
};

class parallel_renderer : public renderer {
public:
    parallel_renderer(int n_threads, int unit_w, int unit_h)
        : pool(n_threads)
        , unit_w_(unit_w)
        , unit_h_(unit_h)
    {
    }
    ~parallel_renderer() = default;
    cv::Mat render(const container& word, const camera& cam, const quality& qua) override;

private:
    struct small_region {
        int x, y; // left-top pixel
        int w, h; // size
    };

    struct piece_of_work {
        small_region rect;
        cv::Mat image;
    };

    void split(int width, int height, std::vector<small_region>& region_list) const;
    void render_small_region(const small_region region, const container& word, const camera& cam, const quality& qua);

    moodycamel::BlockingConcurrentQueue<piece_of_work> queue;
    cxx::ThreadPool pool;
    int unit_w_, unit_h_;
};