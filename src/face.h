#ifndef FACE_H_
#define FACE_H_
#include "darray.h"
#include "texture.h"
#include "vector.h"
typedef struct face_instance_t* face_t;

face_t face_create(int a, int b, int c, vec2_t a_uv, vec2_t b_uv, vec2_t c_uv);

void face_copy(face_t orig, face_t dest);

void face_set(face_t inst, int a, int b, int c, vec2_t a_uv, vec2_t b_uv, vec2_t c_uv);
void face_set_a_uv(face_t inst, vec2_t a_uv);
void face_set_b_uv(face_t inst, vec2_t b_uv);
void face_set_c_uv(face_t inst, vec2_t c_uv);

int face_get_a(face_t inst);
int face_get_b(face_t inst);
int face_get_c(face_t inst);
vec2_t face_get_a_uv(face_t inst);
vec2_t face_get_b_uv(face_t inst);
vec2_t face_get_c_uv(face_t inst);

void face_destroy(face_t inst);

darray_header(face_t);

#endif // FACE_H_
