#include <stdio.h>
#include <SDL3/SDL.h>

void manageInput(SDL_Event *e) {
    SDL_assert(e->type == SDL_EVENT_KEY_DOWN);
    if (e->key.scancode == SDL_SCANCODE_W) {
        printf("'W' key was pressed down\n");
    }
    else if (e->key.scancode == SDL_SCANCODE_A) {
        printf("'A' key was pressed down\n");
    }
    else if (e->key.scancode == SDL_SCANCODE_S) {
        printf("'S' key was pressed down\n");
    }
    else if (e->key.scancode == SDL_SCANCODE_D) {
        printf("'D' key was pressed down\n");
    }
}

int main() {
    printf("hello snake\n");

    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "Snake",
        512,
        512,
        NULL);

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window %s\n", SDL_GetError());
        return 1;
    }

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }

            // keyboard events for input
            if (event.type == SDL_EVENT_KEY_DOWN) {
                manageInput(&event);
            }
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
