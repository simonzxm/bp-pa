#include "game.h"
#include "sts_io.h"
#include <stdlib.h>

int main() {
    GameState current = STATE_MENU;

    while (1) {
        switch (current) {
            case STATE_EXIT:
                sts_println("Exiting the game. Goodbye!");
                exit(0);
            case STATE_MENU:
                current = show_main_menu();
                break;
            case STATE_GAMEPLAY:
                current = start_gameplay();
                break;
        }
    }

    return 0;
}

const char ascii_art[] =
    "  _____ _               _   _          \n"
    " / ____| |             | | | |         \n"
    "| (___ | | __ _ _   _  | |_| |__   ___ \n"
    " \\___ \\| |/ _` | | | | | __| '_ \\ / _ \\\n"
    " ____) | | (_| | |_| | | |_| | | |  __/\n"
    "|_____/|_|\\__,_|\\__, |  \\__|_| |_|\\___|\n"
    "           _____ __/ | _               \n"
    "          / ____|___/ (_)              \n"
    "         | (___  _ __  _ _ __ ___      \n"
    "          \\___ \\| '_ \\| | '__/ _ \\     \n"
    "          ____) | |_) | | | |  __/     \n"
    "         |_____/| .__/|_|_|  \\___|     \n"
    "                | |                    \n"
    "                |_|                    ";

GameState show_main_menu() {
    sts_clear_screen();
    sts_title(ascii_art);
    sts_println("(1) Start Game");
    sts_println("(2) Exit");
    int choice = sts_read_int_range("Enter your choice: ", 1, 2);
    return choice == 1 ? STATE_GAMEPLAY : STATE_EXIT;
}
