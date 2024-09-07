#include "mesh.h"
#include <stdio.h>
#include <string.h>

struct face_instance_t {
    int a;
    int b;
    int c;
};

struct mesh_instance_t {
    vec3_t* vertices;
    struct face_instance_t* faces;
};

//TODO: Create arrays for vertices and faces

// Reads obj file and saves to a mesh struct instance
void mesh_load_file(mesh_t mesh, char* filename) {
    FILE* fp = fopen(filename, "r");
    char line[100];

    if (!fp) {
        printf("Error loading mesh: Could not open the file\n");
        return;
    }

    while (fgets(line, 100, fp)) {


        // TODO: Load values into mesh
        if (line[0] == 'v') {
            float x;
            float y;
            float z;
            sscanf(line, "c %f %f %f", &x, &y, &z);
            printf("vertex: %f %f %f\n", x, y, z);
            continue;
        }

        if (line[0] == 'f') {
            int t;
            int a;
            int b;
            int c;

            sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &a, &t, &t,
                   &b, &t, &t, &c, &t, &t);
            printf("face: %d %d %d\n", a, b, c);
            continue;
        }
    }

    

    fclose(fp);
}
