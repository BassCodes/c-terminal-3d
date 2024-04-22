// Alexander Bass
// Created  4/16/24
// Modified 4/21/24
#include "point.h"
#include "types.h"

point_t point(vec3_t pos, vec3_t color) {
    point_t p = {pos, color};
    return p;
}