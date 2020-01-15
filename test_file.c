#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/wait.h>

#define NUMBER_OF_PLAYERS_KEY 222
#define WAITING_PLAYERS_ARRAY_KEY 2222
#define TURN_COUNTER_KEY 22222

int main(){

  int nop_key;
  int wpa_key;
  int tc_key;
  int player_number;
  int * nop;
  int wpa[10];
  int * turn_counter;
  char buffer[7];

  nop_key = shmget(NUMBER_OF_PLAYERS_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  if (nop_key == -1){
     nop_key = shmget(NUMBER_OF_PLAYERS_KEY, sizeof(int), 0644);
     if (nop_key == -1){
       printf("error nop_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     nop = shmat(nop_key, 0, 0);
     if (nop == NULL){
       printf("error nop_shmat %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     (*nop)++;
     player_number = *nop;
     int spoon = fork();
     if (spoon == 0){
       while(1){
         printf("Welcome to the card game Tres!\n");
         printf("Waiting for first player to start the game!\n");
         sleep(100);
       }
     }
     wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), 0644);
     if (wpa_key == -1){
       printf("error wpa_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     int * wpa = (int *) shmat(wpa_key, 0, 0);
     if (wpa == NULL){
       printf("error wpa_shmat %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     wpa[player_number] = spoon;
     wait(NULL);
  }
  else{
    nop = shmat(nop_key, 0, 0);
    if (nop == NULL){
      printf("error nop_shmat %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    *nop = 1;
    player_number = 1;
    wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), IPC_CREAT | IPC_EXCL | 0644);
    if (wpa_key == -1){
      printf("error wpa_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    printf("Welcome to the card game Tres!\n");
    printf("Enter \"start\" into the terminal at any time to start the game!\n");
    fgets(buffer, 7, stdin);
    while (strcmp(buffer, "start\n") != 0){
      printf("Tip: If you wanted to start the game, enter \"start\"!\n");
      fgets(buffer, 7, stdin);
      sleep(0.01);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);
    int i;
    printf("%d\n", *nop);
    for (i = 2; i <= *nop; i++){
      kill(wpa[i], SIGKILL);
      wpa[i] = 0;
    }
    tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    if (tc_key == -1){
      printf("error tc_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    turn_counter = shmat(tc_key, 0, 0);
    *turn_counter = 1;
  }

  printf("Fantastic! Now the game has begun!\n");
  if (player_number != 1){
    int spoon = fork();
    if (spoon == 0){
      while(1){
        printf("Waiting for your turn...\n");
        sleep(100);
      }
    }
    wpa[player_number] = spoon;
    wait(NULL);
  }

  return 0;

}
