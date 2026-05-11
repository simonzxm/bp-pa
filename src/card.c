#include "card.h"
#include "game.h"
#include <assert.h>
#include <stdlib.h>

void strike_effect(Player *player, Enemy *enemy) {
    deal_damage(&enemy->base, 6);
}

void defend_effect(Player *player, Enemy *enemy) {
    player->base.block += 5;
}

void bloodletting_effect(Player *player, Enemy *enemy) {
    deal_damage(&player->base, 3);
}

void play_card(Card *card, Player *player, Enemy *enemy) {
    assert(player->energy >= card->cost);
    player->energy -= card->cost;
    card->effect(player, enemy);
}

void init_deck(Deck *deck, CardType *card_types, int size) {
    deck->cards = malloc(sizeof(Card) * size);
    deck->status = malloc(sizeof(CardStatus) * size);
    deck->size = size;
    for (int i = 0; i < size; i++) {
        deck->cards[i] = cards[card_types[i]];
        deck->status[i] = CARD_IN_HAND;
    }
}

Card *find_card_in_hand(Deck *deck, int index) {
    assert(index >= 0 && index < deck->size);
    for (int i = 0, j = 0; i < deck->size; i++) {
        if (deck->status[i] == CARD_IN_HAND) {
            if (j++ == index) {
                return &deck->cards[i];
            }
        }
    }
    return NULL;
}

int get_hand_size(Deck *deck) {
    int count = 0;
    for (int i = 0; i < deck->size; i++) {
        if (deck->status[i] == CARD_IN_HAND) {
            count++;
        }
    }
    return count;
}

void discard_card(Deck *deck, int index) {
    assert(index >= 0 && index < deck->size);
    for (int i = 0, j = 0; i < deck->size; i++) {
        if (deck->status[i] == CARD_IN_HAND) {
            if (j++ == index) {
                deck->status[i] = CARD_IN_DISCARD;
                return;
            }
        }
    }
}

void free_deck(Deck *deck) {
    free(deck->cards);
    free(deck->status);
}
