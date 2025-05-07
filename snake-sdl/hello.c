#include <stdio.h>
#include <SDL3/SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 512

struct SnakePart {
    int x; // x coordinate of the snake head
    int y; // y coordinate of the snake head
};

struct SnakePart head;

void manageInput(SDL_Event *e) {
    int vInput = 0;
    int hInput = 0;

    SDL_assert(e->type == SDL_EVENT_KEY_DOWN);
    if (e->key.scancode == SDL_SCANCODE_W) {
        printf("'W' key was pressed down\n");
        vInput -= 1;
    }
    else if (e->key.scancode == SDL_SCANCODE_A) {
        printf("'A' key was pressed down\n");
        hInput -= 1;
    }
    else if (e->key.scancode == SDL_SCANCODE_S) {
        printf("'S' key was pressed down\n");
        vInput += 1;
    }
    else if (e->key.scancode == SDL_SCANCODE_D) {
        printf("'D' key was pressed down\n");
        hInput += 1;
    }

    if (vInput != 0) {
        head.y += 50 * vInput;
    }
    else if (hInput != 0) {
        head.x += 50 * hInput;
    }
}

void renderBoard() {
    // clear the board
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

    SDL_FRect rect = { head.x+1, head.y+1, 49, 49 };

    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
}

int main() {
    printf("hello snake\n");
    head.x = 0;
    head.y = 0;

    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer("Snake", 512, 512, NULL, &window, &renderer);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window %s\n", SDL_GetError());
        return 1;
    }
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer %s\n", SDL_GetError());
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
        } // end of event polling

        // Render Grid
        renderBoard();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
