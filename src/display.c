#include <SDL2/SDL.h>
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "display.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* framebuffer = NULL;
static SDL_Texture* framebuffer_texture = NULL;

static int window_width = 1280;
static int window_height = 720;


bool init_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL not initialized! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Get display width and height
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    // Create SDL Window
    window =
        SDL_CreateWindow(NULL, 0, 0, window_width, window_height,
                         SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE |
                             SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_INPUT_FOCUS |
                         SDL_WINDOW_MOUSE_FOCUS);
    if (!window) {
        printf("Window not created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Renderer not created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void setup(void) {
    framebuffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

    if (!framebuffer) {
        printf("Error allocating the framebuffer");
    }

    // Creating a SDL texture that is used to display the color buffer
    framebuffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height
    );
}

void draw_pixel(int x, int y, uint32_t color) {
    if (y < 0 || y >= window_height) {
        printf("Draw pixel error: Invalid y: %d\n",y);
        return;
    }
    if (x < 0 || x >= window_width) {
        printf("Draw pixel error: Invalid x: %d\n",x);
        return;
    }

    // Invert y to draw the pixel in correct position
    y = window_height - y - 1;

    framebuffer[(window_width * y) + x] = color;
}

void draw_grid(void) {
    for (int y = 0; y < window_height; y += 10) {
        for (int x = 0; x < window_width; x += 10) {
            draw_pixel(x, y, 0xFF444444);
        }
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void render_framebuffer(void) {
    // Update texture with the framebuffer data
    SDL_UpdateTexture(
        framebuffer_texture,
        NULL,
        framebuffer,
        (int)(window_width * sizeof(uint32_t))
    );
    // Copy the data to be rendered to the renderer
    SDL_RenderCopy(renderer, framebuffer_texture, NULL, NULL);
}

void clear_framebuffer(uint32_t color) {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            framebuffer[(window_width * y) + x] = color;
        }
    }
}


void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid();

    draw_rect(300, 200, 300, 150, 0xFFFF00FF);

    render_framebuffer();
    clear_framebuffer(0xFF000000);

    SDL_RenderPresent(renderer);

}

void destroy_display(void) {
    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyRenderer(renderer);
    free(framebuffer);
    SDL_DestroyWindow(window);
}
