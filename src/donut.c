// Alexander Bass
// Created 4/16/24
// Modified 4/21/24

#include "donut.h"
#include "point.h"
#include "point_list.h"
#include "texture.h"
#include "types.h"
#include "vec3.h"
#include <math.h>

vec3_t sample_rgb(texture_t tex, size_t index) {
    u8 red = tex.bytes[index];
    u8 green = tex.bytes[index + 1];
    u8 blue = tex.bytes[index + 2];
    vec3_t color = {(f64)red, (f64)green, (f64)blue};
    color = mul_vec(color, vec3(1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0));
    return color;
};

vec3_t mix_transparency(vec3_t transparent, vec3_t solid) {
    if (transparent.i == 0.0 && transparent.j == 0.0 && transparent.k == 0.0) {
        return solid;
    } else {
        return transparent;
    }
}
void generate_cube(point_list_t *points, texture_t bottom_tex, texture_t top_tex, texture_t side_tex, i32 face_x, i32 face_y) {
    const vec3_t grass_color_scale = {0.4, 1.0, 0.4};
    const size_t image_size = 16;
    const size_t point_count = 20;
    const size_t world_width = 16;
    for (size_t l_x = 0; l_x < point_count; l_x++) {
        for (size_t l_y = 0; l_y < point_count; l_y++) {

            f64 sx = (f64)l_x / (f64)point_count;
            f64 sy = (f64)l_y / (f64)point_count;
            size_t index = 3 * ((size_t)(round(sx * (f64)(image_size))) + (size_t)(round(sy * (f64)(image_size)) * image_size));
            vec3_t dirt = sample_rgb(bottom_tex, index);
            vec3_t grass = sample_rgb(top_tex, index);
            vec3_t side = sample_rgb(side_tex, index);
            grass = mul_vec(grass, grass_color_scale);
            side = mul_vec(side, grass_color_scale);

            side = mix_transparency(side, dirt);

            f64 x_w = sx * (f64)world_width;
            f64 y_w = sy * (f64)world_width;
            f64 w_w = (f64)world_width;

            if (face_y == -1 || (face_y == 1 && face_x == 0)) {
                push_point(points, point(vec3(x_w, 0, y_w), side));
            }
            if (face_x == -1 || (face_x == 1 && face_y == 0)) {
                push_point(points, point(vec3(0, x_w, y_w), side));
            }
            if (face_x == 1 || (face_x == -1 && face_y == 0)) {
                push_point(points, point(vec3(w_w, x_w, y_w), side));
            }
            if (face_y == 1 || (face_y == -1 && face_x == 0)) {
                push_point(points, point(vec3(x_w, w_w, y_w), side));
            }

            push_point(points, point(vec3(x_w, y_w, w_w), dirt));
            push_point(points, point(vec3(x_w, y_w, 0.0), grass));
        }
    }

    for (size_t i = 0; i < points->length; i++) {
        point_t *p = &points->list[i];
        p->pos.i += ((f64)(face_x) * (f64)(world_width) - (f64)(world_width) / 2.0);
        p->pos.j += ((f64)(face_y) * (f64)(world_width) - (f64)(world_width) / 2.0);
        p->pos.k += (-(f64)world_width / 2.0);
        rotate_j(&p->pos, 180 * 3.14159265 / 180);
    }
}

point_list_t generate_donut(texture_t bottom, texture_t top, texture_t side) {
    point_list_t donut_points = point_list(15000);
    for (i32 x = -1; x <= 1; x++) {
        for (i32 y = -1; y <= 1; y++) {
            if (x == y && x == 0) {
                continue;
            }
            point_list_t cube_points = point_list(2000);
            generate_cube(&cube_points, bottom, top, side, x, y);
            extend_point_list(&donut_points, &cube_points);
            free_point_list(&cube_points);
        }
    }
    return donut_points;
}