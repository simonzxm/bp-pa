#include "card.h"
#include "game.h"
#include <assert.h>
#include <string.h>

static void test_card_definitions(void) {
    assert(CARD_TYPE_COUNT == 4);

    assert(strcmp(cards[CARD_STRIKE].name, "Strike") == 0);
    assert(cards[CARD_STRIKE].cost == 1);
    assert(cards[CARD_STRIKE].effect == strike_effect);

    assert(strcmp(cards[CARD_DEFEND].name, "Defend") == 0);
    assert(cards[CARD_DEFEND].cost == 1);
    assert(cards[CARD_DEFEND].effect == defend_effect);

    assert(strcmp(cards[CARD_BLOODLETTING].name, "Bloodletting") == 0);
    assert(cards[CARD_BLOODLETTING].cost == -1);
    assert(cards[CARD_BLOODLETTING].effect == bloodletting_effect);

    assert(strcmp(cards[CARD_RAMPAGE].name, "Rampage") == 0);
    assert(cards[CARD_RAMPAGE].cost == 1);
    assert(cards[CARD_RAMPAGE].effect == rampage_effect);
    assert(cards[CARD_RAMPAGE].data_cnt == 1);
    assert(cards[CARD_RAMPAGE].data[0] == 8);
}

static void test_play_card_applies_cost_and_effect(void) {
    Player p = {{80, 80, 0}, 3, 3};
    Enemy e = {{40, 40, 0}};

    play_card(&cards[CARD_STRIKE], NULL, &p, &e);
    assert(p.energy == 2);
    assert(e.base.health == 34);

    play_card(&cards[CARD_DEFEND], NULL, &p, &e);
    assert(p.energy == 1);
    assert(p.base.block == 5);

    play_card(&cards[CARD_BLOODLETTING], NULL, &p, &e);
    assert(p.energy == 2);
    assert(p.base.health == 77);
    assert(p.base.block == 5);
}

static void test_bloodletting_does_not_make_health_negative(void) {
    Player p = {{80, 2, 0}, 3, 0};
    Enemy e = {{40, 40, 0}};

    play_card(&cards[CARD_BLOODLETTING], NULL, &p, &e);
    assert(p.energy == 1);
    assert(p.base.health == 0);
    assert(e.base.health == 40);
}

static void test_rampage_damage_scales_on_same_card_copy(void) {
    CardType types[] = {CARD_RAMPAGE};
    Deck deck;
    Player p = {{80, 80, 0}, 3, 3};
    Enemy e = {{40, 40, 0}};

    init_deck(&deck, types, 1);
    draw_cards(&deck, 1);
    Card *rampage = find_card_in_hand(&deck, 0);
    assert(rampage != NULL);

    play_card(rampage, &deck, &p, &e);
    assert(p.energy == 2);
    assert(e.base.health == 32);
    assert(rampage->data[0] == 13);

    p.energy = 3;
    play_card(rampage, &deck, &p, &e);
    assert(p.energy == 2);
    assert(e.base.health == 19);
    assert(rampage->data[0] == 18);

    assert(cards[CARD_RAMPAGE].data[0] == 8);
    free_deck(&deck);
}

int main(void) {
    test_card_definitions();
    test_play_card_applies_cost_and_effect();
    test_bloodletting_does_not_make_health_negative();
    test_rampage_damage_scales_on_same_card_copy();
}
