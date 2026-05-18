#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <raylib.h>

#define SCREEN_W 540
#define SCREEN_H 360

#define DEFAULT_FPS 60

#define MAZE_H 25
#define MAZE_W 25

typedef uint8_t u8;

typedef enum {
    WALL=0, PATH=1, END=2
} maze_layout;

static u8 maze[MAZE_H][MAZE_W] = {
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

double get_time_diff(struct timespec* pre, struct timespec* post) {
    double pre_f = (double)pre->tv_sec + (double)(pre->tv_nsec*1e-9);
    double post_f = (double)post->tv_sec + (double)(post->tv_nsec*1e-9);
    return post_f - pre_f;
}

void display_maze() {
    size_t scale_maze_h = SCREEN_W / MAZE_H;
    size_t scale_maze_w = SCREEN_H / MAZE_W;

    for (size_t i = 0; i < MAZE_H; ++i) {
        for (size_t j = 0; j < MAZE_W; ++j) {
            if (maze[j][i] == 0) DrawRectangle(scale_maze_h*i, scale_maze_w*j, scale_maze_h*(i+1), scale_maze_w*(j+1), RED);
            else if (maze[j][i] == 1) DrawRectangle(scale_maze_h*i, scale_maze_w*j, scale_maze_h*(i+1), scale_maze_w*(j+1), BLUE);
            else DrawRectangle(scale_maze_h*i, scale_maze_w*j, scale_maze_h*(i+1), scale_maze_w*(j+1), GREEN);
        }
    }
}

void random_walk(Vector2* start_pos, int *maze_complete, struct timespec* pre, struct timespec* post, double *time_taken) {
    size_t scale_maze_h = SCREEN_W / MAZE_H;
    size_t scale_maze_w = SCREEN_H / MAZE_W;
    u8 section = maze[(size_t)start_pos->x][(size_t)start_pos->y];
    if (section == END) {
        if (*maze_complete == 0) {
            *maze_complete=1;
            timespec_get(post, TIME_UTC);
            *time_taken = get_time_diff(pre, post);
        }
        return;
    }

    size_t valid_offset=0;
    Vector2 valid[4];
    memset(valid, 0, sizeof(valid));
    if (start_pos->x-1 >= 0 && (maze[(int)start_pos->x-1][(int)start_pos->y] == PATH || maze[(int)start_pos->x-1][(int)start_pos->y] == END)) valid[valid_offset++]=(Vector2){start_pos->x-1, start_pos->y};
    if (start_pos->x+1 < MAZE_W && (maze[(int)start_pos->x+1][(int)start_pos->y] == PATH || maze[(int)start_pos->x+1][(int)start_pos->y] == END)) valid[valid_offset++]=(Vector2){start_pos->x+1, start_pos->y};
    if (start_pos->y-1 >= 0 && (maze[(int)start_pos->x][(int)start_pos->y-1] == PATH || maze[(int)start_pos->x][(int)start_pos->y-1] == END)) valid[valid_offset++]=(Vector2){start_pos->x, start_pos->y-1};
    if (start_pos->y+1 < MAZE_H && (maze[(int)start_pos->x][(int)start_pos->y+1] == PATH || maze[(int)start_pos->x][(int)start_pos->y+1] == END)) valid[valid_offset++]=(Vector2){start_pos->x, start_pos->y+1};
    if (valid_offset > 0) {
        *start_pos = valid[rand()%valid_offset];
        section = maze[(size_t)start_pos->x][(size_t)start_pos->y];
    }
}

void draw_random_walk(const Vector2* start_pos) {
    size_t scale_maze_h = SCREEN_W / MAZE_H;
    size_t scale_maze_w = SCREEN_H / MAZE_W;

    DrawRectangle(start_pos->y*scale_maze_h, start_pos->x*scale_maze_w, scale_maze_h, scale_maze_w, BLACK);
}

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Maze");
    SetTargetFPS(DEFAULT_FPS);
    srand(time(0));
    Vector2 start_pos = {0, 7};
    int maze_complete=0;
    double time_taken=0.0f;
    struct timespec prev;
    struct timespec post;
    timespec_get(&prev, TIME_UTC);
    while (!WindowShouldClose()) {
        // update
        random_walk(&start_pos, &maze_complete, &prev, &post, &time_taken);
        BeginDrawing();
        ClearBackground(RAYWHITE);
        display_maze();
        draw_random_walk(&start_pos);
        if (maze_complete) {
            DrawText(TextFormat("Maze complete! Time taken: %.02fs", time_taken), 20, 20, 20, BLACK);
        }
        EndDrawing();
    }
    CloseWindow();
}
