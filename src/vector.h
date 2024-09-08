#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct vec2_instance_t* vec2_t;
typedef struct vec3_instance_t* vec3_t;
typedef struct vec4_instance_t* vec4_t;

vec2_t vec2_create(float x, float y);
vec3_t vec3_create(float x, float y, float z);
vec4_t vec4_create(float x, float y, float z, float w);

int vec2_get_x(vec2_t inst);
int vec2_get_y(vec2_t inst);
int vec3_get_z(vec3_t inst);
int vec4_get_w(vec4_t inst);

#endif // VECTOR_H_
