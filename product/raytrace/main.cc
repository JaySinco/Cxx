#include "common/utility/base.h"
#include "render.h"

void setup_scene(container& word)
{
    auto s1 = create<sphere>(vec3(0, 0, -1), 0.5);
    s1->made_of<diffuse>(RGB(0.1, 0.2, 0.5));
    word.put(s1);
    auto s2 = create<sphere>(vec3(0, -100.5, -1), 100);
    s2->made_of<diffuse>(RGB(0.8, 0.8, 0));
    word.put(s2);
    auto s3 = create<sphere>(vec3(1, 0, -1), 0.5);
    s3->made_of<metal>(RGB(0.8, 0.6, 0.2), 0.2);
    word.put(s3);
    auto s4 = create<sphere>(vec3(-1, 0, -1), 0.5);
    s4->made_of<dielectric>(1.5);
    word.put(s4);
    auto s5 = create<sphere>(vec3(-1, 0, -1), -0.45);
    s5->made_of<dielectric>(1.5);
    word.put(s5);
}

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;

    container word;
    setup_scene(word);
    parallel_renderer render(2, 10, 10);
    camera cam;
    auto image = render.render(word, cam, { 200, 100, 100 });
}