#ifndef VECTOR_H_
#define VECTOR_H_
#include "darray.h"

typedef struct vec2_instance_t* vec2_t;
typedef struct vec3_instance_t* vec3_t;
typedef struct vec4_instance_t* vec4_t;

vec2_t vec2_create(float x, float y);
vec3_t vec3_create(float x, float y, float z);
vec4_t vec4_create(float x, float y, float z, float w);

float vec2_get_x(vec2_t inst);
float vec2_get_y(vec2_t inst);
float vec3_get_z(vec3_t inst);
float vec4_get_w(vec4_t inst);

void vec3_project(vec3_t inst, vec2_t res);

void vec2_vadd(vec2_t a, vec2_t b, vec2_t res);
void vec3_vadd(vec3_t a, vec3_t b, vec3_t res);
void vec2_add(vec2_t a, float b_x, float b_y, vec2_t res);
void vec3_add(vec3_t a, float b_x, float b_y, float b_z, vec3_t res);

void vec3_rotate_x(vec3_t inst, float angle);
void vec3_rotate_y(vec3_t inst, float angle);
void vec3_rotate_z(vec3_t inst, float angle);

void vec3_copy(vec3_t src, vec3_t dst);

void vec2_to_string(vec2_t inst, char string[]);
void vec3_to_string(vec3_t inst, char string[]);

void vec2_destroy(vec2_t inst);
void vec3_destroy(vec3_t inst);
void vec4_destroy(vec3_t inst);

darray_header(vec3_t);
#endif // VECTOR_H_
