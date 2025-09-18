#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_W 400
#define WINDOW_H 600
#define PLAYER_W 40
#define PLAYER_H 20
#define BULLET_W 5
#define BULLET_H 10
#define ENEMY_W 30
#define ENEMY_H 20
#define MAX_BULLETS 50
#define MAX_ENEMIES 20

typedef struct {
    int x, y;
    int active;
} Bullet;

typedef struct {
    int x, y;
    int active;
} Enemy;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("Shooting Game",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        WINDOW_W, WINDOW_H, 0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // フォント読み込み（DejaVuSans.ttf などを同じフォルダに置く）
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 20);
    if (!font) {
        printf("Font Error: %s\n", TTF_GetError());
        return 1;
    }

    srand(time(NULL));

    // プレイヤー
    int playerX = WINDOW_W / 2 - PLAYER_W / 2;
    int playerY = WINDOW_H - 50;

    Bullet bullets[MAX_BULLETS] = {0};
    Enemy enemies[MAX_ENEMIES] = {0};
    int score = 0;

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) running = 0;
                if (e.key.keysym.sym == SDLK_SPACE) {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].x = playerX + PLAYER_W / 2 - BULLET_W / 2;
                            bullets[i].y = playerY;
                            bullets[i].active = 1;
                            break;
                        }
                    }
                }
            }
        }

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT] && playerX > 0) playerX -= 5;
        if (keystate[SDL_SCANCODE_RIGHT] && playerX < WINDOW_W - PLAYER_W) playerX += 5;

        // 弾の更新
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].y -= 5;
                if (bullets[i].y < 0) bullets[i].active = 0;
            }
        }

        // 敵の生成（スロー化）
        if (rand() % 100 =
