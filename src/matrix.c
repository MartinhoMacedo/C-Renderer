#include "matrix.h"
#include "vector.h"
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
        // Copy data to matrix
        memcpy(inst->data, data, sizeof(float) * 16);
        return inst;
    }
    // Init matrix with zeros
    memset(inst->data, 0, sizeof(float)*16);
    return inst;
}

mat4_t mat4_init_identity(void *buffer) {
    mat4_t inst = (mat4_t) buffer;

    // Init matrix with zeros
    memset(inst->data, 0, sizeof(float)*16);

    inst->data[0][0] = 1;
    inst->data[1][1] = 1;
    inst->data[2][2] = 1;
    inst->data[3][3] = 1;
    return inst;
}

mat4_t mat4_create_identity(void) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));

    mat4_init_identity(inst);
    return inst;
}

mat4_t mat4_init_scale(void *buffer, float x, float y, float z) {
    mat4_t inst = (mat4_t) buffer;

    inst = mat4_init_identity(inst);
    inst->data[0][0] = x;
    inst->data[1][1] = y;
    inst->data[2][2] = z;
    return inst;
}

mat4_t mat4_create_scale(float x, float y, float z) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));

    inst = mat4_init_scale(inst, x, y, z);
    return inst;
}

mat4_t mat4_init_rotation_x(void *buffer, float angle) {
    mat4_t inst = (mat4_t) buffer;

    inst = mat4_init_identity(buffer);
    inst->data[1][1] = cos(angle);
    inst->data[1][2] = -sin(angle);
    inst->data[2][1] = sin(angle);
    inst->data[2][2] = cos(angle);
    return inst;
}

mat4_t mat4_create_rotation_x(float angle) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));

    inst = mat4_init_rotation_x(inst, angle);
    return inst;
}

mat4_t mat4_init_rotation_y(void* buffer, float angle) {
    mat4_t inst = (mat4_t) buffer;

    inst = mat4_init_identity(buffer);
    inst->data[0][0] = cos(angle);
    inst->data[0][2] = sin(angle);
    inst->data[2][0] = -sin(angle);
    inst->data[2][2] = cos(angle);
    return inst;
}

mat4_t mat4_create_rotation_y(float angle) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));

    inst = mat4_init_rotation_y(inst, angle);
    return inst;
}

mat4_t mat4_init_rotation_z(void *buffer, float angle) {
    mat4_t inst = (mat4_t) buffer;

    inst = mat4_init_identity(inst);
    inst->data[0][0] = cos(angle);
    inst->data[0][1] = -sin(angle);
    inst->data[1][0] = sin(angle);
    inst->data[1][1] = cos(angle);
    return inst;
}

mat4_t mat4_create_rotation_z(float angle) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));

    inst = mat4_init_rotation_z(inst, angle);
    return inst;
}

mat4_t mat4_init_translation(void *buffer, float x, float y, float z) {
    mat4_t inst = (mat4_t) buffer;

    inst = mat4_init_identity(inst);
    inst->data[0][3] = x;
    inst->data[1][3] = y;
    inst->data[2][3] = z;
    return inst;
}

mat4_t mat4_create_translation(float x, float y, float z) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));
    inst = mat4_init_translation(inst, x, y, z);
    return inst;
}

