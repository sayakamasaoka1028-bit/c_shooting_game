#include "shooting_score.h"

void create_enemy(Enemy *enemies, int index, int x, int y) {
    enemies[index].x = x;
    enemies[index].y = y;
    enemies[index].active = 1;
}

int enemy_defeated_score(int current_score) {
    return current_score + 100;
}

