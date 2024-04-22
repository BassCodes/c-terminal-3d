// Alexander Bass
// Created 4/16/24
// Modified 4/21/24
#include "vec3.h"
#include "types.h"

#include <math.h>

void rotate_i(vec3_t *point, f64 angle) {
    f64 cos_a = cos(angle);
    f64 sin_a = sin(angle);
    f64 y_1 = point->j * cos_a - point->k * sin_a;
    f64 z_1 = point->j * sin_a + point->k * cos_a;
    point->j = y_1;
    point->k = z_1;
}

void rotate_j(vec3_t *point, f64 angle) {
    f64 cos_a = cos(angle);
    f64 sin_a = sin(angle);

    f64 x_1 = point->i * cos_a + point->k * sin_a;
    f64 z_1 = point->k * cos_a - point->i * sin_a;
    point->i = x_1;
    point->k = z_1;
}

void rotate_k(vec3_t *point, f64 angle) {
    f64 cos_a = cos(angle);
    f64 sin_a = sin(angle);

    f64 x_1 = point->i * cos_a - point->j * sin_a;
    f64 y_1 = point->i * sin_a + point->j * cos_a;
    point->i = x_1;
    point->j = y_1;
}

vec3_t vec3(f64 i, f64 j, f64 k) {
    vec3_t vector = {i, j, k};
    return vector;
}

vec3_t mul_vec(vec3_t lhs, vec3_t rhs) {
    lhs.i *= rhs.i;
    lhs.j *= rhs.j;
    lhs.k *= rhs.k;
    return lhs;
}

vec3_t mul_scalar(vec3_t lhs, f64 rhs) {
    lhs.i *= rhs;
    lhs.j *= rhs;
    lhs.k *= rhs;
    return lhs;
}