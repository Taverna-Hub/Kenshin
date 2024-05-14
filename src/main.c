#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define SPRITE_WIDTH 7
#define SPRITE_HEIGHT 4
int timer = 0;

void printSprite(int x, int y, char sprite[SPRITE_HEIGHT][SPRITE_WIDTH + 1])
{
    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        screenGotoxy(x, y + i);
        printf("%s", sprite[i]);
    }
    screenUpdate();
}

void clearSprite(int x, int y, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        screenGotoxy(x, y + i);
        for (int j = 0; j < width; j++)
        {
            printf(" ");
        }
    }
}

char baseSprite1[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', '@', ' ', ' ', '/'},
    {' ', '|', '=', '/', ' '},
    {' ', '|', ' ', ' ', ' '},
    {'/', ' ', '\\', ' ', ' '},
};

char attackSprite1[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', '@', ' ', ' ', ' ', ' ', ' '},
    {' ', '|', '-', '*', '-', '-', '-'},
    {' ', '|', ' ', ' ', ' ', ' ', ' '},
    {'/', ' ', '\\', ' ', ' ', ' ', ' '},
};

char defenseSprite1[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', '@', ' ', '|'},
    {' ', '|', '=', '|'},
    {' ', '|', ' ', ' '},
    {'/', ' ', '\\', ' '},
};

char baseSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {'\\', ' ', ' ', '@', ' '},
    {' ', '\\', '=', '|', ' '},
    {' ', ' ', ' ', '|', ' '},
    {' ', ' ', '/', ' ', '\\'},
};

char attackSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', ' ', ' ', ' ', ' ', '@', ' '},
    {'-', '-', '-', '*', '-', '|', ' '},
    {' ', ' ', ' ', ' ', ' ', '|', ' '},
    {' ', ' ', ' ', ' ', '/', ' ', '\\'},
};

char defenseSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {'|', ' ', '@', ' '},
    {'|', '=', '|', ' '},
    {' ', ' ', '|', ' '},
    {' ', '/', ' ', '\\'},
};

void updatePlayer(int *x, int *y, int dx, char sprite[SPRITE_HEIGHT][SPRITE_WIDTH + 1])
{
    clearSprite(*x, *y, SPRITE_WIDTH, SPRITE_HEIGHT);
    *x += dx;
    printSprite(*x, *y, sprite);
}

void cenario(int x, int y) {
    screenGotoxy(x, y);
    printf("\033[0;32m");
    printf("ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ");
    printf("\033[0m");
}

int main()
{
    int ch = 0;
    int player1X = 34, player1Y = 18;
    int player2X = 50, player2Y = 18;
    int player1Health = 100;
    int player2Health = 100;
    int player1State = 0;
    int player2State = 0;

    screenInit(1);
    keyboardInit();
    timerInit(1);
    screenSetColor(WHITE, BLACK);
    cenario(2, 22);

    while (ch != 10)
    { // enter
        if (keyhit())
        {
            ch = readch();
            if (ch == 27)
            { // escape
                break;
            }

            // player 1 moveset
            else if (ch == 97)
            { // 'a' move left
                updatePlayer(&player1X, &player1Y, -1, baseSprite1);
            }
            else if (ch == 100)
            { // 'd' move right
                updatePlayer(&player1X, &player1Y, 1, baseSprite1);
            }
            else if (ch == 113)
            { // 'q' player 1 attack
                player2Health -= 10;
                updatePlayer(&player1X, &player1Y, 0, attackSprite1);
            }
            else if (ch == 101)
            {
                updatePlayer(&player1X, &player1Y, 1, defenseSprite1);
            }

            // player 2 moveset
            else if (ch == 117)
            { // 'u' player 2 attack
                player1Health -= 10;
                updatePlayer(&player2X, &player2Y, 0, attackSprite2);
            }
            else if (ch == 106)
            { // 'j' move left
                updatePlayer(&player2X, &player2Y, -1, baseSprite2);
            }
            else if (ch == 108)
            { // 'l' move right
                updatePlayer(&player2X, &player2Y, 1, baseSprite2);
            }
            else if (ch == 111)
            { // o player 2 defense[SPRITE_HEIGHT][SPRITE_WIDTH] = {
                updatePlayer(&player2X, &player2Y, 0, defenseSprite2);
            }
        }

        if (timerTimeOver() == 1)
        {

            screenGotoxy(10, 5);
            printf("Player 1 Health: %d", player1Health);
            screenGotoxy(50, 5);
            printf("Player 2 Health: %d", player2Health);

            if (player1Health <= 0)
            {
                printf("Player 1 has died. Game over.\n");
                break;
            }
            else if (player2Health <= 0)
            {
                printf("Player 2 has died. Game over.\n");
                break;
            }
            if (player1State == 1)
                player1State = 0;
            if (player2State == 1)
                player2State = 0;
        }
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();

    return 0;
}
