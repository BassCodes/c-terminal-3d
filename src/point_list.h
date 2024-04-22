#pragma once
// Alexander Bass
// Created  4/21/24
// Modified 4/21/24
#include "point.h"
#include <stddef.h>

typedef struct point_list_t {
    point_t *list;
    size_t length;
    size_t capacity;
} point_list_t;

point_list_t point_list(size_t capacity);

void push_point(point_list_t *points, point_t point);
void free_point_list(point_list_t *points);
void extend_point_list(point_list_t *list, point_list_t *extension);