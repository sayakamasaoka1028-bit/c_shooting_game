#include <SDL.h>
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

    SDL_Window *win = SDL_CreateWindow("Shooting Game",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        WINDOW_W, WINDOW_H, 0);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

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

        // 敵の生成（スロー化：出にくくする）
        if (rand() % 100 == 0) {   // ← 50 → 100 に変更
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (!enemies[i].active) {
                    enemies[i].x = rand() % (WINDOW_W - ENEMY_W);
                    enemies[i].y = 0;
                    enemies[i].active = 1;
                    break;
                }
            }
        }

        // 敵の更新（スロー化：落下スピードを遅くする）
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                enemies[i].y += 1;  // ← 2 → 1 に変更
                if (enemies[i].y > WINDOW_H) enemies[i].active = 0;
            }
        }

        // 当たり判定
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                for (int j = 0; j < MAX_ENEMIES; j++) {
                    if (enemies[j].active &&
                        bullets[i].x < enemies[j].x + ENEMY_W &&
                        bullets[i].x + BULLET_W > enemies[j].x &&
                        bullets[i].y < enemies[j].y + ENEMY_H &&
                        bullets[i].y + BULLET_H > enemies[j].y) {
                        bullets[i].active = 0;
                        enemies[j].active = 0;
                        score += 10;   // ★ 1体倒すと +10 点
                    }
                }
            }
        }

        // 画面クリア
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // プレイヤー描画
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        SDL_Rect playerRect = {playerX, playerY, PLAYER_W, PLAYER_H};
        SDL_RenderFillRect(ren, &playerRect);

        // 弾描画
        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                SDL_Rect b = {bullets[i].x, bullets[i].y, BULLET_W, BULLET_H};
                SDL_RenderFillRect(ren, &b);
            }
        }

        // 敵描画
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                SDL_Rect en = {enemies[i].x, enemies[i].y, ENEMY_W, ENEMY_H};
                SDL_RenderFillRect(ren, &en);
            }
        }

        SDL_RenderPresent(ren);

        // スロー化：待ち時間を増やす（30msごと）
        SDL_Delay(30);

        // スコアはコンソールに表示
        printf("\rScore: %d", score);
        fflush(stdout);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
