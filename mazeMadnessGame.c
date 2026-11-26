#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <wchar.h> // Required for wide characters and wprintf/wscanf
#include <locale.h> // Required to set the locale for wide characters

#include "MazeMadness.h"

#define ROWS 150
#define COLS 150

int main (void){
// randomizer 
srand(time(NULL));

// wide character to allow wprintf
setlocale(LC_CTYPE, "");
if (MB_CUR_MAX == 1) setlocale(LC_CTYPE, "C.UTF-8");

char userChoice = 'Y';


while (userChoice == 'Y' || userChoice == 'y') {

    gameIntro();

    wchar_t maze[ROWS][COLS];
    int difficulty;

    printf("\n\nEnter Maze Difficulty (1-3): ");
    difficulty = readChoice();

    generateMaze(difficulty, maze);
    Player p1 = playerStartStats(difficulty);

    clearMaze();
    displayMaze(difficulty, maze);
    statUpdate(p1, difficulty);

    // MAIN GAME LOOP
    while (p1.health > 0 && !p1.gameWon) {

        movePlayer(maze, &p1, difficulty);

        displayMaze(difficulty, maze);
        statUpdate(p1, difficulty);
    }

    // END STATE
    if (p1.gameWon) {
        printf("\n🏆 CONGRATULATIONS! You completed the maze and beat the boss! 🏆\n");
    } else {
        printf("\n💀 GAME OVER! Your health reached zero. 💀\n");
    }

    userChoice = gameContinue();

}



return 0; 

}