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
#define JUMP_STATE 3
#define MAX_JUMP_HEIGHT 6

#define ALTURA_BAMBOO 10
#define QTD_BAMBOO 8

int timer = 0;
int player1JumpHeight = 0;
int player2JumpHeight = 0;
int player1Jumping = 0;
int player2Jumping = 0;
int alturaTela = 25;

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
    screenUpdate();
}

char baseSprite1[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', '@', ' ', ' ', '/', ' '},
    {' ', '|', '=', '/', ' ', ' '},
    {' ', '|', ' ', ' ', ' ', ' '},
    {'/', ' ', '\\', ' ', ' ', ' '}};

char attackSprite1[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', '@', ' ', ' ', ' ', ' ', ' '},
    {' ', '|', '-', '*', '-', '-', '-'},
    {' ', '|', ' ', ' ', ' ', ' ', ' '},
    {'/', ' ', '\\', ' ', ' ', ' ', ' '}};

char defenseSprite1[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', '@', ' ', '|', ' ', ' '},
    {' ', '|', '=', '|', ' ', ' '},
    {' ', '|', ' ', ' ', ' ', ' '},
    {'/', ' ', '\\', ' ', ' ', ' '}};

char baseSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', ' ', '\\', ' ', ' ', '@', ' '},
    {' ', ' ', ' ', '\\', '=', '|', ' '},
    {' ', ' ', ' ', ' ', ' ', '|', ' '},
    {' ', ' ', ' ', ' ', '/', ' ', '\\'}};

char attackSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', ' ', ' ', ' ', ' ', '@', ' '},
    {'-', '-', '-', '*', '-', '|', ' '},
    {' ', ' ', ' ', ' ', ' ', '|', ' '},
    {' ', ' ', ' ', ' ', '/', ' ', '\\'}};

char defenseSprite2[SPRITE_HEIGHT][SPRITE_WIDTH + 1] = {
    {' ', ' ', ' ', '|', ' ', '@', ' '},
    {' ', ' ', ' ', '|', '=', '|', ' '},
    {' ', ' ', ' ', ' ', ' ', '|', ' '},
    {' ', ' ', ' ', ' ', '/', ' ', '\\'}};

void updatePlayer(int *x, int *y, int dx, int dy, char sprite[SPRITE_HEIGHT][SPRITE_WIDTH + 1], int scndX, int scndY, char scndSprite[SPRITE_HEIGHT][SPRITE_WIDTH + 1], int scndJumping)
{
    clearSprite(*x, *y, SPRITE_WIDTH, SPRITE_HEIGHT);
    if ((*x + dx) < MAXX - SPRITE_WIDTH && (*x + dx) > MINX)
    {
        if (dx > 0 && (*x + SPRITE_WIDTH + dx > +scndX) && (*x < scndX + SPRITE_WIDTH) && !(*y + dy+2 < scndY) && !(scndJumping && *y + dy >= scndY + SPRITE_HEIGHT))
        {
            dx = 0;
        }
        else if (dx < 0 && (*x + dx< scndX + SPRITE_WIDTH) && (*x > scndX) && !(*y + dy+2 < scndY) && !(scndJumping && *y + dy >= scndY + SPRITE_HEIGHT))
        {
            dx = 0;
        }

        *x += dx;
    }
    *y += dy;
    printSprite(*x, *y, sprite);
    printSprite(scndX, scndY, scndSprite);
}


void cenario(int x, int y)
{
    screenGotoxy(x, y);
    printf("\033[0;32m");
    for (int i = 0; i < 117; i++){
        printf("-");
    }
    printf("\033[0m");
    screenGotoxy(x,y+1);
     
    for (int i = 0; i < 110; i++) { 
        if (i % 73 == 0) {
            printf("☠");
        } else {
            printf(" ");
        }
    }
}

