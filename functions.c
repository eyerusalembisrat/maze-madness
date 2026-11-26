#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif


#include <stdio.h>
#include <wchar.h> // Required for wide characters and wprintf/wscanf
#include <locale.h> // Needed to properly display unicode characters (like █)

#include "MazeMadness.h"

#define ROWS 150
#define COLS 150

// Read a complete line so invalid input cannot leak into the next prompt.
static void readLine(char *line, size_t size) {
    fflush(stdout);
    if (!fgets(line, size, stdin)) {
        if (ferror(stdin)) { perror("Input"); exit(EXIT_FAILURE); }
        printf("\nThanks for playing!\n");
        exit(EXIT_SUCCESS);
    }
    if (!strchr(line, '\n') && !feof(stdin)) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        line[0] = '\0';
    }
}

int readChoice(void) {
    char line[128], *end;
    for (;;) {
        readLine(line, sizeof line);
        errno = 0;
        long value = strtol(line, &end, 10);
        if (end != line && errno == 0) {
            while (isspace((unsigned char)*end)) end++;
            if (*end == '\0' && value >= 1 && value <= 3) return (int)value;
        }
        printf("Please enter 1, 2, or 3: ");
    }
}

static char readCommand(void) {
    char line[128], command, extra;
    readLine(line, sizeof line);
    return sscanf(line, " %c %c", &command, &extra) == 1 ? command : '\0';
}

void gameIntro(void){
   // Game Intro: Rules & Welcome
printf("  ▄▄▄▄███▄▄▄▄      ▄████████  ▄███████▄     ▄████████\n");                                  
printf("▄██▀▀▀███▀▀▀██▄   ███    ███ ██▀     ▄██   ███    ███\n");                                      
printf("███   ███   ███   ███    ███       ▄███▀   ███    █▀\n");                                       
printf("███   ███   ███   ███    ███  ▀█▀▄███▀▄▄  ▄███▄▄▄\n");                                          
printf("███   ███   ███ ▀███████████   ▄███▀   ▀ ▀▀███▀▀▀\n");                                          
printf("███   ███   ███   ███    ███ ▄███▀         ███    █▄\n");                                       
printf("███   ███   ███   ███    ███ ███▄     ▄█   ███    ███\n ");                                     
printf("▀█   ███   █▀    ███    █▀   ▀████████▀   ██████████\n\n");                                      
                                                                                      
printf("  ▄▄▄▄███▄▄▄▄      ▄████████ ████████▄  ███▄▄▄▄      ▄████████    ▄████████    ▄████████\n");   
printf("▄██▀▀▀███▀▀▀██▄   ███    ███ ███   ▀███ ███▀▀▀██▄   ███    ███   ███    ███   ███    ███\n");   
printf("███   ███   ███   ███    ███ ███    ███ ███   ███   ███    █▀    ███    █▀    ███    █▀\n");   
printf("███   ███   ███   ███    ███ ███    ███ ███   ███  ▄███▄▄▄       ███          ███     \n");   
printf("███   ███   ███ ▀███████████ ███    ███ ███   ███ ▀▀███▀▀▀     ▀███████████ ▀███████████\n");   
printf("███   ███   ███   ███    ███ ███    ███ ███   ███   ███    █▄           ███          ███\n");   
printf("███   ███   ███   ███    ███ ███   ▄███ ███   ███   ███    ███    ▄█    ███    ▄█    ███\n");   
printf(" ▀█   ███   █▀    ███    █▀  ████████▀   ▀█   █▀    ██████████  ▄████████▀   ▄████████▀ \n\n\n");         


printf("Objective: Navigate through the maze filled with obstacles and beat the Boss! \n");
printf("Rules:\n");
printf("1. Choose Maze Difficulty (easy = 1, medium = 2, hard = 3)\n");
printf("2. You are Player (P), use WASD keys to navigate the maze (W = up, S = down, A = left, D = right)\n");
printf("3. Make it to the Boss while preserving your health!\n\n");


printf("Encounters: on your journey you will come across obstacles \n");
printf("a. Troll(?) - Mandatory : When you land on a troll you will be asked a question, be sure to answer correctly or suffer the consequences!\n");
printf("b. Shield(#) - Optional : Opportunities to gain shields that protect against troll attacks.\n");
printf("c. Boss (B) - Mandatory : You must defeat the boss to exit the maze & win the game!\n");


}

