#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdio.h>

typedef struct mat4_instance_t* mat4_t;

mat4_t mat4_create(float data[4][4]);
mat4_t mat4_create_identity(void);
mat4_t mat4_create_scale(float x, float y, float z);
mat4_t mat4_create_rotation(float x, float y, float z);
mat4_t mat4_create_rotation_x(float angle);
mat4_t mat4_create_rotation_y(float angle);
mat4_t mat4_create_rotation_z(float angle);
mat4_t mat4_create_translation(float x, float y, float z);
mat4_t mat4_create_world(float scale_x, float scale_y, float scale_z,
                         float rotation_x, float rotation_y, float rotation_z,
                     float translation_x, float translation_y, float translation_z);
mat4_t mat4_create_projection(float fov, float zfar, float znear);
mat4_t mat4_init_projection(void *buffer, float fov, float zfar, float znear);

void mat4_make_world(mat4_t scale, mat4_t rotation_x, mat4_t rotation_y, mat4_t rotation_z,
                     mat4_t translation, mat4_t res);

float mat4_get_element(mat4_t inst, int line, int column);

size_t mat4_struct_get_size();

void mat4_mul_mat4(mat4_t a, mat4_t b, mat4_t res);

#endif // MATRIX_H_
