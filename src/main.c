// Alexander Bass
// Created  4/16/24
// Modified 4/21/24

#include "donut.h"
#include "point.h"
#include "point_list.h"
#include "ppm/ppm.h"
#include "texture.h"
#include "types.h"
#include "vec3.h"
#include "vec4.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const i32 WIDTH = 120;
const i32 HEIGHT = 90;
const i32 DEPTH = WIDTH;

void draw(vec4_t *, const point_list_t *);
void update(point_list_t *);

int main() {
    texture_t dirt = load_ppm_file("dirt.ppm");
    texture_t grass = load_ppm_file("grass_top.ppm");
    texture_t grass_side = load_ppm_file("grass_side.ppm");

    // point_list_t points = point_list(300);

    point_list_t points = generate_donut(dirt, grass, grass_side);
    // extend_point_list(&points, &donut_points);
    // free_point_list(&donut_points);

    free_texture(&dirt);
    free_texture(&grass);
    free_texture(&grass_side);

    vec4_t *pixel_buffer = malloc((size_t)(WIDTH * HEIGHT) * sizeof(vec4_t));
    for (u32 i = 0; i < (u32)(WIDTH * HEIGHT); i++) {
        // pixel_buffer[i] = vec4(0.0, 0.0, 0.0, 0.0);
    };

    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = 1000000L * 1000L / 60L;

    while (true) {
        // Move cursor to top left
        printf("\033[0;0H");
        // Draw points
        draw(pixel_buffer, &points);
        // Rotate points
        update(&points);
        // Wait
        nanosleep(&tim, NULL);
        // Repeat
    }

    free_point_list(&points);
    free(pixel_buffer);
    return 0;
}

i32 w_x_to_screen(f64 w_x) {
    return (i32)(round(w_x)) + WIDTH / 2;
}

i32 w_y_to_screen(f64 w_y) {
    return (i32)(round(w_y)) + HEIGHT / 2;
}

f64 w_z_to_screen(f64 w_z) {
    return (w_z + (f64)(DEPTH) / 2.0) / (f64)(DEPTH);
}

void draw(vec4_t *pixel_buffer, const point_list_t *points) {
    for (u32 i = 0; i < (u32)(WIDTH * HEIGHT); i++) {
        pixel_buffer[i].l = 0.0;
    };

    // Pseudo-rasterization
    for (size_t i = 0; i < points->length; i++) {
        point_t point = points->list[i];

        vec3_t pos = point.pos;
        vec3_t color = point.color;

        f64 p_x = pos.i;
        f64 p_y = pos.j;
        f64 p_z = pos.k;

        i32 s_x = w_x_to_screen(p_x);
        i32 s_y = w_y_to_screen(p_y);
        f64 s_z = w_z_to_screen(p_z);

        if (0 <= s_x && s_x < WIDTH && 0 <= s_y && s_y < HEIGHT) {
            size_t index = (size_t)(s_x + s_y * WIDTH);
            vec4_t existing = pixel_buffer[index];
            if (existing.l < s_z) {
                pixel_buffer[index].i = color.i;
                pixel_buffer[index].j = color.j;
                pixel_buffer[index].k = color.k;
                pixel_buffer[index].l = s_z;
            }
        };
    }

    for (size_t y = 0; y < (size_t)(HEIGHT); y++) {
        for (size_t x = 0; x < (size_t)(WIDTH); x++) {
            size_t index = x + y * (size_t)(WIDTH);
            vec4_t pixel = pixel_buffer[index];
            f64 depth = pixel.l;
            if (depth <= 0.0) {
                printf("  ");
                continue;
            }

            assert(depth >= 0.0);
            assert(depth <= 1.0);

            u8 red = (u8)(round(pixel.i * depth * 255.0));
            u8 green = (u8)(round(pixel.j * depth * 255.0));
            u8 blue = (u8)(round(pixel.k * depth * 255.0));
            printf("\033[38;2;%u;%u;%um██", red, green, blue);
        }
        printf("\n");
    }
}

void update(point_list_t *points) {
    for (size_t i = 0; i < points->length; i++) {
        vec3_t *pos = &points->list[i].pos;
        rotate_i(pos, 0.02);
        rotate_j(pos, -0.04);
        rotate_k(pos, 0.03);
    }
}