// this function prompts the user to see if theyd like to play agian after losing or winning 
char gameContinue(void){
    char userChoice;
    printf("Would you like to play again? (Y/N): ");
    while ((userChoice = readCommand()) != 'Y' && userChoice != 'y' &&
           userChoice != 'N' && userChoice != 'n') {
        printf("Please enter Y or N: ");
    }

     if (userChoice== 'Y' || userChoice == 'y') {
        return 'Y';  // continue
    } else {
        printf("Thanks for playing!");
        return 'N';  // exit
    }
}

// this function copies a preset maze layout into the active maze array depending on difficulty
void generateMaze(int difficulty, wchar_t maze[ROWS][COLS]) {
   if (difficulty == 3) { // Hard maze
       const wchar_t hardMaze[ROWS][COLS] = {
       L"█ P ████████████████████████████████████",
       L"█       █        █     █  █     █      █",
       L"█    ████  ████  █  █  █  █  █  █  █   █",
       L"█       █  █   ?    █        █     █   █",
       L"████    ███████  █  █████████████  █████",
       L"█       █    #   █     █     █         █",
       L"████    ████  ██████████  ████  ████   █",
       L"█         ?   █  █  █     #     █  █   █",
       L"███████    ████  █  ██████████  █  █   █",
       L"█             #           █        █   █",
       L"█  ████  █    ████  ███████  ███████████",
       L"█     █  █       █    ?   █        █   █",
       L"█  ████  ████████████  █  █  ████  █   █",
       L"█  █     █    #     █  █  █  █         █",
       L"█  ███████  ████    █  ████  █  ████   █",
       L"█     ?        █    █     █  █  █      █",
       L"█  █████████████    █  ████  ████  █   █",
       L"█     █  █  █       █     █  █  █  █   █",
       L"█  ████  █  ███████████████  █  ████████",
       L"█     █  █  █     █       █     █      █",
       L"█  ████  █  █  █████████  █  ████  █   █",
       L"█  █   #       █       ?           █   █",
       L"█  ████  ██████████    █  █  ███████   █",
       L"█              █       █  █        █   █",
       L"████████████████████████████████████ B █"


       };


       // copies hardMaze into active maze
       for (int r = 0; r < ROWS; r++) {
           for (int c = 0; c < COLS; c++) {
               maze[r][c] = hardMaze[r][c];
           }
       }
   }
   else if (difficulty == 2) { // Medium maze
       const wchar_t medMaze[ROWS][COLS] = {
       L"█  P  ██████████████████████",
       L"█         █     █     █    █",
       L"████   ████  █  ████  █    █",
       L"█      █  █  █     ?       █",
       L"█  █   █  ████   █  █  █   █",
       L"█  █   █    #    █  █  █   █",
       L"█  ███████████   ████  █   █",
       L"█  █      ?         █  █   █",
       L"█  ███████████   ███████████",
       L"█        #   █             █",
       L"████████  █  █  █   █  █   █",
       L"█     ?   █     █   █  █   █",
       L"█   ████  ██████████████████",
       L"█   █  █  █     #   █      █",
       L"█   █  █  ████████  █  █   █",
       L"█      █     ?         █   █",
       L"████████████████████████ B █"
         
       };
       // copies medMaze into active maze
       for (int r = 0; r < ROWS; r++) {
           for (int c = 0; c < COLS; c++) {
               maze[r][c] = medMaze[r][c];
           }
       }
   }
   else if (difficulty == 1) { // Easy maze
       const wchar_t easyMaze[ROWS][COLS] = {
       L"█ P ████████████████",
       L"█     # █    #     █",
       L"█ ?  ███████  █    █",
       L"█       █     █    █",
       L"█████ ? █  █  ██████",
       L"█          █  █    █",
       L"█ # █   ███████    █",
       L"█   █   █          █",
       L"█████   ████ # █   █",
       L"█     ?        █   █",
       L"████████████████ B █"


       };

        // copies easyMaze into active maze
       for (int r = 0; r < ROWS; r++) {
           for (int c = 0; c < COLS; c++) {
               maze[r][c] = easyMaze[r][c];
           }
       }
   }
   else {
       printf("Error: Enter a valid maze difficulty (1-3)\n");
   }
}

