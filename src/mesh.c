#include "mesh.h"
#include "darray.h"
#include "vector.h"
#include <stdio.h>
#include <string.h>

struct face_instance_t {
    int a;
    int b;
    int c;
};

struct mesh_instance_t {
    darray_vec3_t vertices;
    darray_face_t faces;
};

mesh_t mesh_create() {
    mesh_t inst = calloc(1, sizeof(struct mesh_instance_t));
    return inst;
}

face_t face_create(int a, int b, int c) {
    face_t inst = calloc(1, sizeof(struct face_instance_t));
    *inst = (struct face_instance_t){a, b, c};
    return inst;
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
            a = face->a;
            b = face->b;
            c = face->c;
            printf("face: %d %d %d\n", a, b, c);
            i++;
            continue;
        }
    }

    

    fclose(fp);
}
