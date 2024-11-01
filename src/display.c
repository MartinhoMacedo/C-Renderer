#include <SDL2/SDL.h>
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "clipping.h"
#include "display.h"
#include "mesh.h"
#include "face.h"
#include "texture.h"
#include "vector.h"
#include "macros.h"
#include "triangle.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* framebuffer = NULL;
static SDL_Texture* framebuffer_texture = NULL;

static float* zbuffer = NULL;

static int window_width = 0;
static int window_height = 0;
static float aspect_ratio = 0;
static float aspect_ratio_x = 0;

static mesh_t mesh = NULL;

static bool lines_on = true;
static bool vertices_on = true;
static bool fill_on = true;

static float fovy = M_PI/2;
static float fovx = 0;
static float znear = 1;
static float zfar = 50;

void switch_wireframe() {
    lines_on = !lines_on;
    vertices_on = !vertices_on;
}

void switch_fill() {
    fill_on = !fill_on;
}

void set_render_mode(int mode) {
    switch (mode) {
        case 1:
            lines_on = true;
            vertices_on = true;
            fill_on = false;
            break;
        case 2:
            lines_on = true;
            vertices_on = true;
            fill_on = true;
            break;
        case 3:
            lines_on = false;
            vertices_on = false;
            fill_on = true;
            break;
    }
}

void set_mesh(mesh_t mesh_a) {
    mesh = mesh_a;
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
    aspect_ratio = (float)window_height/ window_width;
    aspect_ratio_x = (float)window_width/window_height;

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
    zbuffer = (float*) malloc(sizeof(float) * window_width * window_height);

    fovx = atan(tan(fovy/2)* aspect_ratio_x) * 2.0;

    // TODO: Pass arguments correctly
    planes_create(fovy, fovx, znear, zfar);
    create_intersection_points();


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

void zbuffer_clear() {
    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            zbuffer[(window_width * y) + x] = 1;
        }
    }
}

void zbuffer_add(int x, int y, float z) {
    if (y < 0 || y >= window_height) {
        // Give some slack for clipping error
        if(y < -1 || y >= window_height+2){
            printf("Adding to zbuffer error: Invalid y: %d\n",y);
        }
        return;
    }
    if (x < 0 || x >= window_width) {
        if(x < -1 || x >= window_height+2){
            printf("Adding to zbuffer error: Invalid x: %d\n",x);
        }
        return;
    }

    // Invert y to draw the pixel in correct position
    y = window_height - y - 1;

    zbuffer[(window_width * y) + x] = z;
}

float zbuffer_get(int x, int y) {
    if (y < 0 || y >= window_height) {
        if(y < -1 || y >= window_height+2){
            printf("Getting zbuffer error: Invalid y: %d\n", y);
        }
        return -11;
    }
    if (x < 0 || x >= window_width) {
        if(x < -1 || x >= window_height+2){
            printf("Getting zbuffer error: Invalid x: %d\n",x);
        }
        return -11;
    }

    // Invert y to get the pixel in correct position
    y = window_height - y - 1;

    return zbuffer[(window_width * y) + x];
}

