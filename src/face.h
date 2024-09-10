#ifndef FACE_H_
#define FACE_H_
#include "darray.h"

typedef struct face_instance_t* face_t;

face_t face_create(int a, int b, int c);

int face_get_a(face_t inst) ;
int face_get_b(face_t inst) ;
int face_get_c(face_t inst) ;

void face_destroy(face_t inst);

darray_header(face_t);

#endif // FACE_H_
