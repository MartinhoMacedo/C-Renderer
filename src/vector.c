#include <stdlib.h>
#include "vector.h"

struct vec2_instance_t {
    int x;
    int y;
};

struct vec3_instance_t {
    struct vec2_instance_t;
    int z;
};

struct vec4_instance_t{
    struct vec3_instance_t;
    int w;
};

vec2_t vec2_create(int x, int y) {
    vec2_t inst = calloc(1, sizeof(struct vec2_instance_t));
    inst->x = x;
    inst->y = y;

    return inst;
}

vec3_t vec3_create(int x, int y, int z) {
    vec3_t inst = calloc(1, sizeof(struct vec3_instance_t));
    inst->x = x;
    inst->y = y;
    inst->z = z;

    return inst;
}

vec4_t vec4_create(int x, int y, int z, int w) {
    vec4_t inst = calloc(1, sizeof(struct vec4_instance_t));
    inst->x = x;
    inst->y = y;
    inst->z = z;
    inst->w = w;

    return inst;
}

// Can be used for ex with a vec3 by typecasting it to a vec2: vec2_get_x((vec2_t) vec3)
int vec2_get_x(vec2_t inst) {
    return inst->x;
}

int vec2_get_y(vec2_t inst) {
    return inst->y;
}

int vec3_get_z(vec3_t inst) {
    return inst->z;
}

int vec4_get_w(vec4_t inst) {
    return inst->w;
}
