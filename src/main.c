// Alexander Bass
// Created  4/16/24
// Modified 5/4/24

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
#include <string.h>
#include <time.h>

#define WIDTH 120
#define HEIGHT 90
#define DEPTH WIDTH

#define SECONDS_TO_NANOSECONDS 1000000000
#define FRAMERATE 60

#define LARGEST_PIXEL_CHAR_STRING "\033[38;2;255;255;255m██"
#define LARGEST_PIXEL_CHAR_LEN sizeof(LARGEST_PIXEL_CHAR_STRING) - 1
#define CHAR_BUFFER_CAPACITY (HEIGHT * WIDTH * LARGEST_PIXEL_CHAR_LEN + HEIGHT) * sizeof(char)

void draw(vec4_t *, const point_list_t *, char *);
void update(point_list_t *, u64);

int main() {
    texture_t dirt = load_ppm_file("dirt.ppm");
    texture_t grass = load_ppm_file("grass_top.ppm");
    texture_t grass_side = load_ppm_file("grass_side.ppm");

    point_list_t points = generate_donut(dirt, grass, grass_side);

    free_texture(&dirt);
    free_texture(&grass);
    free_texture(&grass_side);

    vec4_t *pixel_buffer = malloc(WIDTH * HEIGHT * sizeof(vec4_t));
    for (u32 i = 0; i < (WIDTH * HEIGHT); i++) {
        pixel_buffer[i] = vec4(0.0, 0.0, 0.0, 0.0);
    };

    char *screen_char_buffer = malloc(CHAR_BUFFER_CAPACITY);

#ifdef __GLIBC__
    __SYSCALL_SLONG_TYPE frame_budget = SECONDS_TO_NANOSECONDS / FRAMERATE;
    struct timespec frame_start, frame_end, sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = frame_budget;
#endif
    u64 counter = 0;

    while (true) {
#ifdef __GLIBC__
        clock_gettime(CLOCK_MONOTONIC_RAW, &frame_start);
#endif
        // Move cursor to top left
        printf("\033[0;0H");
        // Draw points
        draw(pixel_buffer, &points, screen_char_buffer);
        // Rotate points
        update(&points, counter);

#ifdef __GLIBC__
        // Compute time until next frame
        clock_gettime(CLOCK_MONOTONIC_RAW, &frame_end);
        __SYSCALL_SLONG_TYPE diff_ns = frame_end.tv_nsec - frame_start.tv_nsec;
        __SYSCALL_SLONG_TYPE diff_s = frame_end.tv_sec - frame_start.tv_sec;
        diff_ns += SECONDS_TO_NANOSECONDS * diff_s;

        __SYSCALL_SLONG_TYPE excess = diff_ns > frame_budget ? 0 : frame_budget - diff_ns;
        // Framerate is dropping
        // if (excess == 0) {
        //     printf("\033[38;2;255;255;255mCan't keep up; Exiting. Ran for %lu frames\n", counter);
        //     exit(0);
        // }

        sleep_time.tv_nsec = excess;
        // Wait until next frame
        nanosleep(&sleep_time, NULL);
#else
        // At least it's portable
        sleep(1);
#endif
        counter += 1;
    }

    free_point_list(&points);
    free(pixel_buffer);
    return 0;
}

inline size_t w_x_to_screen(f64 w_x) {
    return (size_t)(round(w_x)) + WIDTH / 2;
}

inline size_t w_y_to_screen(f64 w_y) {
    return (size_t)(round(w_y)) + HEIGHT / 2;
}

inline f64 w_z_to_screen(f64 w_z) {
    return (w_z + (f64)(DEPTH) / 2.0) / (f64)(DEPTH);
}

inline void push_string(char *to, const char *from, size_t *to_length) {
    // Assumes `to` has enough room for the string in `from`
    size_t i = 0;
    while (from[i] != '\0') {
        char c = from[i];
        to[i + *to_length] = c;
        i++;
    }
    (*to_length) += i;
}

void draw(vec4_t *pixel_buffer, const point_list_t *points, char *screen_char_buffer) {
    // Reset Depth buffer
    for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
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

        size_t s_x = w_x_to_screen(p_x);
        size_t s_y = w_y_to_screen(p_y);
        f64 s_z = w_z_to_screen(p_z);

        if (s_x < WIDTH && s_y < HEIGHT) {
            size_t index = (s_x + s_y * WIDTH);
            vec4_t existing = pixel_buffer[index];
            if (existing.l < s_z) {
                pixel_buffer[index].i = color.i;
                pixel_buffer[index].j = color.j;
                pixel_buffer[index].k = color.k;
                pixel_buffer[index].l = s_z;
            }
        };
    }

    size_t char_buffer_length = 0;

    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            size_t index = x + y * WIDTH;
            vec4_t pixel = pixel_buffer[index];
            f64 depth = pixel.l;
            if (depth <= 0.0) {
                screen_char_buffer[char_buffer_length] = ' ';
                screen_char_buffer[char_buffer_length + 1] = ' ';
                char_buffer_length += 2;
                continue;
            }

            assert(depth >= 0.0);
            assert(depth <= 1.0);

            char color_buf[64] = "";

            u8 red = (u8)(round(pixel.i * depth * 255.0));
            u8 green = (u8)(round(pixel.j * depth * 255.0));
            u8 blue = (u8)(round(pixel.k * depth * 255.0));

            snprintf(color_buf, 32, "\033[38;2;%hhu;%hhu;%hhum██", red, green, blue);
            push_string(screen_char_buffer, color_buf, &char_buffer_length);
        }
        screen_char_buffer[char_buffer_length] = '\n';
        char_buffer_length++;
    }

    fwrite(screen_char_buffer, 1, char_buffer_length, stdout);
}

void update(point_list_t *points, u64 frame_count) {
    for (size_t i = 0; i < points->length; i++) {
        vec3_t *pos = &points->list[i].pos;
        // rotate_i(pos, sin(frame_count / 32.0) / 32.0);
        rotate_j(pos, 0.04);
        rotate_k(pos, -0.037);
    }
}
