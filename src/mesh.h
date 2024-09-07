#ifndef MESH_H_
#define MESH_H_

#include "vector.h"

typedef struct face_instance_t* face_t;
typedef struct mesh_instance_t* mesh_t;

void mesh_load_file(mesh_t mesh, char* filename);

#endif // MESH_H_
