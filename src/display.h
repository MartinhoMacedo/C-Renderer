#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdbool.h>
#include <stdint.h>

bool init_window(void);
void setup(void);
void draw_grid(void);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void render_framebuffer(void);
void clear_framebuffer(uint32_t color);
void render(void);


#endif // DISPLAY_H_
