#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>

#define MAX_NAME 50
#define ROWS 10
#define COLUMNS 40

/*

usamos threads quando queremos executar várias funções de uma vez só
caso, por exemplo, criemos 3 threads, as 3 threads serão executadas ao mesmo tempo

- cria uma thread, salva o endereço em threads[0] e passa a rotina func (o que a thread deve fazer)
pthread_create(&(threads[0]), NULL, func, NULL);

- inicia a thread
pthread_join(threads[0], NULL);

*/

pthread_t threads[100];
char map[ROWS][COLUMNS];
int currentCol = 0, currentRow = 8, score = 0, attackRow = 0, attackColumn;

// Structs
typedef struct {
    char name[MAX_NAME];
    int score;
} Player;

void reloadMap();
void startGame();
void *playerAction();
Player playerData();

// Functions
Player playerData() {
    Player player;

    printf("Enter your nickname: ");
    fgets(player.name, MAX_NAME, stdin);

    player.name[strlen(player.name) - 1] = '\0';

    player.score = 0;

    return player;
}

void reloadMap() {
    // clear screen
    clear();

    for(int i = 0; i < ROWS; i++) {
        for(int j = 0; j < COLUMNS; j++) {
            printw("%c ", map[i][j]);
        }
        printw("\n");
    }

    // refresh screen
    refresh();
}

void startGame() {
    initscr(); // inicializa o modo de tela ncurses
    cbreak(); // desabilita linha de buffer
    noecho(); // teclas digitadas não são exibidas
    keypad(stdscr, TRUE); // permite entrada de caracteres especiais

    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            if(row == 0 || row == (ROWS - 1)) {
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

    pthread_create(&(threads[0]), NULL, playerAction, NULL);
    pthread_join(threads[0], NULL);
}

void *attack(void *arg) {
    int *attackParams = (int *)arg;
    int localAttackRow = attackParams[0];
    int localAttackColumn = attackParams[1];

    // Precisa ir de attackRow até ROWS - 1
    for(int lin = localAttackRow; lin > 0; lin--) {
        map[lin][localAttackColumn] = '*';

        sleep(1);

        reloadMap();

        // cada thread deve gerenciar sua própria condição de vida
        // pthread_exit(NULL); para encerar a thread (não encerra todas, apenas a que chegou nesse ponto)
    }
}

void *playerAction(void *arg) {
    while(1) {
        printw("%d", currentCol);

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
                // Para garantir que cada thread tenha seu ataque, não se usa attackRow e attackColumn diretamente
                int attackParams[2];

                attackRow = currentRow - 1;
                attackColumn = currentCol;

                attackParams[0] = attackRow;
                attackParams[1] = attackColumn;

                pthread_create(&(threads[0]), NULL, attack, (void *)attackParams);

                break;
            }
        }

        reloadMap();
    }
}

int main() {
    startGame();

    //Player player = playerData();
    // Save player name
    /*
    FILE *players_list;
    players_list = fopen("player.txt", "a");
    fprintf(players_list, player.name);
    */

    return 1;
}
