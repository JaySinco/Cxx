#include "scenery.h"
#include "material.h"

void random_sphere(const quality& qua, container& word, camera& cam)
{
    vec3 lookfrom(12, 2, 3);
    vec3 lookat(0, 0, 0);
    cam = camera(lookfrom, lookat, vec3(0, 1, 0), 20, float(qua.w) / qua.h, 0.05, length(lookfrom - lookat));

    auto s0 = create<sphere>(vec3(0, -1000, -1), 1000);
    s0->made_of<diffuse>(RGB(0.5, 0.5, 0.5));
    word.put(s0);

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float choose_mat = rand0_1();
            vec3 center(a + 0.9 * rand0_1(), 0.2, b + 0.9 * rand0_1());
            auto s = create<sphere>(center, 0.2);
            if (length(center - vec3(4, 0.2, 0)) > 0.9) {
                if (choose_mat < 0.8) {
                    s->made_of<diffuse>(RGB(rand0_1(), rand0_1(), rand0_1()));
                } else if (choose_mat < 0.9) {
                    s->made_of<metal>(
                        RGB(0.5 * (rand0_1() + 1),
                            0.5 * (rand0_1() + 1),
                            0.5 * (rand0_1() + 1)),
                        0.5 * rand0_1());
                } else {
                    s->made_of<dielectric>(1.5);
                }
                word.put(s);
            }
        }
    }

    auto s1 = create<sphere>(vec3(0, 1, 0), 1);
    s1->made_of<dielectric>(1.5);
    word.put(s1);

    auto s2 = create<sphere>(vec3(-4, 1, 0), 1);
    s2->made_of<diffuse>(RGB(0.4, 0.2, 0.1));
    word.put(s2);

    auto s3 = create<sphere>(vec3(4, 1, 0), 1);
    s3->made_of<metal>(RGB(0.7, 0.6, 0.5), 0.0);
    word.put(s3);
}