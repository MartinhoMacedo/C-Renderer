#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct mat4_instance_t {
    float data[4][4];
};

mat4_t mat4_create(float data[4][4]) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));
    if (data != NULL) {
        memcpy(inst->data, data, sizeof(float) * 16);
        return inst;
    }
    memset(inst->data, 0, sizeof(float)*16);
    return inst;
}

mat4_t mat4_create_identity(void) {
    mat4_t inst = mat4_create(NULL);
    inst->data[0][0] = 1;
    inst->data[1][1] = 1;
    inst->data[2][2] = 1;
    inst->data[3][3] = 1;
    return inst;
}

mat4_t mat4_create_scale(float x, float y, float z) {
    mat4_t inst = mat4_create_identity();
    inst->data[0][0] = x;
    inst->data[1][1] = y;
    inst->data[2][2] = z;
    return inst;
}

mat4_t mat4_create_rotation_x(float angle) {
    mat4_t inst = mat4_create_identity();
    inst->data[1][1] = cos(angle);
    inst->data[1][2] = -sin(angle);
    inst->data[2][1] = sin(angle);
    inst->data[2][2] = cos(angle);
    return inst;
}

mat4_t mat4_create_rotation_y(float angle) {
    mat4_t inst = mat4_create_identity();
    inst->data[0][0] = cos(angle);
    inst->data[0][2] = -sin(angle);
    inst->data[2][0] = sin(angle);
    inst->data[2][2] = cos(angle);
    return inst;
}

mat4_t mat4_create_rotation_z(float angle) {
    mat4_t inst = mat4_create_identity();
    inst->data[0][0] = cos(angle);
    inst->data[0][1] = sin(angle);
    inst->data[1][0] = -sin(angle);
    inst->data[1][1] = cos(angle);
    return inst;
}

mat4_t mat4_create_translation(float x, float y, float z) {
    mat4_t inst = mat4_create_identity();
    inst->data[0][3] = x;
    inst->data[1][3] = y;
    inst->data[2][3] = z;
    return inst;
}

float mat4_get_element(mat4_t inst, int line, int column) {
    return inst->data[line][column];
}
