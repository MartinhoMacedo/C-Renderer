#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdbool.h>
#include <stdint.h>
#include "mesh.h"

void set_mesh(mesh_t mesh_a);
bool init_window(void);
void setup(void);

void set_render_mode(int mode);

void render_framebuffer(void);
void clear_framebuffer(uint32_t color);
void render(void);
void destroy_display(void);
#endif // DISPLAY_H_
