#ifndef DARRAY_H_
#define DARRAY_H_
#include <stdlib.h>
// TEMPLATES

typedef struct darray_inst_t* darray_t;

#define darray_struct(T)                                                       \
  struct darray_##T##_inst_t {                                                 \
    T *first_element;                                                          \
    size_t occupied;                                                           \
    size_t capacity;                                                           \
  }

#define darray_object(T) typedef struct darray_##T##_inst_t *darray_##T##_t

#define darray_function_signatures(T)                                          \
  darray_##T##_t darray_##T##_create(void);                                    \
  void darray_##T##_set(darray_##T##_t inst, size_t idx, T element);           \
  void darray_##T##_push(darray_##T##_t inst, T element);                      \
  T darray_##T##_get(darray_##T##_t inst, int idx);                            \
  size_t darray_##T##_get_capacity(darray_##T##_t inst);                       \
  size_t darray_##T##_get_occupied(darray_##T##_t inst);

#define darray_create_function(T)                                              \
  darray_##T##_t darray_##T##_create(void) {                                   \
    darray_##T##_t inst = calloc(1, sizeof(struct darray_##T##_inst_t));       \
    inst->capacity = 2;                                                        \
    inst->first_element = calloc(inst->capacity, sizeof(T));                   \
    inst->occupied = 0;                                                        \
    return inst;                                                               \
  }

#define darray_set_function(T)                                                 \
  void darray_##T##_set(darray_##T##_t inst, size_t idx, T element) {          \
    inst->first_element[idx] = element;                                        \
  }

#define darray_push_function(T)                                                \
  void darray_##T##_push(darray_##T##_t inst, T element) {                     \
    if (inst->occupied == inst->capacity) {                                    \
      inst->capacity *= 2;                                                     \
      inst->first_element =                                                    \
          realloc(inst->first_element, inst->capacity * sizeof(T));            \
    }                                                                          \
    darray_##T##_set(inst, inst->occupied++, element);                         \
  }

#define darray_get_function(T)                                                 \
  T darray_##T##_get(darray_##T##_t inst, int idx) {                           \
    return inst->first_element[idx];                                           \
  }

#define darray_get_capacity_function(T)                                         \
  size_t darray_##T##_get_capacity(darray_##T##_t inst) { return inst->capacity; }

#define darray_get_occupied_function(T)                                        \
  size_t darray_##T##_get_occupied(darray_##T##_t inst) { return inst->occupied; }

#define darray_definition(T)                                                   \
  darray_struct(T);                                                            \
  darray_create_function(T) darray_set_function(T) darray_push_function(T)     \
  darray_get_function(T) darray_get_occupied_function(T) darray_get_capacity_function(T)


// SIGNATURES
darray_object(int);
darray_function_signatures(int);

#endif // DARRAY_H_
