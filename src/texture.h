#pragma once
// Alexander Bass
// Created  4/21/24
// Modified 4/21/24
#include "types.h"

typedef struct texture_t {
    u8 *const bytes;
    const u32 width;
    const u32 height;
} texture_t;

texture_t texture(u32 width, u32 height);
void free_texture(texture_t *);