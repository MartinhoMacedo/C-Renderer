#ifndef VECTOR_PRIVATE_H_
#define VECTOR_PRIVATE_H_
#include "vector.h"

struct vec2_instance_t {
    float x;
    float y;
};

struct vec3_instance_t {
    vec2_instance_t;
    float z;
};

struct vec4_instance_t {
    vec3_instance_t;
    float w;
};


#endif // VECTOR_PRIVATE_H_
