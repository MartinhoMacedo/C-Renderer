#include "mesh.h"
#include "darray.h"
#include "macros.h"
#include "upng.h"
#include "vector.h"
#include "face.h"
#include "matrix.h"
#include "texture.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void mesh_load_file(mesh_t mesh, char* filename);

struct mesh_instance_t {
    darray_vec3_t vertices;
    darray_vec2_t vertices_uv;
    darray_face_t faces;
    texture_t texture;
};

void abgr_to_argb(uint32_t* colors, int array_size) {
    uint32_t mask0 = 0xFF000000;
    uint32_t mask1 = 0x00FF0000;
    uint32_t mask2 = 0x0000FF00;
    uint32_t mask3 = 0x000000FF;
    for (int i = 0; i < array_size; i++) {
        uint32_t color = colors[i];
        uint32_t blue = color & mask1;
        uint32_t green = color & mask2;
        uint32_t red = color & mask3;

        blue = blue >> 16;
        red = red << 16;

        color = mask0 | red | green | blue;
        colors[i] = color;
    }
}

void mesh_load_texture(mesh_t inst, char *filename) {
    upng_t* texture_png = upng_new_from_file(filename);
    if (texture_png == NULL) {
        printf("Error loading texture: Texture is NULL\n");
        return;
    }
    upng_decode(texture_png);
    int error;
    if ( (error = upng_get_error(texture_png)) != UPNG_EOK) {
        printf("Error loading texture: Code %d", error);
        return;
    }

    inst->texture.width = upng_get_width(texture_png);
    inst->texture.height = upng_get_height(texture_png);
    uint32_t* texels = (uint32_t *)upng_get_buffer(texture_png);
    abgr_to_argb(texels,inst->texture.width * inst->texture.height);
    inst->texture.texels = texels;

}

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
    int vertices_uv_size = darray_vec2_t_get_occupied(orig->vertices_uv);
    int faces_size = darray_face_t_get_occupied(orig->faces);

    inst->vertices = darray_vec3_t_create();
    inst->faces = darray_face_t_create();
    inst->texture = orig->texture;
    inst->vertices_uv = darray_vec2_t_create();


    // TODO: Temporary

    for(int i = 0; i < vertices_uv_size; i++){
        vec2_t orig_vertex_uv = darray_vec2_t_get(orig->vertices_uv, i);
        vec2_t dest_vertex_uv = vec2_create(0, 0);
        vec2_copy(orig_vertex_uv, dest_vertex_uv);
        darray_vec2_t_push(inst->vertices_uv, dest_vertex_uv);
    }
    for (int i = 0; i < vertices_size; i++) {
        vec3_t orig_vertex = darray_vec3_t_get(orig->vertices, i);
        vec3_t dest_vertex = vec3_create(0, 0, 0);
        vec3_copy(orig_vertex, dest_vertex);
        darray_vec3_t_push(inst->vertices, dest_vertex);
    }

    for (int i = 0; i < faces_size; i++) {
       face_t orig_face = darray_face_t_get(orig->faces, i);
       face_t dest_face = face_create(0, 0, 0, NULL, NULL, NULL);
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
    int vertices_uv_size = darray_vec2_t_get_occupied(orig->vertices_uv);
    int faces_size = darray_face_t_get_occupied(orig->faces);

    dest->texture = orig->texture;

    // NOTE: Temporary

    for (int i = 0; i < vertices_uv_size; i++) {
        vec2_t orig_vertex_uv = darray_vec2_t_get(orig->vertices_uv, i);
        vec2_t dest_vertex_uv = darray_vec2_t_get(dest->vertices_uv, i);
        vec2_copy(orig_vertex_uv, dest_vertex_uv);
    }


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
 * Sets every face that is not seen by the camera to a non rendarizable value */
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
            face_set(face, 0,0,0,NULL, NULL, NULL);
        }
    }
}

darray_vec3_t mesh_get_vertices(mesh_t inst) {
    return inst->vertices;
}

darray_face_t mesh_get_faces(mesh_t inst) {
    return inst->faces;
}

texture_t* mesh_get_texture(mesh_t inst) {
    return &inst->texture;
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
    //darray_vec2_t vertices_texture = darray_vec2_t_create();
    mesh->vertices_uv = darray_vec2_t_create();
    while (fgets(line, 100, fp)) {
        // TODO: Load values into mesh
        if ((line[0] == 'v') && (line[1] == ' ')) {
            float x;
            float y;
            float z;
            sscanf(line, "v %f %f %f", &x, &y, &z);
            darray_vec3_t_push(mesh->vertices, vec3_create(x, y, z));
            continue;
        }

        if ((line[0] == 'v') && (line[1] == 't')) {
            float u;
            float v;
            sscanf(line, "vt %f %f", &u, &v);
            darray_vec2_t_push(mesh->vertices_uv, vec2_create(u, v));
            continue;
        }

        if (line[0] == 'f') {
            int a;
            int b;
            int c;
            int a_texture;
            int b_texture;
            int c_texture;

            sscanf(line, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d", &a, &a_texture,
                   &b, &b_texture, &c, &c_texture);
            darray_face_t_push(
                mesh->faces,
                face_create(a, b, c,
                            darray_vec2_t_get(mesh->vertices_uv, a_texture - 1),
                            darray_vec2_t_get(mesh->vertices_uv, b_texture - 1),
                            darray_vec2_t_get(mesh->vertices_uv, c_texture - 1)));

            /*
            face_t face = darray_face_t_get(mesh->faces, i);
            a = face_get_a(face);
            b = face_get_b(face);
            c = face_get_c(face);
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
    //darray_vec2_t_destroy(mesh->vertices_uv);
}

void mesh_destroy(mesh_t inst) {

    for (int i = 0; i < darray_vec2_t_get_occupied(inst->vertices_uv) ; i++) {
        vec2_destroy(darray_vec2_t_get(inst->vertices_uv, i));
    }
    darray_vec2_t_destroy(inst->vertices_uv);

    for (int i = 0; i < darray_face_t_get_occupied(inst->faces) ; i++) {
        face_t face = darray_face_t_get(inst->faces, i);

        face_destroy(face);
    }

    darray_face_t_destroy(inst->faces);

    // Destroy vertices ""
    for (int i = 0; i < darray_vec3_t_get_occupied(inst->vertices) ; i++) {
        vec3_destroy(darray_vec3_t_get(inst->vertices, i));
    }
    darray_vec3_t_destroy(inst->vertices);

    //TODO: Free texture

    // Lastly free the mesh structure itself
    free(inst);

}
