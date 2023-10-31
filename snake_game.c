#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NAME 50

/*

usamos threads quando queremos executar várias funções de uma vez só
caso, por exemplo, criemos 3 threads, as 3 threads serão executadas ao mesmo tempo

- cria uma thread, salva o endereço em threads[0] e passa a rotina func (o que a thread deve fazer)
pthread_create(&(threads[0]), NULL, func, NULL);

- inicia a thread
pthread_join(threads[0], NULL);

*/

pthread_t threads[100];

typedef struct {
    char name[MAX_NAME];
    int score;
} Player;

Player playerData() {
    Player player;

    printf("Enter your nickname: ");
    fgets(player.name, MAX_NAME, stdin);

    player.score = 0;

    return player;
}

// Creation thread rotine (create five threads)
void *func(void *arg) {
    int i;

    for(i = 0; i <= 5; i++) {
        printf("  *\n", i);
        sleep(1);
    }
}

int main() {
    Player player = playerData();

    system("clear");

    // Save player name
    FILE *players_list;
    players_list = fopen("player.txt", "a");
    fprintf(players_list, player.name);

    pthread_create(&(threads[0]), NULL, func, NULL);
    pthread_create(&(threads[1]), NULL, func, NULL);
    pthread_create(&(threads[2]), NULL, func, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);

    return 1;
}