// this function Prints the maze to screen based of difficulty 
void displayMaze(int difficulty, wchar_t maze[ROWS][COLS]) {
   printf("\n");


   if (difficulty == 3){
    // hard maze has 25 rows 
       for (int r = 0; r < 25; r++) {
           for (int c = 0; c < COLS; c++) {
           if (maze[r][c] == L'\0') break;
           printf("%lc", (wint_t)maze[r][c]);
       }
       printf("\n");
   }
}


    if (difficulty == 2){
    // med maze used 17 rows 
       for (int r = 0; r < 17; r++) {
           for (int c = 0; c < COLS; c++) {
           if (maze[r][c] == L'\0') break;
           printf("%lc", (wint_t)maze[r][c]);
       }
       printf("\n");
   }
   }


    if (difficulty == 1){
    // easy maze uses 11 rows 
       for (int r = 0; r < 11; r++) {
           for (int c = 0; c < COLS; c++) {
           if (maze[r][c] == L'\0') break;
           printf("%lc", (wint_t)maze[r][c]);
       }
       printf("\n");
   }
}


}

// this function initializes the starting player stats based on difficulty
Player playerStartStats(int difficulty){
   Player p1;
   p1.gameWon = 0;
   //defaults easy stats
   p1.health = 3;// all players start off with full health (3)
   p1.shield = 3;
   p1.row = 0;
   p1.col = 2;

   if (difficulty == 3){
    // hard mode adjustments 
       p1.shield = 1;
       p1.row = 0; // starting position of the player based off the hard maze
       p1.col = 2;

       printf("\n");
       printf("PLAYER STATS:\n");
       printf("maze level - hard\n");
       printf("health - %d \n", p1.health);
       printf("shield - %d \n", p1.shield);
       return p1;
    }

   else if (difficulty == 2){
    // med mode adjustments 
       p1.shield = 2;
       p1.row = 0; // starting position of the player based off the med maze
       p1.col = 3;

       printf("\n");
       printf("PLAYER STATS:\n");
       printf("maze level - medium\n");
       printf("health - %d \n", p1.health);
       printf("shield - %d \n", p1.shield);

       return p1;
   }

   else if (difficulty == 1){
    // easy mode adjustments 
       p1.shield = 1;
       p1.row = 0; // starting position of the player based off the easy maze
       p1.col = 2;

       printf("\n");
       printf("PLAYER STATS:\n");
       printf("maze level - easy\n");
       printf("health - %d \n", p1.health);
       printf("shield - %d \n", p1.shield);

       return p1;
    }
   return p1;
}

