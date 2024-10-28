#ifndef CLIPPING_H_
#define CLIPPING_H_
#include "vector.h"
#include "triangle.h"
#include "face.h"

#define N_PLANES 6
#define MAX_POLY_VERTICES 10

enum {
  LEFT_FRUSTUM_PLANE,
  RIGHT_FRUSTUM_PLANE,
  TOP_FRUSTUM_PLANE,
  BOTTOM_FRUSTUM_PLANE,
  NEAR_FRUSTUM_PLANE,
  FAR_FRUSTUM_PLANE
};

void planes_create(float fovy, float fovx, float z_near, float z_far );
void clip_face(face_t face, darray_vec3_t vertices, triangle_t triangles[],
               int *triangles_count);
void create_intersection_points();
void destroy_intersection_points();
void reset_intersection_points();


#endif // CLIPPING_H_
