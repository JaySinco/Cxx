#pragma once
#include "camera.h"
#include "shape.h"

struct quality {
    int w, h, ns; // width, height, sample
};

typedef void (*scenery)(const quality& qua, container& word, camera& cam);

void random_sphere(const quality& qua, container& word, camera& cam);
