#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define SPRITE_WIDTH 7
#define SPRITE_HEIGHT 4
#define ATK_DMG 10
#define DF_DMG 5
#define BASE_STATE 0
#define ATK_STATE 1
#define DF_STATE 2
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
    {' ', ' ', '\\', ' ', ' ', '@', ' '},
    {' ', ' ', ' ', '\\', '=', '|', ' '},
    {' ', ' ', ' ', ' ', ' ', '|', ' '},
    {' ', ' ', ' ', ' ', '/', ' ', '\\'},
};

char attackSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', ' ', ' ', ' ', ' ', '@', ' '},
    {'-', '-', '-', '*', '-', '|', ' '},
    {' ', ' ', ' ', ' ', ' ', '|', ' '},
    {' ', ' ', ' ', ' ', '/', ' ', '\\'},
};

char defenseSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', ' ', ' ', '|', ' ', '@', ' '},
    {' ', ' ', ' ' ,'|', '=', '|', ' '},
    {' ', ' ', ' ' , ' ', ' ', '|', ' '},
    {' ', ' ', ' ', ' ', '/', ' ', '\\'},
};

void updatePlayer(int *x, int *y, int dx, char sprite[SPRITE_HEIGHT][SPRITE_WIDTH + 1])
{
    clearSprite(*x, *y, SPRITE_WIDTH, SPRITE_HEIGHT);
    if((*x+dx)<MAXX-7 && ((*x+dx)>MINX)){
        *x += dx;
    }
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
    int player1X = 17, player1Y = 18;
    int player2X = 56, player2Y = 18;
    int player1Health = 100;
    int player2Health = 100;
    int player1State = BASE_STATE;
    int player2State = BASE_STATE;

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
                player1State=ATK_STATE;
                updatePlayer(&player1X, &player1Y, 0, attackSprite1);
                if (player1X-player2X==-4){
                player2Health -= (player2State==DF_STATE)? DF_DMG:ATK_DMG;

                }
            }
            else if (ch == 101) 
            {
                player1State=DF_STATE;
                updatePlayer(&player1X, &player1Y, 0, defenseSprite1);
            }

            // player 2 moveset
            else if (ch == 117)
            { // 'u' player 2 attack
                player2State=ATK_STATE;
                updatePlayer(&player2X, &player2Y, 0, attackSprite2);
                if (player2X-player1X==4){
                player1Health -= (player1State==DF_STATE)? DF_DMG:ATK_DMG;

                }
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
            {   player2State=DF_STATE;
                updatePlayer(&player2X, &player2Y, 0, defenseSprite2);
            }
        }

        if (timerTimeOver() == 1)
        {
        if (timer==0){
            printSprite(player1X, player1Y, baseSprite1);
            printSprite(player2X, player2Y, baseSprite2);

        }
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
            timer=timer+1;
        }
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();

    return 0;
}
