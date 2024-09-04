#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct vec2_instance_t* vec2_t;
typedef struct vec3_instance_t* vec3_t;
typedef struct vec4_instance_t* vec4_t;

vec2_t vec2_create(int x, int y);
vec3_t vec3_create(int x, int y, int z);
vec4_t vec4_create(int x, int y, int z, int w);
int vec2_get_x(vec2_t inst);

#endif // VECTOR_H_