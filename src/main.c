#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define SPRITE_HEIGHT_ALL 4

#define SPRITE_BASE_BLUE 6
#define SPRITE_ATTACK_BLUE 9
#define SPRITE_DEFENSE_BLUE 5

#define SPRITE_BASE_RED 5
#define SPRITE_ATTACK_RED 8
#define SPRITE_DEFENSE_RED 4

// Posturas Base
char basePostureBlue[SPRITE_HEIGHT_ALL][SPRITE_BASE_BLUE] = {
    "  @  /",
    "  |=/",
    " /\\",
    "/ |",
};
char basePostureRed[SPRITE_HEIGHT_ALL][SPRITE_BASE_RED] = {
    "\\  @",
    " \\=|",
    "   /\\",
    "  |  \\"};

// Posturas Ataque
char AttackPostureBlue[SPRITE_HEIGHT_ALL][SPRITE_ATTACK_BLUE] = {
    "  @",
    "  |=-*---",
    " /\\",
    "/  |"};
char AttackPostureRed[SPRITE_HEIGHT_ALL][SPRITE_ATTACK_RED] = {
    "      @",
    "---*-=|",
    "     /\\",
    "    |  \\"};

// Posturas Defesa
char DefensePostureBlue[SPRITE_HEIGHT_ALL][SPRITE_DEFENSE_BLUE] = {
    "  @ |",
    "  |=|",
    " /\\",
    "/ |"};
char DefensePostureRed[SPRITE_HEIGHT_ALL][SPRITE_DEFENSE_RED] = {
    "| @",
    "|=|",
    " /\\",
    "|  \\"};

struct espada
{
    int posic_lamina;
    int posic_bainha;
};
typedef struct jogador
{
    int posicao;
    int vidaTotal;
    struct espada katana;
} jogador;

void VidaBlue(jogador *blue)
{
    int hpBlue = 5;
    blue->vidaTotal = hpBlue;
    return blue->vidaTotal;
}
void VidaRed(jogador *red)
{
    int hpRed = 5;
    red->vidaTotal = hpRed;
    return red->vidaTotal;
}

void Combate(char atkBlue, char atkRed, int posicaoBlue, int posicaoRed, char defBlue, char defRed, jogador *blue, jogador *red)

{
    if (posicaoBlue = posicaoRed + 1) // se azul tiver no alcançe com distancia de 1 do referencial da cabeca "@"
    {
        if (atkBlue) // se azul atacar
        {
            if (defRed) // se vermelho defender
            {
                VidaRed(red);
                red->vidaTotal -= 0; // perde 0hp
            }
            VidaRed(red);
            red->vidaTotal--; // perde 1hp
        }
    }
    printf("Errou Ataque");
}

void printObject(int x, int y, const char *emoji)
{
    screenGotoxy(x, y);
    printf("%s", emoji);
    screenUpdate();
}

void updatePlayer(int *x, int *y, int dx, char *emoji)
{
    screenGotoxy(*x, *y);
    printf(" ");
    *x += dx;
    printObject(*x, *y, emoji);
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
    char *playerEmoji = "🤺";

    screenInit(1);
    keyboardInit();
    timerInit(1);

    while (ch != 10)
    { // enter
        if (keyhit())
        {
            ch = readch();
            if (ch == 27)
            { // escape
                break;
            }
            else if (ch == 97)
            { // 'a' move left
                updatePlayer(&player1X, &player1Y, -1, playerEmoji);
            }
            else if (ch == 100)
            { // 'd' move right
                updatePlayer(&player1X, &player1Y, 1, playerEmoji);
            }
            else if (ch == 113)
            { // 'q' player 1 attack
                player2Health -= 10;
            }
            else if (ch == 117)
            { // 'u' player 2 attack
                player1Health -= 10;
            }
            else if (ch == 106)
            { // 'j' move left
                updatePlayer(&player2X, &player2Y, -1, playerEmoji);
            }
            else if (ch == 108)
            { // 'l' move right
                updatePlayer(&player2X, &player2Y, 1, playerEmoji);
            }
        }

        if (timerTimeOver() == 1)
        {
            printObject(player1X, player1Y, playerEmoji);
            printObject(player2X, player2Y, playerEmoji);
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