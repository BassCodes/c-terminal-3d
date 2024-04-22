// Alexander Bass
// Created  4/16/24
// Modified 4/21/24
#pragma once
#include "types.h"
#include "vec3.h"
typedef struct point_t {
    vec3_t pos;
    vec3_t color;
} point_t;

point_t point(vec3_t, vec3_t);