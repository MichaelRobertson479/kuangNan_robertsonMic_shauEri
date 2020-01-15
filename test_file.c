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

  int nop_key, wpa_key, tc_key;
  int player_number;
  int * nop;
  int wpa[10];
  int * tc;
  char buffer[256];
  int nop_end, wpa_end, tc_end;
  int nop_term, wpa_term, tc_term;

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
    fgets(buffer, 256, stdin);
    while (strcmp(buffer, "start\n") != 0){
      printf("Tip: If you wanted to start the game, enter \"start\"!\n");
      fgets(buffer, 256, stdin);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);
    int i;
    for (i = 2; i <= *nop; i++){
      kill(wpa[i], SIGKILL);
      wpa[i] = 0;
    }
    tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
    if (tc_key == -1){
      printf("error tc_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    tc = shmat(tc_key, 0, 0);
    *tc = 1;
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
  nop_end = shmdt(nop);
  if (nop_end == -1){
    printf("error nop_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  wpa_term = shmdt(wpa);
  if (wpa_end == -1){
    printf("error wpa_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  tc_end = shmdt(tc);
  if (tc_end == -1){
    printf("error tc_end %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  if (player_number == 1){
    nop_term = shmctl(nop_key, IPC_RMID, 0);
    if (nop_term == -1){
      printf("error nop_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    wpa_term = shmctl(wpa_key, IPC_RMID, 0);
    if (wpa_term == -1){
      printf("error wpa_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    tc_term = shmctl(tc_key, IPC_RMID, 0);
    if (tc_term == -1){
      printf("error tc_term %d: %s\n", errno, strerror(errno));
      exit(1);
    }
  }
  return 0;

}
