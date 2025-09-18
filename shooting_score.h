#ifndef SHOOTING_SCORE_H
#define SHOOTING_SCORE_H

typedef struct {
    int x, y;
    int active;
} Enemy;

void create_enemy(Enemy *enemies, int index, int x, int y);
int enemy_defeated_score(int current_score);

#endif
