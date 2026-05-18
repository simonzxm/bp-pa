#include "card.h"
#include <assert.h>
#include <string.h>

static int count_status(const Deck *deck, CardStatus status) {
    int count = 0;
    for (int i = 0; i < deck->size; i++) {
        if (deck->status[i] == status) {
            count++;
        }
    }
    return count;
}

static void assert_deck_counts(const Deck *deck) {
    assert(deck->cards_in_draw_pile == count_status(deck, CARD_IN_DRAW_PILE));
    assert(deck->cards_in_hand == count_status(deck, CARD_IN_HAND));
    assert(deck->cards_in_discard == count_status(deck, CARD_IN_DISCARD));
    assert(deck->size == deck->cards_in_draw_pile + deck->cards_in_hand +
                             deck->cards_in_discard);
}

static void test_init_deck_copies_cards_into_draw_pile(void) {
    CardType types[] = {CARD_STRIKE, CARD_DEFEND, CARD_BLOODLETTING};
    Deck deck;

    init_deck(&deck, types, 3);

    assert(deck.size == 3);
    assert(deck.cards_in_draw_pile == 3);
    assert(deck.cards_in_hand == 0);
    assert(deck.cards_in_discard == 0);
    assert(strcmp(deck.cards[0].name, "Strike") == 0);
    assert(strcmp(deck.cards[1].name, "Defend") == 0);
    assert(strcmp(deck.cards[2].name, "Bloodletting") == 0);
    assert_deck_counts(&deck);

    free_deck(&deck);
}

static void test_draw_and_find_cards_in_hand(void) {
    CardType types[] = {CARD_STRIKE, CARD_DEFEND, CARD_BLOODLETTING};
    Deck deck;

    init_deck(&deck, types, 3);
    draw_cards(&deck, 2);

    assert(deck.cards_in_draw_pile == 1);
    assert(deck.cards_in_hand == 2);
    assert(deck.cards_in_discard == 0);
    assert(strcmp(find_card_in_hand(&deck, 0)->name, "Strike") == 0);
    assert(strcmp(find_card_in_hand(&deck, 1)->name, "Defend") == 0);
    assert_deck_counts(&deck);

    free_deck(&deck);
}

static void test_discard_one_card_from_hand_by_hand_index(void) {
    CardType types[] = {CARD_STRIKE, CARD_DEFEND, CARD_BLOODLETTING};
    Deck deck;

    init_deck(&deck, types, 3);
    draw_cards(&deck, 3);
    discard_card(&deck, 1);

    assert(deck.cards_in_draw_pile == 0);
    assert(deck.cards_in_hand == 2);
    assert(deck.cards_in_discard == 1);
    assert(deck.status[0] == CARD_IN_HAND);
    assert(deck.status[1] == CARD_IN_DISCARD);
    assert(deck.status[2] == CARD_IN_HAND);
    assert(strcmp(find_card_in_hand(&deck, 0)->name, "Strike") == 0);
    assert(strcmp(find_card_in_hand(&deck, 1)->name, "Bloodletting") == 0);
    assert_deck_counts(&deck);

    free_deck(&deck);
}

static void test_discard_all_and_shuffle_discard_into_draw_pile(void) {
    CardType types[] = {CARD_STRIKE, CARD_DEFEND, CARD_BLOODLETTING};
    Deck deck;

    init_deck(&deck, types, 3);
    draw_cards(&deck, 3);
    discard_all(&deck);
    shuffle_deck(&deck);

    assert(deck.cards_in_draw_pile == 3);
    assert(deck.cards_in_hand == 0);
    assert(deck.cards_in_discard == 0);
    assert_deck_counts(&deck);

    free_deck(&deck);
}

static void test_draw_reshuffles_discard_when_draw_pile_is_empty(void) {
    CardType types[] = {CARD_STRIKE, CARD_DEFEND, CARD_BLOODLETTING};
    Deck deck;

    init_deck(&deck, types, 3);
    draw_cards(&deck, 3);
    discard_all(&deck);
    draw_cards(&deck, 2);

    assert(deck.cards_in_draw_pile == 1);
    assert(deck.cards_in_hand == 2);
    assert(deck.cards_in_discard == 0);
    assert_deck_counts(&deck);

    free_deck(&deck);
}

static void test_init_deck_deep_copies_mutable_card_data(void) {
    CardType types[] = {CARD_RAMPAGE};
    Deck deck;

    init_deck(&deck, types, 1);

    assert(deck.cards[0].data_cnt == 1);
    assert(deck.cards[0].data != cards[CARD_RAMPAGE].data);
    assert(deck.cards[0].data[0] == 8);

    deck.cards[0].data[0] = 42;
    assert(cards[CARD_RAMPAGE].data[0] == 8);
    assert_deck_counts(&deck);

    free_deck(&deck);
}

int main(void) {
    test_init_deck_copies_cards_into_draw_pile();
    test_draw_and_find_cards_in_hand();
    test_discard_one_card_from_hand_by_hand_index();
    test_discard_all_and_shuffle_discard_into_draw_pile();
    test_draw_reshuffles_discard_when_draw_pile_is_empty();
    test_init_deck_deep_copies_mutable_card_data();
}