void draw_pixel(int x, int y, uint32_t color) {
    if (y < 0 || y >= window_height) {
        if(y < -1 || y >= window_height+2){
            printf("Draw pixel error: Invalid y: %d\n",y);
        }
        return;
    }
    if (x < 0 || x >= window_width) {
        if(x < -1 || x >= window_height+2){
            printf("Draw pixel error: Invalid x: %d\n",x);
        }
        return;
    }

    // Invert y to draw the pixel in correct position
    y = window_height - y - 1;

    framebuffer[(window_width * y) + x] = color;
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

void draw_triangle_pixel(int x, int y, vec3_t a, vec3_t b, vec3_t c, uint32_t color) {
    float a_z_inv = 1/vec3_get_z((vec3_t) a);
    float b_z_inv = 1/vec3_get_z((vec3_t) b);
    float c_z_inv = 1/vec3_get_z((vec3_t) c);

    char weights_buffer[vec3_struct_get_size()];
    char p_buffer[vec3_struct_get_size()];

    vec3_t weights = vec3_init(weights_buffer, 0, 0, 0);
    vec3_t p = vec3_init(p_buffer, x, y, 0);

    vec3_barycentric(weights, p, a, b, c);
    float a_w = vec2_get_x((vec2_t) weights);
    float b_w = vec2_get_y((vec2_t) weights);
    float c_w = vec3_get_z( weights);

    // Interpolate z and check z buffer
    // NOTE: We need to interpolate 1/p_z instead because there exists a
    // linear function f(a_z, b_z, c_z) = 1/p_z
    float p_z = 1 / (a_w * a_z_inv + b_w * b_z_inv + c_w * c_z_inv);

    // NOTE: a_w + b_w + c_w should be equal to 1 but some slack is given due to
    // floating point inaccuracies
    if (a_w + b_w + c_w > 1.01) {
        printf("Error calculating barycentric coords, their sum is: %f \n", a_w+b_w+c_w);
    }

    if (p_z < zbuffer_get(x, y)) {
        zbuffer_add(x, y,  p_z);
        draw_pixel(x, y, color);
        return;
    }

    //printf("a_w: %f, b_w: %f, c_w: %f\n", a_w, b_w, c_w);
    //printf("p_z: %f, zbuffer: %f\n", p_z, zbuffer_get(x,y));

}

void draw_triangle_line(int x0, int y0, int x1, int y1, vec3_t a, vec3_t b, vec3_t c, uint32_t color) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int run = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float inc_x =  delta_x / (float) run;
    float inc_y =  delta_y / (float) run;

    float x = x0;
    float y = y0;

    for (int i = 0; i <= run; i++) {
        draw_triangle_pixel(round(x), round(y), a, b, c, color);
        x += inc_x;
        y += inc_y;
    }
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

void draw_triangle_vertices(vec2_t a, vec2_t b, vec2_t c, uint32_t color) {
    float a_x = vec2_get_x(a);
    float a_y = vec2_get_y(a);
    float b_x = vec2_get_x(b);
    float b_y = vec2_get_y(b);
    float c_x = vec2_get_x(c);
    float c_y = vec2_get_y(c);

    // Add vertices to framebuffer
    draw_rect(a_x-2, a_y-2,
                4, 4, color);
    draw_rect(b_x-2, b_y-2,
                4, 4, color);
    draw_rect(c_x-2, c_y-2,
                4, 4, color);

}

void draw_triangle_lines(vec2_t a, vec2_t b, vec2_t c, uint32_t color) {
    float a_x = vec2_get_x(a);
    float a_y = vec2_get_y(a);
    float b_x = vec2_get_x(b);
    float b_y = vec2_get_y(b);
    float c_x = vec2_get_x(c);
    float c_y = vec2_get_y(c);

    // Add lines to framebuffer
    draw_line(a_x, a_y, b_x, b_y, color);
    draw_line(b_x, b_y, c_x, c_y, color);
    draw_line(c_x, c_y, a_x, a_y, color);
}


// Draw triangle abM
void fill_flat_bottom_triangle(vec3_t a, vec3_t b, vec3_t M, vec3_t c, uint32_t color) {
    int a_x = vec2_get_x((vec2_t) a);
    int a_y = vec2_get_y((vec2_t) a);
    int b_x = vec2_get_x((vec2_t) b);
    int b_y = vec2_get_y((vec2_t) b);
    int M_x = vec2_get_x((vec2_t) M);
    int M_y = vec2_get_y((vec2_t) M);

    float start_x = a_x;
    int start_y = a_y;
    float end_x = a_x;
    int end_y = M_y;

    // The inverted slopes (run over rise) between point a and b, and between a and c.
    // NOTE: Division is expensive
    float slope_start = (float)(b_x - a_x) / -(float)(b_y - a_y);
    float slope_end = (float)(M_x - a_x) / -(float)(M_y - a_y);



    for (int y = start_y; y >= end_y; y--) {
        //draw_line(start_x, y, end_x, y, color);
        draw_triangle_line(start_x, y, end_x, y, a, b, c, color);
        /*int aux_start_x = start_x;
        int aux_end_x = end_x;
        if (aux_start_x > aux_end_x) {
            int tmp = aux_start_x;
            aux_start_x = aux_end_x;
            aux_end_x = tmp;
        }
        for (int x = aux_start_x; x <= aux_end_x; x++) {
            draw_triangle_pixel(round(x), round(y), a, b, c, color);
        }*/
        start_x += slope_start;
        end_x += slope_end;
    }
}

// Draw triangle aMc
void fill_flat_top_triangle(vec3_t b, vec3_t M, vec3_t c, vec3_t a, uint32_t color) {
    int b_x = vec2_get_x((vec2_t) b);
    int b_y = vec2_get_y((vec2_t) b);
    int M_x = vec2_get_x((vec2_t) M);
    int M_y = vec2_get_y((vec2_t) M);
    int c_x = vec2_get_x((vec2_t) c);
    int c_y = vec2_get_y((vec2_t) c);

    float start_x = b_x;
    int start_y = b_y;
    float end_x = M_x;
    int end_y = c_y;

    // The inverted slopes (run over rise) between point a and b, and between a and c.
    // NOTE: Division is expensive
    float slope_start = (float)(c_x - b_x) / -(float)(c_y - b_y);
    float slope_end = (float)(c_x - M_x) / -(float)(c_y - M_y);

    for (int y = start_y; y >= end_y; y--) {
        //draw_line(start_x, y, end_x, y, color);
        draw_triangle_line(start_x, y, end_x, y, a, b, c, color);
        /*int aux_start_x = start_x;
        int aux_end_x = end_x;
        if (aux_start_x > aux_end_x) {
            int tmp = aux_start_x;
            aux_start_x = aux_end_x;
            aux_end_x = tmp;
        }
        for (int x = aux_start_x; x <= aux_end_x; x++) {
            draw_triangle_pixel(round(x), round(y), a, b, c, color);
        }*/
        start_x += slope_start;
        end_x += slope_end;
    }
}

void draw_texel(int x, int y, vec3_t a, vec3_t b, vec3_t c, vec2_t a_uv,
                vec2_t b_uv, vec2_t c_uv,
                texture_t* texture) {
    float a_z_inv = 1/vec3_get_z((vec3_t) a);
    float b_z_inv = 1/vec3_get_z((vec3_t) b);
    float c_z_inv = 1/vec3_get_z((vec3_t) c);
    float a_u = vec2_get_x(a_uv);
    float b_u = vec2_get_x(b_uv);
    float c_u = vec2_get_x(c_uv);
    float a_v = vec2_get_y(a_uv);
    float b_v = vec2_get_y(b_uv);
    float c_v = vec2_get_y(c_uv);
    float texture_width = texture->width;
    float texture_height = texture->height;

    char weights_buffer[vec3_struct_get_size()];
    char p_buffer[vec3_struct_get_size()];

    vec3_t weights = vec3_init(weights_buffer, 0, 0, 0);
    vec3_t p = vec3_init(p_buffer, x, y, 0);

    vec3_barycentric(weights, p, a, b, c);
    float a_w = vec2_get_x((vec2_t) weights);
    float b_w = vec2_get_y((vec2_t) weights);
    float c_w = vec3_get_z( weights);

    // Interpolate z and check z buffer
    // NOTE: We need to interpolate 1/p_z instead because there exists a
    // linear function f(a_z, b_z, c_z) = 1/p_z
    float p_z = 1 / (a_w * a_z_inv + b_w * b_z_inv + c_w * c_z_inv);

    // Interpolate uv
    float p_u = a_w * a_u + b_w * b_u + c_w * c_u;
    float p_v = a_w * a_v + b_w * b_v + c_w * c_v;

    int texel_x = p_u * texture_width;
    //int texel_y =  p_v * texture_height;
    int texel_y = texture_height - p_v * texture_height - 1;

    // NOTE: a_w + b_w + c_w should be equal to 1 but some slack is given due to
    // floating point inaccuracies
    if (a_w + b_w + c_w > 1.01) {
        printf("Error calculating barycentric coords, their sum is: %f \n", a_w+b_w+c_w);
    }

    if (p_z < zbuffer_get(x, y)) {
        zbuffer_add(x, y,  p_z);
        //int texel = texture_width * texture_height - (texel_y * texture_width + texel_x) - 1;
        int texel = texel_y * texture_width + texel_x;
        if (texel > texture_width * texture_height - 1 || texel < 0) {
            //printf("Error drawing texel: Invalid uv: (%d, %d) \n", texel_x, texel_y);
            return;
        }
        draw_pixel(x, y, texture->texels[texel]);
        //draw_pixel(x, y, 0xFFFF0000);
        return;
    }
    //printf("a_w: %f, b_w: %f, c_w: %f\n", a_w, b_w, c_w);
    //printf("p_z: %f, zbuffer: %f\n", p_z, zbuffer_get(x,y));
}

// Draw triangle abM
void texture_flat_bottom_triangle(vec3_t a, vec3_t b, vec3_t M, vec3_t c,
                                  vec2_t a_uv, vec2_t b_uv,
                                  vec2_t c_uv,texture_t* texture) {

    int a_x = vec2_get_x((vec2_t) a);
    int a_y = vec2_get_y((vec2_t) a);
    int b_x = vec2_get_x((vec2_t) b);
    int b_y = vec2_get_y((vec2_t) b);
    int M_x = vec2_get_x((vec2_t) M);
    int M_y = vec2_get_y((vec2_t) M);

    float start_x = a_x;
    int start_y = a_y;
    float end_x = a_x;
    int end_y = M_y;

    // The inverted slopes (run over rise) between point a and b, and between a and c.
    // NOTE: Division is expensive
    float slope_start = (float)(b_x - a_x) / -(float)(b_y - a_y);
    float slope_end = (float)(M_x - a_x) / -(float)(M_y - a_y);



    for (int y = start_y; y >= end_y; y--) {
        int aux_start_x = start_x;
        int aux_end_x = end_x;
        if (aux_start_x > aux_end_x) {
            int tmp = aux_start_x;
            aux_start_x = aux_end_x;
            aux_end_x = tmp;
        }
        for (int x = aux_start_x; x <= aux_end_x; x++) {
            draw_texel(x, y, a, b, c, a_uv, b_uv, c_uv, texture);
        }
        start_x += slope_start;
        end_x += slope_end;
    }
}

// Draw triangle aMc
void texture_flat_top_triangle(vec3_t b, vec3_t M, vec3_t c, vec3_t a,
                               vec2_t a_uv,
                               vec2_t b_uv, vec2_t c_uv, texture_t* texture) {
    int b_x = vec2_get_x((vec2_t) b);
    int b_y = vec2_get_y((vec2_t) b);
    int M_x = vec2_get_x((vec2_t) M);
    int M_y = vec2_get_y((vec2_t) M);
    int c_x = vec2_get_x((vec2_t) c);
    int c_y = vec2_get_y((vec2_t) c);

    float start_x = b_x;
    int start_y = b_y;
    float end_x = M_x;
    int end_y = c_y;

    // The inverted slopes (run over rise) between point a and b, and between a and c.
    // NOTE: Division is expensive
    float slope_start = (float)(c_x - b_x) / -(float)(c_y - b_y);
    float slope_end = (float)(c_x - M_x) / -(float)(c_y - M_y);

    // TODO: Fix subpixel precision
    for (int y = start_y; y > end_y; y--) {
        int aux_start_x = start_x;
        int aux_end_x = end_x;
        if (aux_start_x > aux_end_x) {
            int tmp = aux_start_x;
            aux_start_x = aux_end_x;
            aux_end_x = tmp;
        }
        for (int x = aux_start_x; x < aux_end_x; x++) {
            draw_texel(x, y, a, b, c, a_uv, b_uv, c_uv, texture);
        }
        start_x += slope_start;
        end_x += slope_end;
    }
}


void draw_textured_triangle(vec3_t a, vec3_t b, vec3_t c, vec2_t a_uv,
                            vec2_t b_uv, vec2_t c_uv, texture_t* texture) {
    // Get sorted vertices so that a_y > b_y > c_y
    //
    //

    // Manual bubble sort
    // TODO: This is simple enough for this but maybe create separate sorting functions
    if (vec2_get_y((vec2_t) b) > vec2_get_y((vec2_t) a)) {
        vec3_t tmp = a;
        a = b;
        b = tmp;

        vec2_t tmp_uv = a_uv;
        a_uv = b_uv;
        b_uv = tmp_uv;
    }
    if (vec2_get_y((vec2_t) c) > vec2_get_y((vec2_t) b)) {
        vec3_t tmp = b;
        b = c;
        c = tmp;

        vec2_t tmp_uv = b_uv;
        b_uv = c_uv;
        c_uv = tmp_uv;
    }
    // 2nd pass
    if (vec2_get_y((vec2_t) b) > vec2_get_y((vec2_t) a)) {
        vec3_t tmp = a;
        a = b;
        b = tmp;

        vec2_t tmp_uv = a_uv;
        a_uv = b_uv;
        b_uv = tmp_uv;
    }

    float a_x = vec2_get_x((vec2_t) a);
    float b_x = vec2_get_x((vec2_t) b);
    float c_x = vec2_get_x((vec2_t) c);
    float a_y = vec2_get_y((vec2_t) a);
    float b_y = vec2_get_y((vec2_t) b);
    float c_y = vec2_get_y((vec2_t) c);

    float M_y = b_y;
    float M_x = ((c_x - a_x) * (b_y - a_y)) / (c_y - a_y) + a_x;

    // NOTE: This kind of allocation is expensive
    //vec2_t M = vec2_create(M_x, M_y);
    char M_buffer[vec3_struct_get_size()];

    vec3_t M = vec3_init(&M_buffer, M_x, M_y, 0);


    texture_flat_bottom_triangle(a, b, M, c, a_uv, b_uv, c_uv, texture);
    texture_flat_top_triangle(b, M, c, a, a_uv, b_uv, c_uv, texture);
}


void draw_filled_triangle(vec3_t a, vec3_t b, vec3_t c, uint32_t color) {
    // Get sorted vertices so that a_y > b_y > c_y
    //
    //

    // Manual bubble sort
    // TODO: This is simple enough for this but maybe create separate sorting functions
    if (vec2_get_y((vec2_t) b) > vec2_get_y((vec2_t) a)) {
        vec3_t tmp = a;
        a = b;
        b = tmp;
    }
    if (vec2_get_y((vec2_t) c) > vec2_get_y((vec2_t) b)) {
        vec3_t tmp = b;
        b = c;
        c = tmp;
    }
    // 2nd pass
    if (vec2_get_y((vec2_t) b) > vec2_get_y((vec2_t) a)) {
        vec3_t tmp = a;
        a = b;
        b = tmp;
    }

    float a_x = vec2_get_x((vec2_t) a);
    float b_x = vec2_get_x((vec2_t) b);
    float c_x = vec2_get_x((vec2_t) c);
    float a_y = vec2_get_y((vec2_t) a);
    float b_y = vec2_get_y((vec2_t) b);
    float c_y = vec2_get_y((vec2_t) c);

    /*draw_triangle_line(a_x, a_y, b_x, b_y, a, b, c, color);
    draw_triangle_line(b_x, b_y, c_x, c_y, a, b, c, color);
    draw_triangle_line(c_x, c_y, a_x, a_y, a, b, c, color);
*/
    float M_y = b_y;
    float M_x = ((c_x - a_x) * (b_y - a_y)) / (c_y - a_y) + a_x;

    // NOTE: This kind of allocation is expensive
    //vec2_t M = vec2_create(M_x, M_y);
    char M_buffer[vec3_struct_get_size()];

    vec3_t M = vec3_init(&M_buffer, M_x, M_y, 0);


    fill_flat_bottom_triangle(a, b, M, c, color);
    fill_flat_top_triangle(b, M, c, a, color);

    //vec2_destroy(M);
}

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

    //NOTE: Allocating and destroying this temp vectors everytime is extremely inefficient.
    //TODO: Maybe create a pool allocator memory management system.
    /*vec2_t a_proj = vec2_create(0,0);
    vec2_t b_proj = vec2_create(0,0);
    vec2_t c_proj = vec2_create(0,0);
    */
    char a_proj_buffer[vec3_struct_get_size()];
    char b_proj_buffer[vec3_struct_get_size()];
    char c_proj_buffer[vec3_struct_get_size()];

    vec3_t a_proj = (vec3_t) a_proj_buffer;
    vec3_t b_proj = (vec3_t) b_proj_buffer;
    vec3_t c_proj = (vec3_t) c_proj_buffer;

    vec3_project(a, M_PI/2, 50, 1,a_proj);
    vec3_project(b, M_PI/2, 50, 1, b_proj);
    vec3_project(c, M_PI/2, 50, 1, c_proj);

    // Convert from NDC to display space

    // Sccale to screen size
    vec2_scale((vec2_t) a_proj,
               ((float)window_width / 2) * aspect_ratio,
               (float)window_height / 2,
               (vec2_t) a_proj);
    vec2_scale((vec2_t) b_proj,
               (float)window_width / 2 * aspect_ratio,
               (float)window_height / 2,
               (vec2_t) b_proj);
    vec2_scale((vec2_t) c_proj, (float)window_width / 2 * aspect_ratio,
               (float)window_height / 2,
               (vec2_t) c_proj);

    // Translate to the middle of the screen
    vec2_add((vec2_t) a_proj, window_width/2, window_height/2, (vec2_t) a_proj);
    vec2_add((vec2_t) b_proj, window_width/2,window_height/2, (vec2_t) b_proj);
    vec2_add((vec2_t) c_proj, window_width/2, window_height/2, (vec2_t) c_proj);

    if(fill_on){
        if (a_idx % 2 == 0) {
            draw_filled_triangle(a_proj, b_proj, c_proj, 0xFF0000FF);
        } else {
            draw_filled_triangle(a_proj, b_proj, c_proj, 0xFFFF0000);
        }
    }
    // TODO: draw triangle lines and triangle vertices of all triangles outside of here after rasterization
    if (lines_on) {
        draw_triangle_lines((vec2_t) a_proj, (vec2_t) b_proj, (vec2_t) c_proj, 0xFFFFFFFF);
    }
    if (vertices_on) {
        draw_triangle_vertices((vec2_t) a_proj, (vec2_t) b_proj, (vec2_t) c_proj, 0xFFFFFFFF);
    }

    /*vec2_destroy(a_proj);
    vec2_destroy(b_proj);
    vec2_destroy(c_proj);
    */
    /*printf("Drawing face %d: a: (%f, %f)\n", face,
            vec2_get_x(a_proj), vec2_get_y(a_proj));
    printf("Drawing face %d: b: (%f, %f)\n", face,
            vec2_get_x(b_proj), vec2_get_y(b_proj));
    printf("Drawing face %d: c: (%f, %f)\n", face,
            vec2_get_x(c_proj), vec2_get_y(c_proj));*/
}