// this function checks if the players proposed position holds an obstacle (?, #, B or a wall)
void EncounterCheck(wchar_t maze[ROWS][COLS], Player *p1, int proposedRow, int proposedCol, int difficulty){
   wchar_t target_char = maze[proposedRow][proposedCol];
   
   // check for trolls then calls a question if true (based off difficulty)
   if (target_char == L'?'){
    
       if (difficulty == 3){
           char hQA[30][256];
           char *hardQ[15];
           char *hardA[15];
           hardQuestion(hQA,hardQ,hardA);
           int qpick = rand() % 15;
           char *qchosen = hardQ[qpick];
           char *apchosen = hardA[qpick];
           printf("Q: %s\nA: ",qchosen);
           int userans = readChoice();
           int correctAns = atoi(apchosen); // converts "1" -> 1
           AnswerCheck(userans, correctAns, p1);

       }


       else if (difficulty == 2){
           char mQA[30][256];
           char *medQ[15];
           char *medA[15];
           medQuestion(mQA,medQ,medA);
           int qpick = rand() % 15;
           char *qchosen = medQ[qpick];
           char *apchosen = medA[qpick];
           printf("Q: %s",qchosen);
           int userans = readChoice();
           int correctAns = atoi(apchosen); // converts "1" -> 1
           AnswerCheck(userans, correctAns, p1);

       }


       else if (difficulty == 1){
           char eQA[30][256];
           char *easyQ[15];
           char *easyA[15];
           easyQuestion(eQA,easyQ,easyA);
           int qpick = rand() % 15;
           char *qchosen = easyQ[qpick];
           char *apchosen = easyA[qpick];
           printf("Q: %s",qchosen);
           int userans = readChoice();
           int correctAns = atoi(apchosen); // converts "1" -> 1
           AnswerCheck(userans, correctAns, p1);

       }


   }
   // checks if user encouters a shield and calls the shieldGamble function 
   else if (target_char == L'#'){
   char sQA[30][256];
   char *shieldQ[15];
   char *shieldA[15];
   shieldQuestion(sQA,shieldQ,shieldA);
   int qpick = rand() % 15;
   char *qchosen = shieldQ[qpick];
   char *apchosen = shieldA[qpick];
   printf("Q: %s",qchosen);
   int userans = readChoice();
   int correctAns = atoi(apchosen); // converts "1" -> 1
   shieldGamble(userans,correctAns,p1);
   }
   // checks if user encounters the boss then loops the boss question 3 times 
   else if (target_char == L'B'){
       char bQA[30][256];
       char *bossQ[15];
       char *bossA[15];
       bossQuestion(bQA,bossQ,bossA);

       for (int i = 0; i < 3 && p1->health > 0; i++ ){
       int qpick = rand() % 15;
       char *qchosen = bossQ[qpick];
       char *apchosen = bossA[qpick];
       printf("Q: %s",qchosen);
       int userans = readChoice();
       int correctAns = atoi(apchosen); // converts "1" -> 1
        BossAnswerCheck(userans, correctAns, p1);
       if (p1->health <= 0) break; // lose if health zero
    }
    if (p1->health > 0) {
        printf("\n🎉 YOU DEFEATED THE BOSS! 🎉\n");
        p1->gameWon = 1;  // signal that player won
    }

   }


}

// this function will move the player based of WASD input 
void movePlayer (wchar_t maze[ROWS][COLS], Player *p1, int difficulty){

   // stores the amount if rows in each maze based off difficulty
   // this will be used to make sure the player doesnt move out of bounds

  int maxRows = 0;
  if (difficulty == 3){
   maxRows = 25;
  }

  else if (difficulty == 2){
   maxRows = 17;
  }

  else if (difficulty == 1){
   maxRows = 11;
  }
 
   // proposedRow and proposedCol store the current position of the player by referncing the struct 
  int proposedRow = p1->row;
  int proposedCol = p1->col;


 // prompts user to enter in their move (WASD) and stores it in playerMovementInput
  char playerMovementInput;
  printf("\nEnter next move (WASD): ");
  playerMovementInput = readCommand();

// updates proposed position based on key pressed
 if (playerMovementInput == 'W' || playerMovementInput == 'w'){
       proposedRow --;
  }
  else if (playerMovementInput == 'S' || playerMovementInput == 's'){
       proposedRow ++;
  }
  else if (playerMovementInput == 'A' || playerMovementInput == 'a'){
       proposedCol --;
  }
  else if (playerMovementInput == 'D' || playerMovementInput == 'd'){
       proposedCol ++;
  }
  else{
   printf("Invalid move. Use W, A, S, or D.\n");
   return; // loops back to ask the user for their naxt move
  }


  // Reject out-of-bounds moves before accessing the maze.
  if (proposedRow < 0 || proposedRow >= maxRows || proposedCol < 0 ||
      proposedCol >= COLS || (size_t)proposedCol >= wcslen(maze[proposedRow])) {
      printf("Invalid move. Please stay within the maze.\n");
      return;
  }

  wchar_t target_char = maze[proposedRow][proposedCol];
  if (target_char == L'█') {
      printf("\nInvalid movement - WALL. Choose a different direction.\n");
      return;
  }
  EncounterCheck(maze, p1, proposedRow, proposedCol, difficulty);

   // Finalize move ONLY if not a wall
if (maze[proposedRow][proposedCol] != L'█') {

    // Clear old position
    maze[p1->row][p1->col] = L' ';

    // Update player position
    p1->row = proposedRow;
    p1->col = proposedCol;

    // Place player in maze
    maze[p1->row][p1->col] = L'P';
}

}

