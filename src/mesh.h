#ifndef MESH_H_
#define MESH_H_

#include "vector.h"
// TODO: fix this by changing darray declarations to classes files
#include "darray.h"
#include "face.h"

typedef struct mesh_instance_t* mesh_t;

mesh_t mesh_create(char* filename);
void mesh_destroy(mesh_t inst);

darray_vec3_t mesh_get_vertices(mesh_t inst);
darray_face_t mesh_get_faces(mesh_t inst);



#endif // MESH_H_
