#include <SDL2/SDL.h>
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "display.h"
#include "mesh.h"
#include "face.h"
#include "vector.h"
#include "macros.h"


static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* framebuffer = NULL;
static SDL_Texture* framebuffer_texture = NULL;

static int window_width = 0;
static int window_height = 0;

static mesh_t mesh = NULL;

void set_mesh(mesh_t mesh_a) {
    mesh = mesh_a;
}

// DDA ALGO
void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int run = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float inc_x =  delta_x / (float) run;
    float inc_y =  delta_y / (float) run;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= run; i++) {
        draw_pixel(round(x), round(y), color);
        x += inc_x;
        y += inc_y;
    }
}
//TODO: Maybe create a drawer class
void draw_face(face_t face, darray_vec3_t vertices) {

    // Get triangle vertices indexes
    int a_idx = face_get_a(face)-1;
    int b_idx = face_get_b(face)-1;
    int c_idx = face_get_c(face)-1;

    if ((a_idx < 0) || (b_idx < 0) || (c_idx < 0)) {
        return;
    }

    // Get triangles vertices coordinates
    vec3_t a = darray_vec3_t_get(vertices, a_idx);
    vec3_t b = darray_vec3_t_get(vertices, b_idx);
    vec3_t c = darray_vec3_t_get(vertices, c_idx);

    // Calculate the projected 2 dimensional equivalent vertices
    vec2_t a_proj = vec2_create(0,0);
    vec2_t b_proj = vec2_create(0,0);
    vec2_t c_proj = vec2_create(0,0);
    vec3_project(a, a_proj);
    vec3_project(b, b_proj);
    vec3_project(c, c_proj);

    // Translate to the middle of the screen
    vec2_add(a_proj, window_width/2, window_height/2, a_proj);
    vec2_add(b_proj, window_width/2,window_height/2, b_proj);
    vec2_add(c_proj, window_width/2, window_height/2, c_proj);

    float a_proj_x = vec2_get_x(a_proj);
    float a_proj_y = vec2_get_y(a_proj);
    float b_proj_x = vec2_get_x(b_proj);
    float b_proj_y = vec2_get_y(b_proj);
    float c_proj_x = vec2_get_x(c_proj);
    float c_proj_y = vec2_get_y(c_proj);

    // Add lines to framebuffer
    draw_line(a_proj_x, a_proj_y, b_proj_x, b_proj_y, 0xFF00FF00);
    draw_line(b_proj_x, b_proj_y, c_proj_x, c_proj_y, 0xFF00FF00);
    draw_line(c_proj_x, c_proj_y, a_proj_x, a_proj_y, 0xFF00FF00);

    // Add vertices to framebuffer
    draw_rect(a_proj_x-2, a_proj_y-2,
                4, 4, 0xFF00FF00);
    draw_rect(b_proj_x-2, b_proj_y-2,
                4, 4, 0xFF00FF00);
    draw_rect(c_proj_x-2, c_proj_y-2,
                4, 4, 0xFF00FF00);

    /*printf("Drawing face %d: a: (%f, %f)\n", face,
            vec2_get_x(a_proj), vec2_get_y(a_proj));
    printf("Drawing face %d: b: (%f, %f)\n", face,
            vec2_get_x(b_proj), vec2_get_y(b_proj));
    printf("Drawing face %d: c: (%f, %f)\n", face,
            vec2_get_x(c_proj), vec2_get_y(c_proj));*/
}

void draw_mesh(mesh_t mesh) {
    darray_face_t faces = mesh_get_faces(mesh);
    darray_vec3_t vertices = mesh_get_vertices(mesh);
    int faces_size = darray_face_t_get_occupied(faces);


    // Cicle faces
    for (int i = 0; i < faces_size; i++) {
        face_t face = darray_face_t_get(faces, i);
        draw_face(face, vertices);
    }
}

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

    //draw_rect(300, 200, 300, 150, 0xFFFF00FF);
    if (mesh == NULL) {
        printf("Mesh is NULL\n");
        return;
    }
    draw_mesh(mesh);

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
