#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_render.h"
#include "display.h"
#include "mesh.h"
#include "vector.h"
#include "macros.h"

bool is_running = false;

mesh_t mesh = NULL;
mesh_t mesh_transformed = NULL;
vec3_t camera_position = NULL;

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
            break;
    }
}

void update(void) {
    // TODO: create mesh_transformer class
    static float translation_x = 0;
    static float translation_y = 0;
    static float translation_z = -CAMERA_SHIFT;
    static float rotation_x = 0;
    static float rotation_y = 0;
    static float rotation_z = 0;

    // reset transformed mesh to origin (mesh space)
    mesh_copy(mesh, mesh_transformed);
    // Apply transformations
    mesh_transform(mesh_transformed,
                   translation_x, translation_y, translation_z,
                   rotation_x+=0.01, rotation_y+=0.01, rotation_z+=0.01);

    // Apply backface culling
    mesh_backface_culling(mesh_transformed, camera_position);
}

int main(void) {
    int previous_frame_time = 0;

    is_running = init_window();

    setup();

    mesh = mesh_create("./assets/cube.obj");
    // TODO: change to mesh_create_clone(mesh)
    mesh_transformed = mesh_create_clone(mesh);

    set_mesh(mesh_transformed);

    camera_position = vec3_create(0, 0, 0);

    while(is_running){
        process_input();
        update();
        render();

        // Maintain constant FPS
        int wait_time = TARGET_FRAME_TIME - (SDL_GetTicks() - previous_frame_time);
        if (wait_time > 0 && wait_time <= TARGET_FRAME_TIME) {
            SDL_Delay(wait_time);
        }

        previous_frame_time = SDL_GetTicks();

    }

    printf("Renderer exited.");

    destroy_display();
    mesh_destroy(mesh);
    mesh_destroy(mesh_transformed);
    return 0;
}
