#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vector.h"
#include "texture.h"


typedef struct {
    vec3_t vertices[3];
    vec2_t vertices_uv[3];
} triangle_t;


#endif // TRIANGLE_H_
