#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct mat4_instance_t* mat4_t;

mat4_t mat4_create(float data[4][4]);
mat4_t mat4_create_identity(void);
mat4_t mat4_create_scale(float x, float y, float z);
mat4_t mat4_create_rotation(float x, float y, float z);
mat4_t mat4_create_rotation_x(float angle);
mat4_t mat4_create_rotation_y(float angle);
mat4_t mat4_create_rotation_z(float angle);
mat4_t mat4_create_translation(float x, float y, float z);

float mat4_get_element(mat4_t inst, int line, int column);

#endif // MATRIX_H_
