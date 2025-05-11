#include <stdio.h>
#include <time.h>

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

struct Tile {
    int x;
    int y;
};

struct Tile body[191] = { NULL }; // (Width / TileSize) * (Height / TileSize), max number of tiles on the board should mean the max number of snake body parts total
int bodyCount;

struct Tile apple;

enum Direction {
    NONE = 0,
    NORTH,
    EAST,
    SOUTH,
    WEST,
};

enum Direction currDir = EAST;

const char* tileStateNames[] = { "Empty", "Snake", "Apple" }; // right now only for debug output purposes
enum TileState {
    EMPTY = 0,
    SNAKE,
    APPLE,
};

enum TileState boardTiles[191] = { EMPTY };

// timing variables
unsigned int lastTime = 0, currentTime;

// n = number of actual elements declared in the original array
void insert(struct Tile arr[], int* n, int pos, struct Tile val) {

    // Shift elements to the right
    for (int i = *n; i > pos; i--)
        arr[i] = arr[i - 1];

    // Insert val at the specified position
    arr[pos] = val;

    // Increase the current size
    (*n)++;
}

void setTile(int xCoord, int yCoord, enum TileState newState) {
    boardTiles[yCoord * 16 + xCoord] = newState;
    printf("Set tile %d, %d to %s\n", xCoord, yCoord, tileStateNames[newState]);
}

void manageInput(SDL_Event *e) {
    int vInput = 0;
    int hInput = 0;

    SDL_assert(e->type == SDL_EVENT_KEY_DOWN); // this assert was made in an example for the basic input handling, might not be necessary to keep
    if (e->key.scancode == SDL_SCANCODE_W) {
        printf("'W' key was pressed down\n");
        currDir = NORTH;
    }
    else if (e->key.scancode == SDL_SCANCODE_A) {
        printf("'A' key was pressed down\n");
        currDir = WEST;
    }
    else if (e->key.scancode == SDL_SCANCODE_S) {
        printf("'S' key was pressed down\n");
        currDir = SOUTH;
    }
    else if (e->key.scancode == SDL_SCANCODE_D) {
        printf("'D' key was pressed down\n");
        currDir = EAST;
    }
}

// renders the snake and the apple
void renderBoard() {
    // clear the board
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // the next SDL_RenderClear call uses the current render draw color so set it to black here
    SDL_RenderClear(renderer);

    // set color to green for drawing the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);

    int tileRenderSize = tileSize - (tileMargin * 2);

    SDL_FRect tile = { 0, 0, tileRenderSize, tileRenderSize };

    for (int i = 0; i < bodyCount; i++) {
        tile.x = body[i].x * tileSize + tileMargin;
        tile.y = body[i].y * tileSize + tileMargin;
        SDL_RenderFillRect(renderer, &tile);
    }

    // set color to red for drawing the apple
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    tile.x = apple.x * tileSize + tileMargin;
    tile.y = apple.y * tileSize + tileMargin;
    SDL_RenderFillRect(renderer, &tile);

    SDL_RenderPresent(renderer);
}

void addBodyPart() {
    struct Tile newHead = { apple.x, apple.y };

    // When eating an apple and increasing the length of the snake we just insert the new head at the apples position
    // and shift every body part down one index in the array
    // bodyCount is incrememnted by one inside the insert function
    insert(body, &bodyCount, 0, newHead);
    setTile(body[0].x, body[0].y, SNAKE);
}

void placeApple() {
    int row = rand() % (11);
    int col = rand() % (15);

    apple.x = col;
    apple.y = row;
    setTile(apple.x, apple.y, APPLE);
}

void moveSnake() {
    // Calculate what the next position that the head should be in
    int nextX = body[0].x;
    int nextY = body[0].y;
    if (currDir == NORTH) {
        nextY--;
    }
    else if (currDir == EAST) {
        nextX++;
    }
    else if (currDir == SOUTH) {
        nextY++;
    }
    else if (currDir == WEST) {
        nextX--;
    }

    // Check if the next head position is overlapping with the current apples position
    int bodyIndex = nextY * 16 + nextX;
    if (boardTiles[bodyIndex] == APPLE) {
        printf("Snake head is overlapping with apple\n");
        addBodyPart();
        printf("Body Count: %d\n", bodyCount);
        placeApple();
    }
    else {
        // starting from tail to head - 1
        for (int i = bodyCount - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }

        // update the snake head
        body[0].x = nextX;
        body[0].y = nextY;
        setTile(body[0].x, body[0].y, SNAKE);
    }
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

    srand(time(NULL)); // re-seed the random number generation

    bodyCount = 1; // start at 1 since the game with always begin with the snakes head, or body[0]
    body[0].x = 0;
    body[0].y = 0;
    setTile(body[0].x, body[0].y, SNAKE);

    placeApple();

    renderBoard(); // call once here when the game starts other wise the board won't render until after the first second

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    currentTime = SDL_GetTicks();
    if (currentTime > lastTime + 375) {
        moveSnake();
        renderBoard();
        lastTime = currentTime;
    }

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
