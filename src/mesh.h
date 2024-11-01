#ifndef MESH_H_
#define MESH_H_

#include "vector.h"
// TODO: fix this by changing darray declarations to classes files
#include "darray.h"
#include "face.h"

typedef struct mesh_instance_t* mesh_t;

mesh_t mesh_create(char* filename);
mesh_t mesh_create_clone(mesh_t orig);
void mesh_destroy(mesh_t inst);


void mesh_load_texture(mesh_t inst, char *filename);

void mesh_copy(mesh_t orig, mesh_t dest);

void mesh_transform(mesh_t inst, float translate_x, float translate_y,
                    float translate_z, float rotate_x, float rotate_y,
                    float rotate_z,
                    float scale_x, float scale_y, float scale_z);
void mesh_to_camera_space(mesh_t inst, mat4_t camera);


void mesh_translate(mesh_t inst, float x, float y, float z);
void mesh_rotate(mesh_t inst, float x, float y, float z);

void mesh_backface_culling(mesh_t inst, vec3_t camera);

texture_t* mesh_get_texture(mesh_t inst);
darray_vec3_t mesh_get_vertices(mesh_t inst);
darray_face_t mesh_get_faces(mesh_t inst);



#endif // MESH_H_
