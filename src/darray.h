#ifndef DARRAY_H_
#define DARRAY_H_
#include <stdlib.h>
#include "vector.h"
#include "mesh.h"
// TEMPLATES

#define darray_struct(T)                                                       \
  struct darray_##T##_inst_t {                                                 \
    size_t capacity;                                                           \
    size_t occupied;                                                           \
    T *first_element;                                                          \
  }

#define darray_object(T) typedef struct darray_##T##_inst_t *darray_##T

#define darray_function_signatures(T)                                          \
  darray_##T darray_##T##_create(void);                                    \
  void darray_##T##_set(darray_##T inst, size_t idx, T element);           \
  void darray_##T##_push(darray_##T inst, T element);                      \
  T darray_##T##_get(darray_##T inst, int idx);                            \
  size_t darray_##T##_get_capacity(darray_##T inst);                       \
  size_t darray_##T##_get_occupied(darray_##T inst);

#define darray_create_function(T)                                              \
  darray_##T darray_##T##_create(void) {                                   \
    darray_##T inst = calloc(1, sizeof(struct darray_##T##_inst_t));       \
    inst->capacity = 2;                                                        \
    inst->first_element = calloc(inst->capacity, sizeof(T));                   \
    inst->occupied = 0;                                                        \
    return inst;                                                               \
  }

#define darray_set_function(T)                                                 \
  void darray_##T##_set(darray_##T inst, size_t idx, T element) {          \
    inst->first_element[idx] = element;                                        \
  }

#define darray_push_function(T)                                                \
  void darray_##T##_push(darray_##T inst, T element) {                     \
    if (inst->occupied == inst->capacity) {                                    \
      inst->capacity *= 2;                                                     \
      inst->first_element =                                                    \
          realloc(inst->first_element, inst->capacity * sizeof(T));            \
    }                                                                          \
    darray_##T##_set(inst, inst->occupied++, element);                         \
  }

#define darray_get_function(T)                                                 \
  T darray_##T##_get(darray_##T inst, int idx) {                           \
    return inst->first_element[idx];                                           \
  }

#define darray_get_capacity_function(T)                                         \
  size_t darray_##T##_get_capacity(darray_##T inst) { return inst->capacity; }

#define darray_get_occupied_function(T)                                        \
  size_t darray_##T##_get_occupied(darray_##T inst) { return inst->occupied; }

#define darray_definition(T)                                                   \
  darray_struct(T);                                                            \
  darray_create_function(T) darray_set_function(T) darray_push_function(T)     \
  darray_get_function(T) darray_get_occupied_function(T) darray_get_capacity_function(T)

#define darray_header(T) darray_object(T); darray_function_signatures(T)

// SIGNATURES
darray_header(int)
darray_header(vec3_t)
darray_header(face_t)

#endif // DARRAY_H_