void drawHouse(int x, int y)
{
    screenGotoxy(x, y);
    char House[7][15] = {
        {'/', '=', '=', '=', '=', '=', '=', '=', '=', '=', '=', '=', '=', '\\'},
        {'I', ' ', '|', '_', '_', '_', '_', '_', '_', '_', '_', '|', ' ', 'I'},
        {' ', ' ', '|', ' ', ' ', ' ', '|', '|', ' ', ' ', ' ', '|', ' ', ' '},
        {' ', ' ', '|', ' ', ' ', ' ', '|', '|', ' ', ' ', ' ', '|', ' ', ' '},
        {' ', ' ', '|', ' ', ' ', 'o', '|', '|', 'o', ' ', ' ', '|', ' ', ' '},
        {' ', ' ', '|', ' ', ' ', ' ', '|', '|', ' ', ' ', ' ', '|', ' ', ' '},
        {' ', ' ', '|', '_', '_', '_', '|', '|', '_', '_', '_', '|', ' ', ' '},
    };

    for (int i = 0; i < 7; i++)
    {
        screenGotoxy(x,y+i);
        for (int j = 0; j < 15; j++)
        {
            printf("%c", House[i][j]);
        }
       
    }
    screenUpdate();
}

void drawBamboo(int x, int y, int altura, int quantidade)
{
    printf("\033[0;32m"); // Cor Verde

    for (int i = 0; i < quantidade; i++)
    {
        int xBamboo = x + i * 3; // Distância entre os bamboos
        for (int j = 0; j < altura; j++)
        {
            screenGotoxy(xBamboo, y - j);
            printf("║");
        }
    }
    printf("\033[0m"); // Reseta a cor
}

void handleJump(int *playerX, int *playerY, int *jumpHeight, int *jumping, int *state, char sprite[SPRITE_HEIGHT][SPRITE_WIDTH + 1])
{
    clearSprite(*playerX, *playerY, SPRITE_WIDTH, SPRITE_HEIGHT);
    if (*jumping)
    {
        if (*jumpHeight < MAX_JUMP_HEIGHT && *jumpHeight >= 0)
        {
            *playerY -= 1;
            (*jumpHeight)++;
        }
        else if (*jumpHeight >= MAX_JUMP_HEIGHT)
        {
            (*jumpHeight) = -MAX_JUMP_HEIGHT;
        }
        else if (*jumpHeight < 0)
        {
            *playerY += 1;
            (*jumpHeight)++;
            if (*jumpHeight == 0)
            {
                *jumping = 0;
                *state = BASE_STATE;
            }
        }
    }
    printSprite(*playerX, *playerY, sprite);
}

