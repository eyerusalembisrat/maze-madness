#ifndef MAZEMADNESS_H
#define MAZEMADNESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <wchar.h> // For wide characters
#include <locale.h> // For setlocale

// Define the maximum size for the maze array
#define ROWS 150
#define COLS 150

//Structures

typedef struct {
   int health;
   int shield;
   int row; // Current row position of the player 'P'
   int col; // Current column position of the player 'P'
   int gameWon;
} Player;


// Function Prototypes

// Game Flow & I/O
void gameIntro(void);
int readChoice(void);
char gameContinue(void);
void clearMaze(void);

// Maze Generation & Display
void generateMaze(int difficulty, wchar_t maze[ROWS][COLS]);
void displayMaze(int difficulty, wchar_t maze[ROWS][COLS]);

// Player & Stats
Player playerStartStats(int difficulty);
void statUpdate(Player p1, int difficulty);
void movePlayer (wchar_t maze[ROWS][COLS], Player *p1, int difficulty);

// Encounters & Logic
void EncounterCheck(wchar_t maze[ROWS][COLS], Player *p1, int proposedRow, int proposedCol, int difficulty);
void shieldGamble(int userans, int correctans, Player *p1);
void AnswerCheck(int userans, int correctans, Player *p1);
void BossAnswerCheck(int userans, int correctans, Player *p1); 

// Question Management
void shieldQuestion(char QA[30][256], char *shieldQ[15], char *shieldA[15]);
void easyQuestion(char QA[30][256], char *easyQ[15], char *easyA[15]);
void medQuestion(char QA[30][256], char *medQ[15], char *medA[15]);
void hardQuestion(char QA[30][256], char *hardQ[15], char *hardA[15]);
void bossQuestion(char QA[30][256], char *bossQ[15], char *bossA[15]);


#endif // MAZEMADNESS_H