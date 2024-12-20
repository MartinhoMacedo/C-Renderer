#include <stdlib.h>
#include "matrix.h"
#include "vector.h"
#include "macros.h"
#include <stdio.h>
#include <math.h>

darray_definition(vec3_t);
darray_definition(vec2_t);

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
    vec2_t inst = malloc(sizeof(struct vec2_instance_t));
    inst = vec2_init(inst, x, y);

    return inst;
}

vec3_t vec3_create(float x, float y, float z) {
    vec3_t inst = malloc(sizeof(struct vec3_instance_t));
    inst = vec3_init(inst, x, y, z);

    return inst;
}

vec4_t vec4_create(float x, float y, float z, float w) {
    vec4_t inst = malloc(sizeof(struct vec4_instance_t));
    inst = vec4_init(inst, x, y, z, w);

    return inst;
}

// Inits object to previously allocated memory (in the stack with a char buffer[] for ex)
vec2_t vec2_init(void* buffer, float x, float y) {
    vec2_t inst = (vec2_t) buffer;
    inst->x = x;
    inst->y = y;

    return inst;
}

// Inits object to previously allocated memory
vec3_t vec3_init(void* buffer, float x, float y, float z) {
    vec3_t inst = (vec3_t) buffer;
    inst->x = x;
    inst->y = y;
    inst->z = z;

    return inst;
}

vec4_t vec4_init(void* buffer, float x, float y, float z, float w) {
    vec4_t inst = (vec4_t) buffer;
    inst->x = x;
    inst->y = y;
    inst->z = z;
    inst->w = w;

    return inst;
}

