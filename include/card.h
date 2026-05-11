#ifndef CARD_H
#define CARD_H

#include "game.h"

typedef struct {
    char name[20];
    int cost;
    void (*effect)(Player *player, Enemy *enemy);
    char description[100];
} Card;

typedef enum {
    CARD_STRIKE,
    CARD_DEFEND,
    CARD_BLOODLETTING,
    CARD_TYPE_COUNT
} CardType;

void strike_effect(Player *player, Enemy *enemy);
void defend_effect(Player *player, Enemy *enemy);
void bloodletting_effect(Player *player, Enemy *enemy);

static Card cards[CARD_TYPE_COUNT] = {
    {"Strike", 1, strike_effect,
     "Strike (Costs 1 Energy; deals 6 Damage to the enemy)"},
    {"Defend", 1, defend_effect,
     "Defend (Costs 1 Energy; gains 5 Block to yourself)"},
    {"Bloodletting", -1, bloodletting_effect,
     "Bloodletting (Gains 1 Energy; deals 3 Damage to yourself)"},
};

void play_card(Card *card, Player *player, Enemy *enemy);

typedef enum {
    CARD_IN_HAND,
    CARD_IN_DISCARD,
} CardStatus;

typedef struct {
    Card *cards;
    CardStatus *status;
    int size;
} Deck;

void init_deck(Deck *deck, CardType *card_types, int size);
Card *find_card_in_hand(Deck *deck, int index);
int get_hand_size(Deck *deck);
void discard_card(Deck *deck, int index);
void free_deck(Deck *deck);

#endif
