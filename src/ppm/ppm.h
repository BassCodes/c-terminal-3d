// Alexander Bass
// Created  4/16/24
// Modified 4/21/24
#pragma once

#include "../texture.h"
#include "../types.h"
#include <stdio.h>

texture_t load_ppm(FILE *file_stream);
texture_t load_ppm_file(const char *file_name);