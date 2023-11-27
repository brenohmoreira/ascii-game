#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>

#define MAX_NAME 50
#define ROWS 10
#define COLUMNS 40

char map[ROWS][COLUMNS];
int currentCol = 0, currentRow = 8, score = 0, attackRow = 0, attackColumn, gameStatus = 0, numAttacks = 0, counterEnemy = 0, counterAttack = 0;

pthread_t thread_enemy[500];
pthread_t thread_attack[100];
pthread_t thread_player_action;
pthread_t thread_invasion;
pthread_mutex_t map_mutex = PTHREAD_MUTEX_INITIALIZER;


// prototypes
void reloadMap();
void defeatScreen();
void menu();
void startGame();
void *playerAction();
void *invasion();
void *enemy();

// structs
typedef struct {
    char name[MAX_NAME];
    int score;
} Player;

Player playerData();

// functions
Player playerData() {
    Player player;

    getchar();

    system("clear");

    printf("Enter your nickname: ");
    fgets(player.name, MAX_NAME, stdin);

    player.name[strlen(player.name) - 1] = '\0';

    player.score = 0;

    return player;
}

void reloadMap() {
    pthread_mutex_lock(&map_mutex);
    pthread_mutex_unlock(&map_mutex);

    clear();

    printw("Status: %d\n", score);

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            printw("%c ", map[i][j]);
        }
        printw("\n");
    }

    refresh();
}

void menu() {
    int op;

    do {
        system("clear");

        printf("-------------------------------------------------\n|\t\tSPACE INVADERS\t\t\t|\n-------------------------------------------------\n");
        printf("1 - Start\n2 - Players\n3 - Exit\nEnter option: ");
        scanf("%d", &op);

        switch(op) {
            case 1: {
                Player player = playerData();

                FILE *players_list;
                players_list = fopen("player.txt", "a");
                fprintf(players_list, strcat(player.name, "\n"));

                fflush(players_list);
                fclose(players_list);

                startGame();

                break;
            }
            case 2: {
                break;
            }
            case 3: {
                exit(1);

                break;
            }
        }
    }
    while(op < 5 && op > 1);
}

void defeatScreen() {
    system("clear");
}

void startGame() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    gameStatus = 1;

    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            if(row == (ROWS - 1)) {
                map[row][column] = '#';
            }
            else {
                map[row][column] = ' ';
            }
        }
    }

    map[currentRow][currentCol] = '^';

    for(int row = 0; row < ROWS; row++) {
        for(int column = 0; column < COLUMNS; column++) {
            printw("%c ", map[row][column]);
        }
        printw("\n");
    }

    pthread_create(&(thread_player_action), NULL, playerAction, NULL);
    pthread_create(&(thread_invasion), NULL, invasion, NULL);

    pthread_join(thread_player_action, NULL);
    pthread_join(thread_invasion, NULL);
}

// thread rotines
void *enemy(void *arg) {
    int columnEnemy = 1 + (rand() % 38);
    int rowEnemy;

    for (rowEnemy = 0; rowEnemy < ROWS - 1 && gameStatus == 1; rowEnemy++) {
        if(map[rowEnemy][columnEnemy] == ' ') {
            if(rowEnemy != 0 && rowEnemy != ROWS - 1) {
                map[rowEnemy - 1][columnEnemy] = ' ';
            }

            map[rowEnemy][columnEnemy] = 'v';

            reloadMap();

            usleep(800000);
        }
        // ESSA BUDEGA DESGRAÇADA NÃO CONSEGUE CAPTAR TODA COLISÃO
        else if(map[rowEnemy][columnEnemy] == '*') {
            map[rowEnemy - 1][columnEnemy] = 'X';

            reloadMap();

            pthread_exit(NULL);
        }
    }

    gameStatus = 0;

    defeatScreen();

    pthread_exit(NULL);
}

void *invasion(void *arg) {
    while (gameStatus) {
        pthread_create(&(thread_enemy[counterEnemy++]), NULL, enemy, NULL);
        usleep(3000000);
    }

    pthread_exit(NULL);
}

void *attack(void *arg) {
    int *attackParams = (int*) arg;
    int localAttackRow = attackParams[0];
    int localAttackColumn = attackParams[1];

    for(int lin = localAttackRow; lin >= 0; lin--) {
        if (map[lin][localAttackColumn] == 'v') {
            score += 10;
            map[lin + 1][currentCol] = ' ';
            reloadMap();
            pthread_exit(NULL);
        }
        else {
            if (lin + 1 < ROWS) {
                map[lin + 1][localAttackColumn] = ' ';
            }

            map[localAttackRow + 1][currentCol] = '^';

            map[lin][localAttackColumn] = '*';

            usleep(150000);

            reloadMap();
        }
    }

    map[0][localAttackColumn] = ' ';

    reloadMap();

    pthread_exit(NULL);
}

void *playerAction(void *arg) {
    while(gameStatus) {
        switch(getch()) {
            case KEY_LEFT:
            {
                if(currentCol > 0) {
                    map[currentRow][currentCol] = ' ';
                    currentCol--;
                    map[currentRow][currentCol] = '^';
                }

                break;
            }
            case KEY_RIGHT:
            {
                if(currentCol < COLUMNS - 1) {
                    map[currentRow][currentCol] = ' ';
                    currentCol++;
                    map[currentRow][currentCol] = '^';
                }

                break;
            }
            case 32:
            {
                int attackParams[2];

                attackRow = currentRow - 1;
                attackColumn = currentCol;

                attackParams[0] = attackRow;
                attackParams[1] = attackColumn;

                pthread_create(&(thread_attack[counterAttack++]), NULL, attack, (void *)attackParams);

                break;
            }
        }

        reloadMap();
    }

    pthread_exit(NULL);
}


// main function
int main() {
    menu();

    return 1;
}
