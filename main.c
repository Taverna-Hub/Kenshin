// #include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int main() {
    static int ch = 0;
    int player1X = 34, player1Y = 12;
    int player2X = 34, player2Y = 12;
    int inc1X = 0, inc1Y = 0;
    int inc2X = 0, inc2Y = 0;
    int player1Health = 100;
    int player2Health = 100;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    while (ch != 10) { // enter
        // Handle user input
        if (keyhit()) { // funcao da keyboard (ñ indica hit)
            ch = readch();
            if (ch == 27) { // escape
                screenDestroy();
                keyboardDestroy();
                timerDestroy();
                return 0;
            } else if (ch == 97) { // a
                inc1X = -1;
            } else if (ch == 119) { // w
                inc1Y = -1;
            } else if (ch == 115) { // s
                inc1Y = +1;
            } else if (ch == 100) { // d
                inc1X = +1;
            } else if (ch == 113) { // q
                // player 1 attack
                player2Health -= 10;
                printf("Player 1 attacks Player 2 for 10 damage.\n");
            } else if (ch == 117) { // u
                // player 2 attack
                player1Health -= 10;
                printf("Player 2 attacks Player 1 for 10 damage.\n");
            }
        }

        // Update game state (move elements, verify collision, etc)
        if (timerTimeOver() == 1) {
            player1X += inc1X;
            player1Y += inc1Y;
            player2X += inc2X;
            player2Y += inc2Y;

            screenGotoxy(player1X, player1Y);
            printf("@");
            screenGotoxy(player2X, player2Y);
            printf("@");
            screenUpdate();

            printf("Player 1 Health: %d\n", player1Health);
            printf("Player 2 Health: %d\n", player2Health);

            if (player1Health <= 0) {
                printf("Player 1 has died. Game over.\n");
                break;
            } else if (player2Health <= 0) {
                printf("Player 2 has died. Game over.\n");
                break;
            }
        }
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();

    return 0;
}