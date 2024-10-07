#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_render.h"
#include "display.h"
#include "mesh.h"
#include "vector.h"
#include "macros.h"

bool is_running = false;
bool is_paused = false;

mesh_t mesh = NULL;
mesh_t mesh_transformed = NULL;
vec3_t camera_position = NULL;

bool backface_on = true;

void switch_backface() {
    backface_on = !backface_on;
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                is_running = false;
            if (event.key.keysym.sym == SDLK_SPACE)
                is_paused = !is_paused;
            if (event.key.keysym.sym == SDLK_b)
                switch_backface();
            if (event.key.keysym.sym == SDLK_1)
                set_render_mode(1);
            if (event.key.keysym.sym == SDLK_2)
                set_render_mode(2);
            if (event.key.keysym.sym == SDLK_3)
                set_render_mode(3);
            break;
    }
}

void update(void) {
    // TODO: create mesh_transformer class
    //
    static float translation_x = 0;
    static float translation_y = 0;
    static float translation_z = -CAMERA_SHIFT;
    static float rotation_x = 0;
    static float rotation_y = 0;
    static float rotation_z = 0;
    static float scale_x = 1;
    static float scale_y = 1;
    static float scale_z = 1;


    // reset transformed mesh to origin (mesh space)
    mesh_copy(mesh, mesh_transformed);
    // Apply transformations
    // NOTE: Here the x and y coordinates are inverted because this transformation is before projection
    mesh_transform(mesh_transformed, translation_x += 0.00,
                   translation_y += 0.01, translation_z, rotation_x += 0.01,
                   rotation_y += 0.00, rotation_z += 0.00,
                   scale_x+=0.000, scale_y+=0.000, scale_z+=0.000);

    // Apply backface culling
    if (backface_on) {
        mesh_backface_culling(mesh_transformed, camera_position);
    }
}

int main(void) {
    int previous_frame_time = 0;

    is_running = init_window();

    setup();

    mesh = mesh_create("./assets/f22.obj");
    // TODO: change to mesh_create_clone(mesh)
    mesh_transformed = mesh_create_clone(mesh);

    set_mesh(mesh_transformed);

    camera_position = vec3_create(0, 0, 0);

    while(is_running){
        process_input();
        if(!is_paused && is_running){
            update();
            render();
            // Maintain constant FPS
            int wait_time = TARGET_FRAME_TIME - (SDL_GetTicks() - previous_frame_time);
            if (wait_time > 0 && wait_time <= TARGET_FRAME_TIME) {
                SDL_Delay(wait_time);
            }

            previous_frame_time = SDL_GetTicks();
        }
    }

    printf("Renderer exited.");

    destroy_display();
    mesh_destroy(mesh);
    mesh_destroy(mesh_transformed);
    return 0;
}
