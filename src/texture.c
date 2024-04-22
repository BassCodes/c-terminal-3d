#include "texture.h"
// Alexander Bass
// Created  4/21/24
// Modified 4/21/24
#include <stdlib.h>

texture_t texture(u32 width, u32 height) {
    u8 *bytes = (u8 *)(malloc(sizeof(u8) * width * height * 3));
    texture_t tex = {bytes, width, height};
    return tex;
}

void free_texture(texture_t *texture) {
    free(texture->bytes);
}