// this function clears the display so that the new maze wont be displayed underneeth an old maze 
void clearMaze(void){
   // Clear only an interactive terminal; preserve redirected output.
#ifdef _WIN32
   if (_isatty(_fileno(stdout))) system("cls");
#else
   if (isatty(STDOUT_FILENO)) printf("\033[2J\033[H");
#endif
}

//updates stat display to be ran after encounters. Based on difficulty
void statUpdate(Player p1,int difficulty) {//updates stat display to be ran after encounters
   if (difficulty == 3){
       printf("\n");
       printf("PLAYER STATS:\n");
       printf("maze level - hard\n");
       printf("health - %d \n", p1.health);
       printf("shield - %d \n", p1.shield);
   }
   else if (difficulty == 2) {
       printf("\n");
       printf("PLAYER STATS:\n");
       printf("maze level - medium\n");
       printf("health - %d \n", p1.health);
       printf("shield - %d \n", p1.shield);
   }
   else if (difficulty == 1) {
       printf("\n");
       printf("PLAYER STATS:\n");
       printf("maze level - easy\n");
       printf("health - %d \n", p1.health);
       printf("shield - %d \n", p1.shield);
   }
}

//random shield gain/loss func
void shieldGamble(int userans, int correctans, Player *p1) {//random shield gain/loss func
   if (userans == correctans) {// if correct, gain random number of shields between 0-4
       printf("That's Right! But we're not done yet...LET'S GO GAMBLING!!!");//answer response
       int randgain = rand() % 5;//roll rand gain number
       p1->shield += randgain;//add rolled number to current player shield value
       printf("Shields Gained: %d",randgain);//result print
       if (randgain==0) {//special dialogue for rolling +0
           printf("LOL. No shields for you! SEEYA NERD XD");
       }
       else if (randgain==4) {//special dialogue for rolling +4
           printf("JACKPOT!! Guess I was feelin generous ¯\\_(ツ)_/¯ ");
       }
   }
   else {//if incorrect, lose random number of shields between 0-4
       printf("Incorrect! Lady Luck shall decide your fate...LET'S GO GAMBLING!!!");//answer response
       int randloss = rand() % 5;//roll rand loss number
       p1->shield -= randloss;//subtract rolled number from current player shield value
       if (p1->shield<0) {
           p1->shield = 0;
       }
       printf("Shields Loss: %d",randloss);//result print
       if (randloss==0) {//special dialgue for rolling -0
           printf("Now might be the right time to buy a friggin lotto ticket...now SCRAM!! (•`︹´•)");
       }
       else if (randloss==4) {//special dialogue for rolling -4
           printf("Nothing personal kid...*EATS YOUR ARMOR* (っˆڡˆς)");
       }
   }
}

