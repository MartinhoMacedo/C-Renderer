#include <stdlib.h>
#include "vector.h"
#include "macros.h"
#include <stdio.h>
#include <math.h>

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

void vec2_set(vec2_t inst, float x, float y) {
    inst->x = x;
    inst->y = y;
}

float vec2_dot(vec2_t a, vec2_t b) {
    return a->x * b->x + a->y * b->y;
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void vec3_cross(vec3_t a, vec3_t b, vec3_t res) {
    res->x = a->y*b->z - a->z*b->y;
    res->y = a->z*b->x - a->x*b->z;
    res->z = a->x*b->y - a->y*b->x;
}

float vec2_magnitude(vec2_t inst) {
    return sqrt(inst->x * inst->x + inst->y * inst->y);
}

float vec3_magnitude(vec3_t inst) {
    return sqrt(inst->x * inst->x + inst->y * inst->y + inst->z * inst->z);
}

void vec2_normal(vec2_t inst, vec2_t res) {
    float magnitude = vec2_magnitude(inst);
    res->x = inst->x / magnitude;
    res->y = inst->y / magnitude;
}

void vec3_normal(vec3_t inst, vec3_t res) {
    float magnitude = vec3_magnitude(inst);
    res->x = inst->x / magnitude;
    res->y = inst->y / magnitude;
    res->z = inst->z / magnitude;
}

void vec2_vadd(vec2_t a, vec2_t b, vec2_t res) {
    res->x = a->x + b->x;
    res->y = a->y + b->y;
}

void vec3_vadd(vec3_t a, vec3_t b, vec3_t res) {
    res->x = a->x + b->x;
    res->y = a->y + b->y;
    res->z = a->z + b->z;
}

void vec2_add(vec2_t a, float b_x, float b_y, vec2_t res) {
    res->x = a->x + b_x;
    res->y = a->y + b_y;
}

void vec3_add(vec3_t a, float b_x, float b_y, float b_z, vec3_t res) {
    res->x = a->x + b_x;
    res->y = a->y + b_y;
    res->z = a->z + b_z;
}

void vec2_vsub(vec2_t a, vec2_t b, vec2_t res) {
    res->x = a->x - b->x;
    res->y = a->y - b->y;
}

void vec3_vsub(vec3_t a, vec3_t b, vec3_t res) {
    res->x = a->x - b->x;
    res->y = a->y - b->y;
    res->z = a->z - b->z;
}

void vec3_rotate_x(vec3_t inst, float angle) {
    struct vec3_instance_t rotated = {
      .x = inst->x,
      .y = inst->y * cos(angle) - inst->z * sin(angle),
      .z = inst->y * sin(angle) + inst->z * cos(angle)
    };
    *inst = rotated;
}

void vec3_rotate_y(vec3_t inst, float angle) {
    struct vec3_instance_t rotated = {
      .x = inst->x * cos(angle) - inst->z * sin(angle),
      .y = inst->y,
      .z = inst->x * sin(angle) + inst->z * cos(angle)
    };
    *inst = rotated;
}

void vec3_rotate_z(vec3_t inst, float angle) {
    struct vec3_instance_t rotated = {
      .x = inst->x * cos(angle) + inst->y * sin(angle),
      .y = -inst->x * sin(angle) + inst->y * cos(angle),
      .z = inst->z
    };
    *inst = rotated;
}

void vec2_to_string(vec2_t inst, char string[]) {
    snprintf(string, 50, "(%f, %f)", inst->x, inst->y);
}

void vec3_to_string(vec3_t inst, char string[]) {
    snprintf(string, 50, "(%f, %f, %f)", inst->x, inst->y, inst->z);
}

void vec3_copy(vec3_t src, vec3_t dst) {
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