void mat4_mul_mat4(mat4_t a, mat4_t b, mat4_t res) {
    // NOTE: For now I will do allocation of short lived objects using VLAs in the stack
    // like this but VLAs are unsafe
    int vec4_size = vec4_struct_get_size();
    char a_l0_buffer[vec4_size];
    char a_l1_buffer[vec4_size];
    char a_l2_buffer[vec4_size];
    char a_l3_buffer[vec4_size];
    char b_c0_buffer[vec4_size];
    char b_c1_buffer[vec4_size];
    char b_c2_buffer[vec4_size];
    char b_c3_buffer[vec4_size];

    vec4_t a_l0 = vec4_init(a_l0_buffer, a->data[0][0], a->data[0][1], a->data[0][2], a->data[0][3]);
    vec4_t a_l1 = vec4_init(a_l1_buffer, a->data[1][0], a->data[1][1], a->data[1][2], a->data[1][3]);
    vec4_t a_l2 = vec4_init(a_l2_buffer, a->data[2][0], a->data[2][1], a->data[2][2], a->data[2][3]);
    vec4_t a_l3 = vec4_init(a_l3_buffer, a->data[3][0], a->data[3][1], a->data[3][2], a->data[3][3]);
    vec4_t b_c0 = vec4_init(b_c0_buffer, b->data[0][0], b->data[1][0], b->data[2][0], b->data[3][0]);
    vec4_t b_c1 = vec4_init(b_c1_buffer, b->data[0][1], b->data[1][1], b->data[2][1], b->data[3][1]);
    vec4_t b_c2 = vec4_init(b_c2_buffer, b->data[0][2], b->data[1][2], b->data[2][2], b->data[3][2]);
    vec4_t b_c3 = vec4_init(b_c3_buffer, b->data[0][3], b->data[1][3], b->data[2][3], b->data[3][3]);

    struct mat4_instance_t res_tmp = {
      {{vec4_dot(a_l0, b_c0), vec4_dot(a_l0, b_c1), vec4_dot(a_l0, b_c2), vec4_dot(a_l0, b_c3)},
       {vec4_dot(a_l1, b_c0), vec4_dot(a_l1, b_c1), vec4_dot(a_l1, b_c2), vec4_dot(a_l1, b_c3)},
       {vec4_dot(a_l2, b_c0), vec4_dot(a_l2, b_c1), vec4_dot(a_l2, b_c2), vec4_dot(a_l2, b_c3)},
       {vec4_dot(a_l3, b_c0), vec4_dot(a_l3, b_c1), vec4_dot(a_l3, b_c2), vec4_dot(a_l3, b_c3)},
    }};

    *res= res_tmp;
}

mat4_t mat4_init_world(void *buffer, float scale_x, float scale_y,
                       float scale_z, float rotation_x, float rotation_y,
                       float rotation_z, float translation_x,
                       float translation_y, float translation_z) {
    mat4_t inst = (mat4_t) buffer;

    size_t mat4_struct_size = sizeof(struct mat4_instance_t);
    char m_scale_buffer[mat4_struct_size];
    char m_rotation_x_buffer[mat4_struct_size];
    char m_rotation_y_buffer[mat4_struct_size];
    char m_rotation_z_buffer[mat4_struct_size];
    char m_translation_buffer[mat4_struct_size];

    mat4_t m_scale = mat4_init_scale(m_scale_buffer, scale_x, scale_y, scale_z);
    mat4_t m_rotation_x = mat4_init_rotation_x(m_rotation_x_buffer, rotation_x);
    mat4_t m_rotation_y = mat4_init_rotation_y(m_rotation_y_buffer, rotation_y);
    mat4_t m_rotation_z = mat4_init_rotation_z(m_rotation_z_buffer, rotation_z);
    mat4_t m_translation = mat4_init_translation(m_translation_buffer, translation_x,
                              translation_y, translation_z);

    mat4_make_world(m_scale,
                    m_rotation_x, m_rotation_y, m_rotation_z,
                    m_translation, inst);

    return inst;
}

mat4_t mat4_create_world(float scale_x, float scale_y, float scale_z,
                         float rotation_x, float rotation_y, float rotation_z,
                     float translation_x, float translation_y, float translation_z) {
    mat4_t inst = malloc(sizeof (struct mat4_instance_t));

    mat4_init_world(inst, scale_x, scale_y, scale_z, rotation_x, rotation_y,
                    rotation_z, translation_x, translation_y, translation_z);

    return inst;
}

mat4_t mat4_init_projection(void *buffer, float fov, float zfar, float znear) {
    mat4_t inst = (mat4_t) buffer;
    memset(inst->data, 0, sizeof(float)*16);

    float s = 1/(tan(fov/2));
    inst->data[0][0] = s;
    inst->data[1][1] = s;
    inst->data[2][2] = -zfar/(zfar-znear);
    inst->data[2][3] = -(zfar*znear)/(zfar-znear);
    inst->data[3][2] = -1;

    return inst;
}

// Prespective projection into canvas one unit away from camera position
mat4_t mat4_create_projection(float fov, float zfar, float znear) {
    mat4_t inst = mat4_create(NULL);
    mat4_init_projection(inst, fov, zfar, znear);
    return inst;
}

void mat4_transpose(mat4_t inst, mat4_t res) {
    struct mat4_instance_t tmp_res = {
      {
          {inst->data[0][0], inst->data[1][0], inst->data[2][0], inst->data[3][0]},
          {inst->data[0][1], inst->data[1][1], inst->data[2][1], inst->data[3][1]},
          {inst->data[0][2], inst->data[1][2], inst->data[2][2], inst->data[3][2]},
          {inst->data[0][3], inst->data[1][3], inst->data[2][3], inst->data[3][3]}}};

    *res = tmp_res;
}

