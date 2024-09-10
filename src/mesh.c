#include "mesh.h"
#include "darray.h"
#include "macros.h"
#include "vector.h"
#include "face.h"
#include <stdio.h>
#include <string.h>


void mesh_load_file(mesh_t mesh, char* filename);

struct mesh_instance_t {
    darray_vec3_t vertices;
    darray_face_t faces;
};

mesh_t mesh_create(char* filename) {
    mesh_t inst = malloc(sizeof(struct mesh_instance_t));
    mesh_load_file(inst, filename);
    return inst;
}

darray_vec3_t mesh_get_vertices(mesh_t inst) {
    return inst->vertices;
}

darray_face_t mesh_get_faces(mesh_t inst) {
    return inst->faces;
}
// Reads obj file and saves to a mesh struct instance
void mesh_load_file(mesh_t mesh, char* filename) {
    FILE* fp = fopen(filename, "r");
    char line[100];

    mesh->vertices = darray_vec3_t_create();
    mesh->faces = darray_face_t_create();

    if (!fp) {
        printf("Error loading mesh: Could not open the file\n");
        return;
    }

    int i = 0;
    while (fgets(line, 100, fp)) {
        // TODO: Load values into mesh
        if ((line[0] == 'v') && (line[1] = ' ')) {
            float x;
            float y;
            float z;
            sscanf(line, "v %f %f %f", &x, &y, &z);
            darray_vec3_t_push(mesh->vertices, vec3_create(x, y, z));
            continue;
        }

        if (line[0] == 'f') {
            int a;
            int b;
            int c;

            sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &a,
                   &b, &c);
            darray_face_t_push(mesh->faces, face_create(a, b, c));

            face_t face = darray_face_t_get(mesh->faces, i);
            a = face_get_a(face);
            b = face_get_b(face);
            c = face_get_c(face);
            /*
            printf("face: %d %d %d\n", a, b, c);
            int count = darray_face_t_get_occupied(mesh->faces);
            int limit = darray_face_t_get_capacity(mesh->faces);
            printf("size: %d\n", count);
            printf("limit: %d\n", limit);
            */
            i++;
            continue;
        }
    }
    fclose(fp);
}

void mesh_destroy(mesh_t inst) {
    // Destroy faces dynamic array and its elements
    for (int i = 0; i < sizeof(inst->faces)/sizeof(face_t); i++) {
        face_destroy(darray_face_t_get(inst->faces, i));
    }
    /**
    * NOTE: What if we free an "object" that is used somewhere else outside of this array?
    This is why oop languages have garbage collectors...
    */
    darray_face_t_destroy(inst->faces);

    // Destroy vertices ""
    for (int i = 0; i < sizeof(inst->vertices) / sizeof(vec3_t); i++) {
        vec3_destroy(darray_vec3_t_get(inst->vertices, i));
    }
    darray_vec3_t_destroy(inst->vertices);

    // Lastly free the mesh structure itself
    free(inst);
}
