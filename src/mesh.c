#include "mesh.h"
#include "darray.h"
#include "macros.h"
#include "vector.h"
#include "face.h"
#include "matrix.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


void mesh_load_file(mesh_t mesh, char* filename);

struct mesh_instance_t {
    darray_vec3_t vertices;
    darray_face_t faces;
    //int32_t *texture;
};

mesh_t mesh_create(char* filename) {
    mesh_t inst = malloc(sizeof(struct mesh_instance_t));
    if(filename){ mesh_load_file(inst, filename); }
    return inst;
}

/**
 * Creates a new mesh with deep copied values. */
mesh_t mesh_create_clone(mesh_t orig) {
    mesh_t inst = malloc(sizeof(struct mesh_instance_t));

    int vertices_size = darray_vec3_t_get_occupied(orig->vertices);
    int faces_size = darray_face_t_get_occupied(orig->faces);

    inst->vertices = darray_vec3_t_create();
    inst->faces = darray_face_t_create();

    for (int i = 0; i < vertices_size; i++) {
        vec3_t orig_vertex = darray_vec3_t_get(orig->vertices, i);
        vec3_t dest_vertex = vec3_create(0, 0, 0);
        vec3_copy(orig_vertex, dest_vertex);
        darray_vec3_t_push(inst->vertices, dest_vertex);
    }

    for (int i = 0; i < faces_size; i++) {
       face_t orig_face = darray_face_t_get(orig->faces, i);
       face_t dest_face = face_create(0, 0, 0);
       face_copy(orig_face, dest_face);
       darray_face_t_push(inst->faces, dest_face);
    }

    return inst;
}

/**
 * Deep copies the origins array values to the destination.
 * The destination should have allocated arrays of the same size as the origin. */
void mesh_copy(mesh_t orig, mesh_t dest) {
    int vertices_size = darray_vec3_t_get_occupied(orig->vertices);
    int faces_size = darray_face_t_get_occupied(orig->faces);

    for (int i = 0; i < vertices_size; i++) {
        vec3_t orig_vertex = darray_vec3_t_get(orig->vertices, i);
        vec3_t dest_vertex = darray_vec3_t_get(dest->vertices, i);
        vec3_copy(orig_vertex, dest_vertex);
    }

   for (int i = 0; i < faces_size; i++) {
       face_t orig_face = darray_face_t_get(orig->faces, i);
       face_t dest_face = darray_face_t_get(dest->faces, i);
       face_copy(orig_face, dest_face);
    }
}


void mesh_transform(mesh_t inst, float translate_x, float translate_y,
                    float translate_z, float rotate_x, float rotate_y,
                    float rotate_z,
                    float scale_x, float scale_y, float scale_z) {
    int vertices_size = darray_vec3_t_get_occupied(inst->vertices);

    // apply transformation to all vertices
    for (int i = 0; i < vertices_size; i++) {
        char tmp_vertice_buffer[vec4_struct_get_size()];
        char world_buffer[mat4_struct_get_size()];

        vec3_t vertice = darray_vec3_t_get(inst->vertices, i);
        vec4_t tmp_vertice = vec4_init(tmp_vertice_buffer,
                                       vec2_get_x((vec2_t) vertice),
                                       vec2_get_y((vec2_t) vertice),
                                       vec3_get_z(vertice),
                                        1);

        mat4_t world = mat4_init_world(world_buffer, scale_x, scale_y, scale_z, rotate_x,
                                         rotate_y, rotate_z,
                                         translate_x,
                                         translate_y, translate_z);
        vec4_mul_mat4(world, tmp_vertice, tmp_vertice);

        vec3_copy((vec3_t)tmp_vertice, vertice);


        /*
        // rotation
        vec3_rotate_x(vertice, rotate_x);
        vec3_rotate_y(vertice, rotate_y);
        vec3_rotate_z(vertice, rotate_z);

        // translation
        vec3_add(vertice, translate_x, translate_y, translate_z, vertice);
        */
    }
}

void mesh_to_camera_space(mesh_t inst, mat4_t camera) {
    int vertices_size = darray_vec3_t_get_occupied(inst->vertices);

    for (int i = 0; i < vertices_size; i++) {
        char tmp_vertice_buffer[vec4_struct_get_size()];

        vec3_t vertice = darray_vec3_t_get(inst->vertices, i);
        vec4_t tmp_vertice = vec4_init(tmp_vertice_buffer,
                                       vec2_get_x((vec2_t) vertice),
                                       vec2_get_y((vec2_t) vertice),
                                       vec3_get_z(vertice),
                                        1);
        vec4_mul_mat4(camera, tmp_vertice, tmp_vertice);

        vec3_copy((vec3_t)tmp_vertice, vertice);
    }

}

void mesh_translate(mesh_t inst, float x, float y, float z) {
    int vertices_size = darray_vec3_t_get_occupied(inst->vertices);

    // apply transformation to all vertices
    for (int i = 0; i < vertices_size; i++) {
        vec3_t vertice = darray_vec3_t_get(inst->vertices, i);

        // translation
        vec3_add(vertice, x, y, z, vertice);
    }
}

void mesh_rotate(mesh_t inst, float x, float y, float z) {
    int vertices_size = darray_vec3_t_get_occupied(inst->vertices);

    for (int i = 0; i < vertices_size; i++) {
        vec3_t vertice = darray_vec3_t_get(inst->vertices, i);

        // rotation
        vec3_rotate_x(vertice, x);
        vec3_rotate_y(vertice, y);
        vec3_rotate_z(vertice, z);
    }
}
/**
 * Sets every face that is not seen by the camera to a not rendarizable value */
void mesh_backface_culling(mesh_t inst, vec3_t camera) {
    int faces_size = darray_face_t_get_occupied(inst->faces);

    for (int i = 0; i < faces_size; i++) {
        face_t face = darray_face_t_get(inst->faces, i);

        int a_idx = face_get_a(face) - 1;
        int b_idx = face_get_b(face) - 1;
        int c_idx = face_get_c(face) - 1;

        vec3_t a = darray_vec3_t_get(inst->vertices, a_idx);
        vec3_t b = darray_vec3_t_get(inst->vertices, b_idx);
        vec3_t c = darray_vec3_t_get(inst->vertices, c_idx);

        // TODO: Change this to a function in vector "vec3_aligment" maybe
        char ab_buffer[vec3_struct_get_size()];
        char ac_buffer[vec3_struct_get_size()];

        vec3_t ab = (vec3_t) ab_buffer;
        vec3_t ac = (vec3_t) ac_buffer;
        vec3_vsub(b, a, ab);
        vec3_vsub(c, a, ac);

        // normalize vectors
        char cross_buffer[vec3_struct_get_size()];
        vec3_t cross_product = (vec3_t) cross_buffer;

        vec3_cross(ab, ac,  cross_product);
        vec3_normal(cross_product, cross_product);

        // Get vector from camera to point A
        char acamera_buffer[vec3_struct_get_size()];
        vec3_t acamera = (vec3_t) acamera_buffer;

        vec3_vsub(camera, a, acamera);
        vec3_normal(acamera, acamera);

        float dot_product =  vec3_dot(cross_product, acamera);

        if (dot_product <= 0) {
            face_set(face, 0,0,0);
        }
    }
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
