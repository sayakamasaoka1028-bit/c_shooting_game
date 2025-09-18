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

Bullet bullets[MAX_BULLETS];
Enemy enemies[MAX_ENEMIES];
int score = 0;

// 敵生成
void create_enemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].x = rand() % (WINDOW_W - ENEMY_W);
            enemies[i].y = 0;
            enemies[i].active = 1;
            break;
        }
    }
}

// スコア描画
void draw_score(SDL_Renderer *ren, TTF_Font *font) {
    char scoreText[32];
    sprintf(scoreText, "SCORE: %d", score);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Solid(font, scoreText, white);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);

    SDL_Rect dst = {10, 10, surf->w, surf->h};
    SDL_RenderCopy(ren, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

int main() {
    srand(time(NULL));

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

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 24);
    if (!font) {
        printf("Font load error: %s\n", TTF_GetError());
        return 1;
    }

    int running = 1;
    SDL_Event e;
    int playerX = WINDOW_W / 2 - PLAYER_W / 2;
    int playerY = WINDOW_H - PLAYER_H - 10;

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

        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] && playerX > 0) playerX -= 5;
        if (state[SDL_SCANCODE_RIGHT] && playerX < WINDOW_W - PLAYER_W) playerX += 5;

        // 弾の移動
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                bullets[i].y -= 5;
                if (bullets[i].y < 0) bullets[i].active = 0;
            }
        }

        // 敵の生成
        if (rand() % 50 == 0) create_enemy();

        // 敵の移動
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                enemies[i].y += 2;
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
                        score += 10;
                    }
                }
            }
        }

        // 描画
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        SDL_Rect player = {playerX, playerY, PLAYER_W, PLAYER_H};
        SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
        SDL_RenderFillRect(ren, &player);

        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                SDL_Rect b = {bullets[i].x, bullets[i].y, BULLET_W, BULLET_H};
                SDL_RenderFillRect(ren, &b);
            }
        }

        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].active) {
                SDL_Rect en = {enemies[i].x, enemies[i].y, ENEMY_W, ENEMY_H};
                SDL_RenderFillRect(ren, &en);
            }
        }

        draw_score(ren, font);

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
