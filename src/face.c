#include "face.h"
#include <stdlib.h>
#include "triangle.h"
#include "vector.h"

darray_definition(face_t);

struct face_instance_t {
    int a;
    int b;
    int c;
    vec2_t a_uv;
    vec2_t b_uv;
    vec2_t c_uv;
};

face_t face_create(int a, int b, int c, vec2_t a_uv, vec2_t b_uv, vec2_t c_uv) {
    face_t inst = malloc(sizeof(struct face_instance_t));
    inst->a = a;
    inst->b = b;
    inst->c = c;
    //TODO: Load uv
    // TODO: Decide how to allocate uvs and initialize.
    inst->a_uv = a_uv;
    inst->b_uv = b_uv;
    inst->c_uv = c_uv;
    return inst;
}

void face_set(face_t inst, int a, int b, int c, vec2_t a_uv, vec2_t b_uv, vec2_t c_uv) {
    inst->a = a;
    inst->b = b;
    inst->c = c;
    inst->a_uv = a_uv;
    inst->b_uv = b_uv;
    inst->c_uv = c_uv;
}

void face_set_a_uv(face_t inst, vec2_t a_uv) {
    inst->a_uv = a_uv;
}

void face_set_b_uv(face_t inst, vec2_t b_uv) {
    inst->b_uv = b_uv;
}

void face_set_c_uv(face_t inst, vec2_t c_uv) {
    inst->c_uv = c_uv;
}


void face_copy(face_t orig, face_t dest) {
    dest->a = orig->a;
    dest->b = orig->b;
    dest->c = orig->c;
    dest->a_uv = orig->a_uv;
    dest->b_uv = orig->b_uv;
    dest->c_uv = orig->c_uv;
}

int face_get_a(face_t inst) {
    return inst->a;
}

int face_get_b(face_t inst) {
    return inst->b;
}

int face_get_c(face_t inst) {
    return inst->c;
}

vec2_t face_get_a_uv(face_t inst) {
    return inst->a_uv;
}

vec2_t face_get_b_uv(face_t inst) {
    return inst->b_uv;
}

vec2_t face_get_c_uv(face_t inst) {
    return inst->c_uv;
}

void face_destroy(face_t inst) {
    free(inst);
}