//TODO: Maybe create a drawer class
void draw_triangle(triangle_t* triangle, texture_t* texture) {
    static int count = 0;

    // Get triangles vertices coordinates
    vec3_t a = triangle->vertices[0];
    vec3_t b = triangle->vertices[1];
    vec3_t c = triangle->vertices[2];
    // Calculate the projected 2 dimensional equivalent vertices

    //NOTE: Allocating and destroying this temp vectors everytime is extremely inefficient.
    //TODO: Maybe create a pool allocator memory management system.
    /*vec2_t a_proj = vec2_create(0,0);
    vec2_t b_proj = vec2_create(0,0);
    vec2_t c_proj = vec2_create(0,0);
    */
    char a_proj_buffer[vec3_struct_get_size()];
    char b_proj_buffer[vec3_struct_get_size()];
    char c_proj_buffer[vec3_struct_get_size()];

    vec3_t a_proj = (vec3_t) a_proj_buffer;
    vec3_t b_proj = (vec3_t) b_proj_buffer;
    vec3_t c_proj = (vec3_t) c_proj_buffer;

    vec3_project(a, fovy, zfar, znear,a_proj);
    vec3_project(b, fovy, zfar, znear, b_proj);
    vec3_project(c, fovy, zfar, znear, c_proj);

    // Convert from NDC to display space

    // Sccale to screen size
    vec2_scale((vec2_t) a_proj,
               ((float)window_width / 2) * aspect_ratio,
               (float)window_height / 2,
               (vec2_t) a_proj);
    vec2_scale((vec2_t) b_proj,
               (float)window_width / 2 * aspect_ratio,
               (float)window_height / 2,
               (vec2_t) b_proj);
    vec2_scale((vec2_t) c_proj, (float)window_width / 2 * aspect_ratio,
               (float)window_height / 2,
               (vec2_t) c_proj);

    // Translate to the middle of the screen
    vec2_add((vec2_t) a_proj, window_width/2, window_height/2, (vec2_t) a_proj);
    vec2_add((vec2_t) b_proj, window_width/2,window_height/2, (vec2_t) b_proj);
    vec2_add((vec2_t) c_proj, window_width/2, window_height/2, (vec2_t) c_proj);

    // Round to nearest display pixel
    //vec2_round((vec2_t) a_proj);
    //vec2_round((vec2_t) b_proj);
    //vec2_round((vec2_t) c_proj);

    if(fill_on){
        if (count % 2 == 0) {
            //draw_filled_triangle(a_proj, b_proj, c_proj, 0xFF0000FF);
        }
        //draw_filled_triangle(a_proj, b_proj, c_proj, 0xFFFF0000);
        count++;

        draw_textured_triangle(a_proj, b_proj, c_proj, triangle->vertices_uv[0],
                               triangle->vertices_uv[1], triangle->vertices_uv[2],
                               texture);
    }
    // TODO: draw triangle lines and triangle vertices of all triangles outside of here after rasterization
    if (lines_on) {
        draw_triangle_lines((vec2_t) a_proj, (vec2_t) b_proj, (vec2_t) c_proj, 0xFFFFFFFF);
    }
    if (vertices_on) {
        draw_triangle_vertices((vec2_t) a_proj, (vec2_t) b_proj, (vec2_t) c_proj, 0xFFFFFFFF);
    }
}


void draw_mesh(mesh_t mesh) {
    darray_face_t faces = mesh_get_faces(mesh);
    darray_vec3_t vertices = mesh_get_vertices(mesh);
    int faces_size = darray_face_t_get_occupied(faces);
    texture_t* texture = mesh_get_texture(mesh);

    // Cicle faces
    for (int i = 0; i < faces_size; i++) {
    //for (int i = 1; i < 2; i++) {

        face_t face = darray_face_t_get(faces, i);
        triangle_t triangles[10] = {0};
        int triangles_count = 0;
        clip_face(face, vertices, triangles, &triangles_count);
        //draw_face(face, vertices);
        for (int i = 0; i < triangles_count; i++) {
            draw_triangle(&triangles[i], texture);
        }
        reset_intersection_points();
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
    zbuffer_clear();
    draw_mesh(mesh);

    render_framebuffer();
    clear_framebuffer(0xFF000000);

    SDL_RenderPresent(renderer);

}

void destroy_display(void) {
    SDL_DestroyTexture(framebuffer_texture);
    SDL_DestroyRenderer(renderer);
    free(zbuffer);
    destroy_intersection_points();
    free(framebuffer);
    SDL_DestroyWindow(window);
}
