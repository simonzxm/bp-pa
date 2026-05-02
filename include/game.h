#ifndef GAME_H
#define GAME_H

typedef enum {
    STATE_EXIT,
    STATE_MENU,
    STATE_GAMEPLAY
} GameState;

GameState show_main_menu();
GameState start_gameplay();

typedef struct {
    const int max_health;
    int health;
    int block;
} Entity;

typedef struct {
    Entity base;
    const int initial_energy;
    int energy;
} Player;

typedef struct {
    Entity base;
} Enemy;

void draw_hud();

void deal_damage(Entity* target, int damage);

#endif