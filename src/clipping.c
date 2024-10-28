#include "clipping.h"
#include "vector.h"
#include "math.h"
#include "face.h"
#include "triangle.h"
#include "string.h"

struct plane_t {
  vec3_t point;
  vec3_t normal;
};

struct polygon_t {
  vec3_t vertices[MAX_POLY_VERTICES];
  int vertices_count;
};

static struct plane_t planes[N_PLANES];
static vec3_t intersection_points[10];
int intersection_count;

void planes_create(float fovy, float fovx, float z_near, float z_far ) {

    vec3_t rplane_point = vec3_create(0, 0, 0);
    vec3_t rplane_vector = vec3_create(-cos(fovx/2), 0, -sin(fovx/2));
    planes[RIGHT_FRUSTUM_PLANE] = (struct plane_t){rplane_point, rplane_vector};

    vec3_t lplane_point = vec3_create(0, 0, 0);
    vec3_t lplane_vector = vec3_create(cos(fovx/2), 0, -sin(fovx/2));
    planes[LEFT_FRUSTUM_PLANE] = (struct plane_t){lplane_point, lplane_vector};

    vec3_t nplane_point = vec3_create(0, 0, -z_near);
    vec3_t nplane_vector = vec3_create(0, 0, -1);
    planes[NEAR_FRUSTUM_PLANE] = (struct plane_t){nplane_point, nplane_vector};

    vec3_t fplane_point = vec3_create(0, 0, -z_far);
    vec3_t fplane_vector = vec3_create(0, 0, 1);
    planes[FAR_FRUSTUM_PLANE] = (struct plane_t){fplane_point, fplane_vector};

    vec3_t tplane_point = vec3_create(0,0,0);
    vec3_t tplane_vector = vec3_create(0, -cos(fovy/2), -sin(fovy/2));
    planes[TOP_FRUSTUM_PLANE] = (struct plane_t){tplane_point, tplane_vector};

    vec3_t bplane_point = vec3_create(0,0,0);
    vec3_t bplane_vector = vec3_create(0, cos(fovy/2), -sin(fovy/2));
    planes[BOTTOM_FRUSTUM_PLANE] = (struct plane_t){bplane_point, bplane_vector};
}

// TODO: Find more elegant solution
void create_intersection_points() {
    for (int i = 0; i < 10; i++) {
        intersection_points[i] = vec3_create(0, 0, 0);
    }
}

void destroy_intersection_points() {
    for (int i = 0; i < 10; i++) {
        vec3_destroy(intersection_points[i]);
    }
    intersection_count = 0;
}

void reset_intersection_points() {
    intersection_count = 0;
}

void clip_triangle(struct polygon_t* polygon, int plane) {
    int polygon_vertices_count = polygon->vertices_count;
    if (polygon_vertices_count == 0) {
        return;
    }

    vec3_t plane_point = planes[plane].point;
    vec3_t plane_normal = planes[plane].normal;

    int inside_vertices_count = 0;
    vec3_t inside_vertices[MAX_POLY_VERTICES] = {0};

    vec3_t previous_vertex = polygon->vertices[polygon->vertices_count - 1];

    char current_vector_buffer[vec3_struct_get_size()];
    char previous_vector_buffer[vec3_struct_get_size()];

    vec3_t current_vector = vec3_init(current_vector_buffer, 0, 0, 0);
    vec3_t previous_vector = vec3_init(previous_vector_buffer, 0, 0, 0);

    vec3_vsub(previous_vertex, plane_point, previous_vector);

    float previous_dot = vec3_dot(previous_vector, plane_normal);

    // Check every pair of close vertices
    for (int i = 0; i < polygon_vertices_count; i++) {
        vec3_t current_vertex = polygon->vertices[i];
        vec3_vsub(current_vertex, plane_point, current_vector);
        float current_dot = vec3_dot(current_vector, plane_normal);

        // Check if the current vertice is not on the same side of the
        // space divided by the plane as the previous vertice
        if (current_dot * previous_dot < 0) {
            // Find interseciton point
            float t = previous_dot / (previous_dot - current_dot);
            vec3_t intersection_point = intersection_points[intersection_count++];
            vec3_vsub(current_vertex, previous_vertex, intersection_point);
            vec3_mul(intersection_point, t, intersection_point);
            vec3_vadd(previous_vertex, intersection_point, intersection_point);

            inside_vertices[inside_vertices_count++] = intersection_point;
        }
        // Check if the current vertice is inside the clipping space
        if (current_dot >= 0) {
            inside_vertices[inside_vertices_count++] = current_vertex;
        }

        previous_vertex = current_vertex;
        previous_dot = current_dot;
    }
    memcpy(polygon->vertices, inside_vertices, sizeof(vec3_t)*inside_vertices_count);
    polygon->vertices_count = inside_vertices_count;
}

void polygon_from_triangle(struct polygon_t* polygon, triangle_t triangle) {
    polygon->vertices[0] = triangle.vertices[0];
    polygon->vertices[1] = triangle.vertices[1];
    polygon->vertices[2] = triangle.vertices[2];
    polygon->vertices[3] = NULL;
    polygon->vertices[4] = NULL;
    polygon->vertices[5] = NULL;
    polygon->vertices[6] = NULL;
    polygon->vertices[7] = NULL;
    polygon->vertices[8] = NULL;
    polygon->vertices[9] = NULL;
    polygon->vertices_count = 3;
}

void triangles_from_polygon(triangle_t triangles[], struct polygon_t* polygon) {
    // build vertices_count-2 triangles
    for (int i = 2; i < polygon->vertices_count; i++) {
        triangles[i-2].vertices[0] = polygon->vertices[0];
        triangles[i-2].vertices[1] = polygon->vertices[i-1];
        triangles[i-2].vertices[2] = polygon->vertices[i];
    }
}

void clip_face(face_t face, darray_vec3_t vertices, triangle_t triangles[],
               int *triangles_count) {

    // Get triangle vertices indexes
    int a_idx = face_get_a(face)-1;
    int b_idx = face_get_b(face)-1;
    int c_idx = face_get_c(face)-1;

    // NOTE: Currently the idx can be less than 0 because backface culling works that way
    if ((a_idx < 0) || (b_idx < 0) || (c_idx < 0)) {
        return;
    }

    // Get triangles vertices coordinates
    triangle_t triangle;
    triangle.vertices[0] = darray_vec3_t_get(vertices, a_idx);
    triangle.vertices[1] = darray_vec3_t_get(vertices, b_idx);
    triangle.vertices[2] = darray_vec3_t_get(vertices, c_idx);

    struct polygon_t polygon;
    polygon_from_triangle(&polygon, triangle);

    clip_triangle(&polygon, RIGHT_FRUSTUM_PLANE);
    clip_triangle(&polygon, LEFT_FRUSTUM_PLANE);
    clip_triangle(&polygon, FAR_FRUSTUM_PLANE);
    clip_triangle(&polygon, NEAR_FRUSTUM_PLANE);
    clip_triangle(&polygon, TOP_FRUSTUM_PLANE);
    clip_triangle(&polygon, BOTTOM_FRUSTUM_PLANE);

    triangles_from_polygon(triangles, &polygon);

    *triangles_count = polygon.vertices_count-2;
}


void planes_destroy() {
    for (int i = 0; i < N_PLANES; i++) {
        free(planes[i].point);
        free(planes[i].normal);
    }
}