void drawHealthBar(int x, int y, int health, int maxHealth)
{
    int barWidth = 45;
    int filledWidth = (health * barWidth) / maxHealth;

    screenGotoxy(x, y);
    printf("\033[0;31m"); 
    for (int i = 0; i < filledWidth; i++)
    {
        printf("█");
    }
    printf("\033[0;37m"); 
    for (int i = filledWidth; i < barWidth; i++)
    {
        printf("█");
    }
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
    int maxHealth=100;

    screenInit(1);
    keyboardInit();
    timerInit(150);
    screenSetColor(WHITE, BLACK);
    cenario(2, alturaTela - 3);

    while (ch != 10)
    { // enter
        if (keyhit())
        {
            ch = readch();
            if (ch == 27) // escape
            {
                break;
            }

            // player 1 moveset
            else if (ch == 97)
            { // 'a' move left
                player1State = BASE_STATE;
                updatePlayer(&player1X, &player1Y, -1, 0, baseSprite1, player2X, player2Y, (player2State==BASE_STATE)? baseSprite2:defenseSprite2, player2Jumping);
            }
            else if (ch == 100)
            { // 'd' move right
                player1State = BASE_STATE;
                updatePlayer(&player1X, &player1Y, 1, 0, baseSprite1, player2X, player2Y, (player2State==BASE_STATE)? baseSprite2:defenseSprite2, player2Jumping);
            }
            else if (ch == 113)
            { // 'q' player 1 attack
                player1State = ATK_STATE;
                updatePlayer(&player1X, &player1Y, 0, 0, attackSprite1, player2X, player2Y, (player2State==BASE_STATE)? baseSprite2:defenseSprite2 , player2Jumping);
                if (player1X + SPRITE_WIDTH == player2X + 1)
                {
                    player2Health -= (player2State == DF_STATE) ? DF_DMG : ATK_DMG;
                }
            }
            else if (ch == 101)
            { // 'e' player 1 defense
                player1State = DF_STATE;
                updatePlayer(&player1X, &player1Y, 0, 0, defenseSprite1, player2X, player2Y, (player2State==BASE_STATE)? baseSprite2:defenseSprite2, player2Jumping);
            }
            else if (ch == 119 && !player1Jumping)
            { // 'w' player 1 jump
                player1Jumping = 1;
                player1State = JUMP_STATE;
            }

            // player 2 moveset
            else if (ch == 117)
            { // 'u' player 2 attack
                player2State = ATK_STATE;
                updatePlayer(&player2X, &player2Y, 0, 0, attackSprite2, player1X, player1Y, (player1State==BASE_STATE)? baseSprite1:defenseSprite1, player1Jumping);
                if (player2X == player1X + SPRITE_WIDTH - 1)
                {
                    player1Health -= (player1State == DF_STATE) ? DF_DMG : ATK_DMG;
                }
            }
            else if (ch == 106)
            { // 'j' move left
                player2State = BASE_STATE;
                updatePlayer(&player2X, &player2Y, -1, 0, baseSprite2, player1X, player1Y, (player1State==BASE_STATE)? baseSprite1:defenseSprite1, player1Jumping);
            }
            else if (ch == 108)
            { // 'l' move right
                player2State = BASE_STATE;
                updatePlayer(&player2X, &player2Y, 1, 0, baseSprite2, player1X, player1Y, (player1State==BASE_STATE)? baseSprite1:defenseSprite1, player1Jumping);
            }
            else if (ch == 111)
            { // 'o' player 2 defense
                player2State = DF_STATE;
                updatePlayer(&player2X, &player2Y, 0, 0, defenseSprite2, player1X, player1Y, (player1State==BASE_STATE)? baseSprite1:defenseSprite1, player1Jumping);
            }
            else if (ch == 105 && !player2Jumping)
            { // 'i' player 2 jump
                player2Jumping = 1;
                player2State = JUMP_STATE;
            }
        }

        if (timerTimeOver() == 1)
        {
            if (timer == 0)
            {
                printSprite(player1X, player1Y, baseSprite1);
                printSprite(player2X, player2Y, baseSprite2);
            }

            drawBamboo(50, alturaTela - 4, ALTURA_BAMBOO, QTD_BAMBOO);
            drawHouse(25, alturaTela - 10);
            drawHealthBar(10, 5, player1Health, maxHealth);
            drawHealthBar(66, 5, player2Health, maxHealth);

            

            handleJump(&player1X, &player1Y, &player1JumpHeight, &player1Jumping, &player1State, baseSprite1);
            handleJump(&player2X, &player2Y, &player2JumpHeight, &player2Jumping, &player2State, baseSprite2);
            if (player1Jumping && player1Y + SPRITE_HEIGHT > player2Y && player1X + SPRITE_WIDTH > player2X && player1X < player2X + SPRITE_WIDTH)
            {
                clearSprite(player1X,player1Y, SPRITE_WIDTH,SPRITE_HEIGHT);
                player1X = player2X + SPRITE_WIDTH;
            }
            if (player2Jumping && player2Y + SPRITE_HEIGHT > player1Y && player2X + SPRITE_WIDTH > player1X && player2X < player1X + SPRITE_WIDTH)
            {
                 clearSprite(player2X,player2Y, SPRITE_WIDTH,SPRITE_HEIGHT);
                player2X = player1X -SPRITE_WIDTH;
            }

            printSprite(player1X, player1Y, (player1State == BASE_STATE) ? baseSprite1 : (player1State == ATK_STATE) ? attackSprite1
                                                                                     : (player1State == DF_STATE)    ? defenseSprite1
                                                                                                                     : baseSprite1);
            printSprite(player2X, player2Y, (player2State == BASE_STATE) ? baseSprite2 : (player2State == ATK_STATE) ? attackSprite2
                                                                                     : (player2State == DF_STATE)    ? defenseSprite2
                                                                                                                     : baseSprite2);

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
            if (player1State == ATK_STATE && !player1Jumping)
                player1State = BASE_STATE;
            if (player2State == ATK_STATE && !player2Jumping)
                player2State = BASE_STATE;
            timer++;
        }
    }

    screenDestroy();
    keyboardDestroy();
    timerDestroy();

    return 0;
}