mat4_t mat4_init_camera(void *buffer, float x, float y, float z,
                        float rotation_x, float rotation_y, float rotation_z) {
    mat4_t inst = (mat4_t) buffer;
    inst = mat4_init_identity(inst);

    // We need to create the world to camera matrix (which is the inverse of camera to world)

    // The inverse of a translation matrix is the translation components negated
    inst->data[0][3] = -x;
    inst->data[1][3] = -y;
    inst->data[2][3] = -z;

    struct mat4_instance_t rotation;
    struct mat4_instance_t m_rotation_x;
    struct mat4_instance_t m_rotation_y;
    struct mat4_instance_t m_rotation_z;

    // Flip the z axis

    mat4_init_identity(&rotation);
    //TODO: The yaw is working on the opposite direction...
    // If this is set and the projection matrix data[3][2] is 1 instead it seems to work
    //rotation.data[2][2] = -1;
    // The other solution is simply setting the yaw to negative in the update function...
    mat4_init_rotation_x(&m_rotation_x, rotation_x);
    mat4_init_rotation_y(&m_rotation_y, rotation_y);
    mat4_init_rotation_z(&m_rotation_z, rotation_z);

    // Since the rotational matrix is orthoghonal, its inverse is the transpose
    mat4_mul_mat4(&m_rotation_x, &rotation, &rotation);
    mat4_mul_mat4(&m_rotation_y, &rotation, &rotation);
    mat4_mul_mat4(&m_rotation_z, &rotation, &rotation);



    mat4_transpose(&rotation, &rotation);

    // The inverse of a multiplication of matrixes is the same as the multiplication of the inverses
    mat4_mul_mat4(&rotation, inst, inst);

    return inst;
}

mat4_t mat4_create_camera(float x, float y, float z, float rotation_x,
                          float rotation_y, float rotation_z) {
    mat4_t inst = malloc(sizeof(struct mat4_instance_t));

    mat4_init_camera(inst, x, y, z, rotation_x, rotation_y, rotation_z);

    return inst;
}

// For first person camera
void mat4_update_fp_camera(mat4_t inst, float longitudinal, float lateral,
                        float vertical, float pitch, float yaw, float roll) {

      struct mat4_instance_t transformation_buffer;
    mat4_t transformation = mat4_init_identity(&transformation_buffer);

    transformation->data[0][3] = -longitudinal;
    transformation->data[1][3] = -lateral;
    transformation->data[2][3] = -vertical;

    struct mat4_instance_t rotation;
    struct mat4_instance_t m_pitch;
    struct mat4_instance_t m_yaw;
    struct mat4_instance_t m_roll;

    mat4_init_identity(&rotation);
    mat4_init_rotation_x(&m_pitch, pitch);
    mat4_init_rotation_y(&m_yaw, yaw);
    mat4_init_rotation_z(&m_roll, roll);

    // Since the rotational matrix is orthoghonal, the inverse is the transpose
    mat4_mul_mat4(&m_pitch, &rotation, &rotation);
    mat4_mul_mat4(&m_yaw, &rotation, &rotation);
    mat4_mul_mat4(&m_roll, &rotation, &rotation);
    mat4_transpose(&rotation, &rotation);

    // The inverse of a multiplication of matrixes is the same as the multiplication of each inverse
    //mat4_mul_mat4(inst, &rotation, inst);
    mat4_mul_mat4(&rotation, transformation, transformation);

    // We should only apply the transformations relative to the camera
    // after the objects are tramsformed to camera space.
    // This will
    // translate and rotate the objects in relation to the previous camera coordinate system
    // in order to obtain the present camera coordinate system.
    // TLDR: This transformation matrix functions as a "old camera space to new camera space"
    // transformation.
    // TODO: Proove this
    mat4_mul_mat4(transformation, inst, inst);
}

void mat4_make_world(mat4_t scale, mat4_t rotation_x, mat4_t rotation_y, mat4_t rotation_z,
                     mat4_t translation, mat4_t res) {
    struct mat4_instance_t res_tmp;

    // NOTE: The order of the rotations matter
    mat4_mul_mat4(rotation_x, scale, &res_tmp);
    mat4_mul_mat4(rotation_y, &res_tmp, &res_tmp);
    mat4_mul_mat4(rotation_z, &res_tmp, &res_tmp);
    mat4_mul_mat4(translation, &res_tmp, &res_tmp);


    *res = res_tmp;
}

float mat4_get_element(mat4_t inst, int line, int column) {
    return inst->data[line][column];
}

size_t mat4_struct_get_size() {
    return sizeof(struct mat4_instance_t);
}

void mat4_destroy(mat4_t inst) {
    free(inst);
}
