#include "face.h"
#include <stdlib.h>
#include "triangle.h"
#include "vector.h"

darray_definition(face_t);

struct face_instance_t {
    int a;
    int b;
    int c;
    /*
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    */
};

face_t face_create(int a, int b, int c) {
    face_t inst = malloc(sizeof(struct face_instance_t));
    inst->a = a;
    inst->b = b;
    inst->c = c;
    return inst;
}

void face_clip(face_t inst, darray_vec3_t vertices[], int *triangles_count) {


}

void face_set(face_t inst, int a, int b, int c) {
    inst->a = a;
    inst->b = b;
    inst->c = c;
}

void face_copy(face_t orig, face_t dest) {
    dest->a = orig->a;
    dest->b = orig->b;
    dest->c = orig->c;
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

void face_destroy(face_t inst) {
    free(inst);
}
