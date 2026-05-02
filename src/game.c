#include "game.h"
#include "sts_io.h"
#include "stdlib.h"

Player *player;
Enemy *enemy;

GameState start_gameplay() {
    Player p = {{80, 80, 0}, 3, 3};
    Enemy e = {{40, 40, 0}};
    player = &p;
    enemy = &e;
    sts_set_header(draw_hud);
    while (player->base.health > 0 && enemy->base.health > 0) {
        sts_clear_screen();
        sts_println("(1) End Turn");
        sts_println("(2) Attack (Costs 1 Energy; deals 6 Damage to the enemy)");
        sts_println("(3) Defend (Costs 1 Energy; gain 5 Block)");
        int choice = sts_read_int_range("Choose an action: ", 1, 3);
        switch (choice) {
            case 1:
                if (enemy->base.health > 0) {
                    deal_damage(&player->base, 8);
                    sts_clear_screen();
                    sts_println("Enemy attacks for 8 damage!");
                    player->energy = player->initial_energy;
                }
                break;
            case 2:
                if (player->energy > 0) {
                    deal_damage(&enemy->base, 6);
                    player->energy--;
                    sts_clear_screen();
                    sts_println("You attack for 6 damage!");
                } else {
                    sts_println("Not enough energy!");
                }
                break;
            case 3:
                if (player->energy > 0) {
                    player->base.block += 5;
                    player->energy--;
                    sts_clear_screen();
                    sts_println("You gain 5 block!");
                } else {
                    sts_println("Not enough energy!");
                }
                break;
        }
        sts_pause(NULL);
    }
    if (player->base.health <= 0) {
        sts_println("You have been defeated!");
    } else {
        sts_println("You have defeated the enemy!");
    }
    sts_pause(NULL);
    sts_clear_header();
    return STATE_MENU;
}

void draw_hud() {
    sts_title("Slay the Spire");
    sts_print("Battle!\n\nPlayer:");
    sts_set_color(STS_COLOR_RED);
    sts_printf(" ♥︎ %d/%d", player->base.health, player->base.max_health);
    sts_reset_color();
    sts_set_color(STS_COLOR_GREEN);
    sts_printf(" ⛊ %d", player->base.block);
    sts_reset_color();
    sts_set_color(STS_COLOR_BLUE);
    sts_printf(" ✦ %d\n", player->energy);
    sts_reset_color();
    sts_print("Enemy:");
    sts_set_color(STS_COLOR_RED);
    sts_printf(" ♥︎ %d/%d", enemy->base.health, enemy->base.max_health);
    sts_reset_color();
    sts_set_color(STS_COLOR_GREEN);
    sts_printf(" ⛊ %d\n", enemy->base.block);
    sts_reset_color();
    sts_separator();
}

void deal_damage(Entity *target, int damage) {
    if (target->block >= damage) {
        target->block -= damage;
    } else {
        target->health -= (damage - target->block);
        target->block = 0;
    }
    if (target->health < 0) {
        target->health = 0;
    }
}