//question funcs
static void readQuestionFile(const char *filename, char QA[30][256]) {
    FILE *fid = fopen(filename, "r");
    if (!fid) { perror(filename); exit(EXIT_FAILURE); }
    for (int i = 0; i < 30; i++) {
        QA[i][0] = '\0';
        if (!fgets(QA[i], 256, fid) || !strchr(QA[i], '\n')) {
            // A final line without a newline is valid if it fits.
            if (i == 29 && feof(fid) && !ferror(fid) && QA[i][0]) {
                /* validated below */
            } else {
                fprintf(stderr, "Invalid or incomplete question bank: %s\n", filename);
                fclose(fid); exit(EXIT_FAILURE);
            }
        }
        if (i % 2 == 1) {
            char *end;
            long answer = strtol(QA[i], &end, 10);
            while (isspace((unsigned char)*end)) end++;
            if (answer < 1 || answer > 3 || *end) {
                fprintf(stderr, "Invalid answer in %s\n", filename);
                fclose(fid); exit(EXIT_FAILURE);
            }
        }
    }
    fclose(fid);
}


void shieldQuestion(char QA[30][256], char *shieldQ[15],char *shieldA[15]) {//fills q/a arrays

   readQuestionFile("ShieldQA.txt", QA);
   int q =0;
   int a =0;
   for (int i = 0; i < 30; i++) {
       if (i % 2 == 0){
           shieldQ[q++] = QA[i];
       }
       else {
           shieldA[a++] = QA[i];
       }
   }


}

void easyQuestion(char QA[30][256], char *easyQ[15], char *easyA[15]) {
   readQuestionFile("EasyQA.txt", QA);
   int q = 0;
   int a = 0;
   for (int i = 0; i < 30; i++) {
       if (i % 2 == 0){
           easyQ[q++] = QA[i];
       }
       else {
           easyA[a++] = QA[i];
       }
   }


}

void medQuestion(char QA[30][256],char *medQ[15], char *medA[15]) {
   readQuestionFile("MedQA.txt", QA);
   int q = 0;
   int a = 0;
   for (int i = 0; i < 30; i++) {
       if (i % 2 == 0){
           medQ[q++] = QA[i];
       }
       else {
           medA[a++] = QA[i];
       }
   }
}

void hardQuestion(char QA[30][256],char *hardQ[15],char *hardA[15]) {
   readQuestionFile("HardQA.txt", QA);
   int q = 0;
   int a = 0;
   for (int i = 0; i < 30; i++) {
       if (i % 2 == 0){
           hardQ[q++] = QA[i];
       }
       else {
           hardA[a++] = QA[i];
       }
   }
}

void bossQuestion(char QA[30][256], char *bossQ[15], char *bossA[15]) {
   readQuestionFile("BossQA.txt", QA);
   int q = 0;
   int a = 0;
   for (int i = 0; i < 30; i++) {
       if (i % 2 == 0){
           bossQ[q++] = QA[i];
       }
       else {
           bossA[a++] = QA[i];
       }
   }
}

//checks if user answer is right or wrong by comparing to correct answer from the arrays containing the answer to the question posed for troll(“?”) encounters
void AnswerCheck(int userans, int correctans, Player *p1) {
    // checks if the numbers are equal
   if (userans == correctans) {
       printf("\nCorrect! Continue Onward!\n");
       //nothing gained, yet nothing lost for getting correct answer
   }
   else { //wrong answer
   printf("\nWRONG!! *PUNCHES YOU*\n");
       if (p1->shield > 0) {
           p1->shield -= 1; //if the player has shields, reduce value by 1
       }
       else {
           p1->health -= 1; // if no shields are present take 1 health instead
       }
   }
}

//checks if user answer is right or wrong by comparing to correct answer from the arrays containing the answer to the question posed for boss(“B”) encounters
//similar to prev function but has different consequences
void BossAnswerCheck(int userans, int correctans, Player *p1) {
   if (userans == correctans) {
       printf("\nYou're Smarter Than You Appear Mortal...\n");
   }
   else {

       if (p1->shield > 0) {
           printf("\nI will PRY you from that pathetic shell of metal so I may FEAST upon your ignorance at the SOURCE!!\n");
           p1->shield -= 1; //if shields present, take shields for wrong answer
       }
       else {
           printf("\nHAHAHA!! COUNT YOUR BREATHS MORTAL! YOU HAVE FEW LEFT TO COME!!!\n");
           p1->health -= 2; //if no shields take 2 health for wrong answer

       }
   }
}
