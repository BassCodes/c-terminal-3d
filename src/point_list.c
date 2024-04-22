// Alexander Bass
// Created  4/21/24
// Modified 4/21/24
#include "point_list.h"
#include "point.h"
#include <stddef.h>
#include <stdlib.h>

point_list_t point_list(size_t capacity) {
    point_t *points = (point_t *)(malloc(sizeof(point_t) * capacity));
    point_list_t point_list = {points, 0, capacity};
    return point_list;
}

void push_point(point_list_t *points, point_t point) {
    // Dynamic length list
    size_t length = points->length;
    size_t capacity = points->capacity;
    if (length == capacity) {
        size_t new_capacity = capacity + 100;
        points->list = realloc(points->list, new_capacity * sizeof(point_t));
        points->capacity = new_capacity;
    }
    points->list[length] = point;
    (points->length)++;
}

void free_point_list(point_list_t *points) {
    free(points->list);
}

void extend_point_list(point_list_t *points, point_list_t *extension) {
    // Dynamic length list
    size_t length = points->length;
    size_t capacity = points->capacity;
    size_t new_length = length + extension->length;
    if (new_length >= capacity) {
        size_t new_capacity = new_length + 100;
        points->list = realloc(points->list, new_capacity * sizeof(point_t));
        points->capacity = new_capacity;
    }
    for (size_t i = 0; i < extension->length; i++) {
        points->list[i + length] = extension->list[i];
        (points->length)++;
    }
}