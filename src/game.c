#include "game.h"
#include "card.h"
#include "sts_io.h"
#include <assert.h>
#include <stdlib.h>

Player *player;
Enemy *enemy;
Deck deck;

CardType starting_cards[] = {
    CARD_STRIKE, CARD_STRIKE, CARD_STRIKE,       CARD_STRIKE,
    CARD_STRIKE, CARD_DEFEND, CARD_DEFEND,       CARD_DEFEND,
    CARD_DEFEND, CARD_DEFEND, CARD_BLOODLETTING,
};

GameState start_gameplay() {
    Player p = {{80, 80, 0}, 3, 3};
    Enemy e = {{40, 40, 0}};
    player = &p;
    enemy = &e;
    init_deck(&deck, starting_cards, sizeof(starting_cards) / sizeof(CardType));
    draw_cards(&deck, 5);
    sts_set_header(draw_hud);
    while (player->base.health > 0 && enemy->base.health > 0) {
        sts_clear_screen();
        sts_println("(0) End Turn");
        for (int i = 0, j = 0; i < deck.size; i++) {
            if (deck.status[i] == CARD_IN_HAND) {
                sts_printf("(%d) %s\n", ++j, deck.cards[i].description);
            }
        }
        int choice =
            sts_read_int_range("\nChoose an action: ", 0, deck.cards_in_hand);
        if (choice == 0) {
            if (enemy->base.health > 0) {
                deal_damage(&player->base, 8);
                sts_clear_screen();
                sts_println("Enemy attacks for 8 damage!");
                player->base.block = 0;
                player->energy = player->initial_energy;
                discard_all(&deck);
                draw_cards(&deck, 5);
            }
        } else {
            Card *card = find_card_in_hand(&deck, choice - 1);
            assert(card);
            if (card->cost <= player->energy) {
                play_card(card, player, enemy);
                discard_card(&deck, choice - 1);
            } else {
                sts_println("Not enough energy!");
            }
        }
        sts_pause(NULL);
    }
    sts_clear_screen();
    if (player->base.health <= 0) {
        sts_println("\nYou have been defeated!");
    } else {
        sts_println("\nYou have defeated the enemy!");
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