// TODO: To be replaced by a projection matrix multiplication
void vec3_project(vec3_t inst, float fov, float zfar, float znear, vec3_t res) {
    // P'x = FOV * (Px/Pz) * -A, (where A is distance from our eyes to the screen: we assume 1)
    // P'y = FOV * (Py/Pz) * -A, (where A is distance from our eyes to the screen: we assume 1)
    // -A = 1
    //res->x = -FOV*inst->x/inst->z;
    //res->y = -FOV*inst->y/inst->z;

    char mat_buffer[mat4_struct_get_size()];

    mat4_t projmat = mat4_init_projection(mat_buffer, fov, zfar, znear);
    struct vec4_instance_t vec4 = {
        .x = inst->x, .y = inst->y, .z = inst->z, .w = 1};

    vec4_mul_mat4(projmat, &vec4, &vec4);

    vec4_homogeneous_to_cartesian(&vec4, (vec3_t) &vec4);

    *res = (struct vec3_instance_t){.x = vec4.x, .y = vec4.y, .z = vec4.z};
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

float vec4_dot(vec4_t a, vec4_t b) {
    return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
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

// Find barycentric coordinates for p where a b and c are the vertices of the triangle
//
void vec3_barycentric(vec3_t res, vec3_t p, vec3_t a, vec3_t b, vec3_t c) {
    float a_x = vec2_get_x((vec2_t) a);
    float b_x = vec2_get_x((vec2_t) b);
    float c_x = vec2_get_x((vec2_t) c);
    float a_y = vec2_get_y((vec2_t) a);
    float b_y = vec2_get_y((vec2_t) b);
    float c_y = vec2_get_y((vec2_t) c);

    char abc_cross_buffer[vec3_struct_get_size()];
    char ab_buffer[vec3_struct_get_size()];
    char ac_buffer[vec3_struct_get_size()];
    char a_tmp_buffer[vec3_struct_get_size()];
    char b_tmp_buffer[vec3_struct_get_size()];
    char c_tmp_buffer[vec3_struct_get_size()];
    char pbc_cross_buffer[vec3_struct_get_size()];
    char pca_cross_buffer[vec3_struct_get_size()];
    char pab_cross_buffer[vec3_struct_get_size()];
    char pa_buffer[vec3_struct_get_size()];
    char pb_buffer[vec3_struct_get_size()];
    char pc_buffer[vec3_struct_get_size()];
    vec3_t a_tmp = vec3_init(a_tmp_buffer, a_x, a_y, 0);
    vec3_t b_tmp = vec3_init(b_tmp_buffer, b_x, b_y, 0);
    vec3_t c_tmp = vec3_init(c_tmp_buffer, c_x, c_y, 0);
    vec3_t abc_cross = vec3_init(abc_cross_buffer, 0, 0, 0);
    vec3_t pbc_cross = vec3_init(pbc_cross_buffer, 0, 0, 0);
    vec3_t pca_cross = vec3_init(pca_cross_buffer, 0, 0, 0);
    vec3_t pab_cross = vec3_init(pab_cross_buffer, 0, 0, 0);
    vec3_t ab = vec3_init(ab_buffer, 0, 0, 0);
    vec3_t ac = vec3_init(ac_buffer, 0, 0, 0);
    vec3_t pa = vec3_init(pa_buffer, 0, 0, 0);
    vec3_t pb = vec3_init(pb_buffer, 0, 0, 0);
    vec3_t pc = vec3_init(pc_buffer, 0, 0, 0);

    vec3_vsub(b_tmp, a_tmp, ab);
    vec3_vsub(c_tmp, a_tmp, ac);
    vec3_vsub(a_tmp, p, pa);
    vec3_vsub(b_tmp, p, pb);
    vec3_vsub(c_tmp, p, pc);

    vec3_cross(ac, ab, abc_cross);
    float abc_cross_magnitude = vec3_get_z(abc_cross);

    // By calculating the cross product between pc and pb we obtain the area of
    // a paralelogram which is double the area of the triangle pbc.
    // By dividing the area of that triangle by the area of the abc triangle
    // we obtain the "weight" of the vertex "a" which is the barycentric coefficient for "a"
    vec3_cross(pc, pb, pbc_cross);
    float a_w = vec3_get_z(pbc_cross) / abc_cross_magnitude;
    vec3_cross(pa, pc, pca_cross);
    float b_w = vec3_get_z(pca_cross) / abc_cross_magnitude;
    vec3_cross(pb, pa, pab_cross);
    float c_w = vec3_get_z(pab_cross) / abc_cross_magnitude;

    if (a_w < 0 || b_w < 0 || c_w < 0 || a_w > 1 || b_w > 1 || c_w > 1) {
        //TODO: Debug here
        //printf("Barycentric error: a_w=%f; b_w=%f; c_w=%f\n", a_w, b_w, c_w);
        //TODO: Need to fix subpixel precision
    }

    res->x = a_w;
    res->y = b_w;
    res->z = c_w;
}

// TODO: This will have problems if references a = b, fix it!
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

void vec2_mul(vec2_t a, float factor, vec2_t res) {
    res->x = a->x * factor;
    res->y = a->y * factor;
}

void vec3_mul(vec3_t a, float factor, vec3_t res) {
    res->x = a->x * factor;
    res->y = a->y * factor;
    res->z = a->z * factor;
}

void vec2_scale(vec2_t inst, float factor_x, float factor_y, vec2_t res) {
    struct vec2_instance_t tmp_res = {.x = inst->x * factor_x,
                                      .y = inst->y * factor_y};
    *res = tmp_res;
}

void vec3_scale(vec3_t inst, float factor_x, float factor_y, float factor_z, vec3_t res) {
    struct vec3_instance_t tmp_res = {
      .x = inst->x * factor_x,
      .y = inst->y * factor_y,
      .z = inst->z * factor_z};

    *res = tmp_res;
}

void vec2_round(vec2_t inst) {
    inst->x = roundf(inst->x);
    inst->y = roundf(inst->y);
}

// res = mat * vec
void vec4_mul_mat4(mat4_t m, vec4_t v, vec4_t res) {
    struct vec4_instance_t l0 = {.x = mat4_get_element(m, 0, 0),
                                 .y = mat4_get_element(m, 0, 1),
                                 .z = mat4_get_element(m, 0, 2),
                                 .w = mat4_get_element(m, 0, 3)};
    struct vec4_instance_t l1 = {.x = mat4_get_element(m, 1, 0),
                                 .y = mat4_get_element(m, 1, 1),
                                 .z = mat4_get_element(m, 1, 2),
                                 .w = mat4_get_element(m, 1, 3)};
    struct vec4_instance_t l2 = {.x = mat4_get_element(m, 2, 0),
                                 .y = mat4_get_element(m, 2, 1),
                                 .z = mat4_get_element(m, 2, 2),
                                 .w = mat4_get_element(m, 2, 3)};
    struct vec4_instance_t l3 = {.x = mat4_get_element(m, 3, 0),
                                 .y = mat4_get_element(m, 3, 1),
                                 .z = mat4_get_element(m, 3, 2),
                                 .w = mat4_get_element(m, 3, 3)};

    struct vec4_instance_t res_tmp = {.x = vec4_dot(v, &l0),
                                      .y = vec4_dot(v, &l1),
                                      .z = vec4_dot(v, &l2),
                                      .w = vec4_dot(v, &l3)};
   *res = res_tmp;
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

void vec4_homogeneous_to_cartesian(vec4_t inst, vec3_t dst) {
    struct vec3_instance_t converted = {
      .x = inst->x / inst->w,
      .y = inst->y / inst->w,
      .z = inst->z / inst->w};
   *dst = converted;
}

void vec2_to_string(vec2_t inst, char string[]) {
    snprintf(string, 50, "(%f, %f)", inst->x, inst->y);
}

void vec3_to_string(vec3_t inst, char string[]) {
    snprintf(string, 50, "(%f, %f, %f)", inst->x, inst->y, inst->z);
}

void vec2_copy(vec2_t src, vec2_t dst) {
    dst->x = src->x;
    dst->y = src->y;
}

void vec3_copy(vec3_t src, vec3_t dst) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

// Can be used for ex with a vec3 by typecasting it to a vec2: vec2_get_x((vec2_t) vec3)
void vec2_set_x(vec2_t inst, float x) {
    inst->x = x;
}

void vec2_set_y(vec2_t inst, float y) {
    inst->y = y;
}

void vec3_set_z(vec3_t inst, float z) {
    inst->z = z;
}

void vec4_set_w(vec4_t inst, float w) {
    inst->w = w;
}

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

// Important for manual allocations outside the class (using init)
size_t vec2_struct_get_size() {
    return sizeof(struct vec2_instance_t);
}

size_t vec3_struct_get_size() {
    return sizeof(struct vec3_instance_t);
}

size_t vec4_struct_get_size() {
    return sizeof(struct vec4_instance_t);
}


void vec2_destroy(vec2_t inst) {
    free(inst);
}

void vec3_destroy(vec3_t inst) {
    free(inst);
}

void vec4_destroy(vec4_t inst) {
    free(inst);
}
