#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 600

typedef struct {
    int x, y, w, h;
    bool active;
} Bullet;

typedef struct {
    int x, y, w, h;
    bool active;
} Enemy;

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Shooting Game",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // プレイヤー
    SDL_Rect player = {SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT - 40, 40, 20};

    // 弾と敵
    Bullet bullets[100] = {0};
    Enemy enemies[50] = {0};

    bool running = true;
    int frame = 0;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    // 弾を追加
                    for (int i = 0; i < 100; i++) {
                        if (!bullets[i].active) {
                            bullets[i].x = player.x + player.w/2 - 2;
                            bullets[i].y = player.y;
                            bullets[i].w = 4;
                            bullets[i].h = 10;
                            bullets[i].active = true;
                            break;
                        }
                    }
                }
            }
        }

        // キー状態
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] && player.x > 0) player.x -= 5;
        if (state[SDL_SCANCODE_RIGHT] && player.x < SCREEN_WIDTH - player.w) player.x += 5;

        // 弾を更新
        for (int i = 0; i < 100; i++) {
            if (bullets[i].active) {
                bullets[i].y -= 8;
                if (bullets[i].y < 0) bullets[i].active = false;
            }
        }

        // 敵を生成
        if (frame % 60 == 0) {
            for (int i = 0; i < 50; i++) {
                if (!enemies[i].active) {
                    enemies[i].x = rand() % (SCREEN_WIDTH - 20);
                    enemies[i].y = 0;
                    enemies[i].w = 20;
                    enemies[i].h = 20;
                    enemies[i].active = true;
                    break;
                }
            }
        }

        // 敵を更新
        for (int i = 0; i < 50; i++) {
            if (enemies[i].active) {
                enemies[i].y += 2;
                if (enemies[i].y > SCREEN_HEIGHT) enemies[i].active = false;
            }
        }

        // 当たり判定
        for (int i = 0; i < 100; i++) {
            if (bullets[i].active) {
                SDL_Rect b = {bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h};
                for (int j = 0; j < 50; j++) {
                    if (enemies[j].active) {
                        SDL_Rect en = {enemies[j].x, enemies[j].y, enemies[j].w, enemies[j].h};
                        if (SDL_HasIntersection(&b, &en)) {
                            bullets[i].active = false;
                            enemies[j].active = false;
                        }
                    }
                }
            }
        }

        // 描画
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // プレイヤー
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        SDL_RenderFillRect(ren, &player);

        // 弾
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        for (int i = 0; i < 100; i++) {
            if (bullets[i].active) {
                SDL_Rect b = {bullets[i].x, bullets[i].y, bullets[i].w, bullets[i].h};
                SDL_RenderFillRect(ren, &b);
            }
        }

        // 敵
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        for (int i = 0; i < 50; i++) {
            if (enemies[i].active) {
                SDL_Rect en = {enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h};
                SDL_RenderFillRect(ren, &en);
            }
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
        frame++;
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
