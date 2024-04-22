// Alexander Bass
// Created 4/16/24
// Modified 4/21/24
#pragma once
#include "types.h"
typedef struct vec3_t {
    f64 i;
    f64 j;
    f64 k;
} vec3_t;

vec3_t vec3(f64 i, f64 j, f64 k);
vec3_t mul_vec(vec3_t lhs, vec3_t rhs);
vec3_t mul_scalar(vec3_t lhs, f64 rhs);

void rotate_i(vec3_t *point, f64 angle);
void rotate_j(vec3_t *point, f64 angle);
void rotate_k(vec3_t *point, f64 angle);