#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <stdint.h>

typedef struct {
  uint32_t *texels;
  int width;
  int height;
} texture_t;




#endif // TEXTURE_H_
