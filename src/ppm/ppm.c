// Alexander Bass
// Created 4/16/24
// Modified 4/21/24
#include "ppm.h"

#include "../texture.h"
#include "../types.h"
#include <assert.h>
#include <stdio.h>

texture_t load_ppm(FILE *file) {
    assert(fgetc(file) == 'P');
    assert(fgetc(file) == '3');

    u32 width, height;
    u32 color;

    int res1 = fscanf(file, "%d %d", &width, &height);
    assert(res1 > 0);
    int res2 = fscanf(file, "%d", &color);
    assert(res2 > 0);

    assert(width == 16 && height == 16);
    assert(color == 255);

    texture_t tex = texture(width, height);

    for (size_t i = 0; i < width * height * 3; i++) {
        u8 byte;
        int res3 = fscanf(file, "%hhd", &byte);
        assert(res3 > 0);
        tex.bytes[i] = byte;
    }

    return tex;
}

texture_t load_ppm_file(const char *file_name) {
    FILE *file = fopen(file_name, "r");
    texture_t tex = load_ppm(file);
    fclose(file);
    return tex;
}