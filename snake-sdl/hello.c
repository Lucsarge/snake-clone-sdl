#include <stdio.h>

#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

#define GAME_NAME "Snake"
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

const int tileSize = 40; // number of pixels each tile the board is wide
int tileMargin = 3; // empty space around each tile when draw to help distinguish tiles

struct SnakePart {
    int x; // x coordinate of the snake head
    int y; // y coordinate of the snake head
};

struct SnakePart head;


void manageInput(SDL_Event *e) {
    int vInput = 0;
    int hInput = 0;

    SDL_assert(e->type == SDL_EVENT_KEY_DOWN); // this assert was made in an example for the basic input handling, might not be necessary to keep
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
        head.y += 40 * vInput;
    }
    else if (hInput != 0) {
        head.x += 40 * hInput;
    }
}

void renderBoard() {
    // clear the board
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // the next SDL_RenderClear call uses the current render draw color so set it to black here
    SDL_RenderClear(renderer);

    // set color to green for drawing the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

    int tileRenderSize = tileSize - (tileMargin * 2);
    SDL_FRect rect = { head.x + tileMargin, head.y + tileMargin, tileRenderSize, tileRenderSize };

    SDL_RenderFillRect(renderer, &rect);

    // set color to red for drawing the apple
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

    SDL_RenderPresent(renderer);
}

#pragma region SDL Callbacks
SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
    SDL_CreateWindowAndRenderer(GAME_NAME, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, &window, &renderer);
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (renderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    renderBoard();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_DOWN) {
        manageInput(event);
    }

    if (event->type == SDL_EVENT_KEY_UP) {
        if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
            return SDL_APP_SUCCESS; // quit when releasing the escape key
        }
    }

    // handle closing the window
    if (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
#pragma endregion
