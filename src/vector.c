#include <stdlib.h>
#include "vector.h"
#include "macros.h"

darray_definition(vec3_t);

struct vec2_instance_t {
    float x;
    float y;
};

struct vec3_instance_t {
    struct vec2_instance_t;
    float z;
};

struct vec4_instance_t {
    struct vec3_instance_t;
    float w;
};

vec2_t vec2_create(float x, float y) {
    vec2_t inst = calloc(1, sizeof(struct vec2_instance_t));
    inst->x = x;
    inst->y = y;

    return inst;
}

vec3_t vec3_create(float x, float y, float z) {
    vec3_t inst = calloc(1, sizeof(struct vec3_instance_t));
    inst->x = x;
    inst->y = y;
    inst->z = z;

    return inst;
}

vec4_t vec4_create(float x, float y, float z, float w) {
    vec4_t inst = calloc(1, sizeof(struct vec4_instance_t));
    inst->x = x;
    inst->y = y;
    inst->z = z;
    inst->w = w;

    return inst;
}

// TODO: To be replaced by a projection matrix multiplication
void vec3_project(vec3_t inst, vec2_t res) {
    // P'x = FOV * (Px/Pz) * -A, (where A is distance from our eyes to the screen: we assume 1)
    // P'y = FOV * (Py/Pz) * -A, (where A is distance from our eyes to the screen: we assume 1)
    // -A = 1
    res->x = -FOV*inst->x/inst->z;
    res->y = -FOV*inst->y/inst->z;
}

void vec2_add(vec2_t a, vec2_t b, vec2_t res) {
    res->x = a->x + b->x;
    res->y = a->y + b->y;
}

void vec3_add(vec3_t a, vec3_t b, vec3_t res) {
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

void vec3_copy(vec3_t dst, vec3_t src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

// Can be used for ex with a vec3 by typecasting it to a vec2: vec2_get_x((vec2_t) vec3)
float vec2_get_x(vec2_t inst) {
    return inst->x;
}

float vec2_get_y(vec2_t inst) {
    return inst->y;
}

float vec3_get_z(vec3_t inst) {
    return inst->z;
}

float vec4_get_w(vec4_t inst) {
    return inst->w;
}

void vec2_destroy(vec2_t inst) {
    free(inst);
}

void vec3_destroy(vec3_t inst) {
    free(inst);
}

void vec4_destroy(vec3_t inst) {
    free(inst);
}
