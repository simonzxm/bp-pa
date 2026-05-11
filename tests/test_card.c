#include "card.h"
#include "game.h"
#include "sts_io.h"
#include <assert.h>

extern Player *player;
extern Enemy *enemy;

int main() {
    sts_println("Running card tests...\n");

    Player p = {{80, 80, 0}, 3, 3};
    Enemy e = {{40, 40, 0}};
    player = &p;
    enemy = &e;

    play_card(&cards[CARD_STRIKE], player, enemy);
    assert(player->energy == 2);
    assert(enemy->base.health == 34);

    play_card(&cards[CARD_DEFEND], player, enemy);
    assert(player->energy == 1);
    assert(player->base.block == 5);

    play_card(&cards[CARD_BLOODLETTING], player, enemy);
    assert(player->energy == 2);
    assert(player->base.health == 77);
    assert(player->base.block == 5);

    sts_println("\nAll tests passed!");
}
