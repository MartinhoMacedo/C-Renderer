#ifndef VECTOR_H_
#define VECTOR_H_
#include "darray.h"
#include "matrix.h"

typedef struct vec2_instance_t* vec2_t;
typedef struct vec3_instance_t* vec3_t;
typedef struct vec4_instance_t* vec4_t;

vec2_t vec2_create(float x, float y);
vec3_t vec3_create(float x, float y, float z);
vec4_t vec4_create(float x, float y, float z, float w);
vec2_t vec2_init(void* buffer, float x, float y);
vec3_t vec3_init(void* buffer, float x, float y, float z);
vec4_t vec4_init(void* buffer, float x, float y, float z, float w);

void vec2_set_x(vec2_t inst, float x);
void vec2_set_y(vec2_t inst, float y);
void vec3_set_z(vec3_t inst, float z);
void vec4_set_w(vec4_t inst, float w);
float vec2_get_x(vec2_t inst);
float vec2_get_y(vec2_t inst);
float vec3_get_z(vec3_t inst);
float vec4_get_w(vec4_t inst);

size_t vec2_struct_get_size();
size_t vec3_struct_get_size();
size_t vec4_struct_get_size();

void vec3_project(vec3_t inst, float fov, float zfar, float znear, vec3_t res);

float vec2_dot(vec2_t a, vec2_t b);
float vec3_dot(vec3_t a, vec3_t b);
float vec4_dot(vec4_t a, vec4_t b);
// TODO: Change unchanged args to const
//void vec3_cross(const vec3_t a, const vec3_t b, vec3_t res) {
void vec3_cross(vec3_t a, vec3_t b, vec3_t res);
float vec2_magnitude(vec2_t inst);
float vec3_magnitude(vec3_t inst);
void vec2_normal(vec2_t inst, vec2_t res);
void vec3_normal(vec3_t inst, vec3_t res);

void vec2_vadd(vec2_t a, vec2_t b, vec2_t res);
void vec3_vadd(vec3_t a, vec3_t b, vec3_t res);
void vec2_add(vec2_t a, float b_x, float b_y, vec2_t res);
void vec3_add(vec3_t a, float b_x, float b_y, float b_z, vec3_t res);
void vec2_vsub(vec2_t a, vec2_t b, vec2_t res);
void vec3_vsub(vec3_t a, vec3_t b, vec3_t res);
void vec2_mul(vec2_t a, float factor, vec2_t res);
void vec3_mul(vec3_t a, float factor, vec3_t res);
void vec2_scale(vec2_t inst, float factor_x, float factor_y, vec2_t res);
void vec3_scale(vec3_t inst, float factor_x, float factor_y, float factor_z, vec3_t res);
void vec2_round(vec2_t inst);


void vec3_barycentric(vec3_t res, vec3_t p, vec3_t a, vec3_t b, vec3_t c);

void vec4_mul_mat4(mat4_t m, vec4_t v, vec4_t res);


void vec3_rotate_x(vec3_t inst, float angle);
void vec3_rotate_y(vec3_t inst, float angle);
void vec3_rotate_z(vec3_t inst, float angle);


void vec4_homogeneous_to_cartesian(vec4_t inst, vec3_t dst);

void vec2_copy(vec2_t src, vec2_t dst);
void vec3_copy(vec3_t src, vec3_t dst);

void vec2_to_string(vec2_t inst, char string[]);
void vec3_to_string(vec3_t inst, char string[]);

void vec2_destroy(vec2_t inst);
void vec3_destroy(vec3_t inst);
void vec4_destroy(vec4_t inst);

darray_header(vec3_t);
darray_header(vec2_t);
#endif // VECTOR_H_
