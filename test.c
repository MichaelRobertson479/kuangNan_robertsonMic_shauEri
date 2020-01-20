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
#define DIRECTION_KEY 222222

int main(){

  int nop_key, wpa_key, tc_key, dir_key;
  int player_number;
  int * direction;
  int * nop;
  int wpa[10];
  int * tc;
  char buffer[256];
  int nop_end, wpa_end, tc_end;
  int nop_term, wpa_term, tc_term;
  int i;

  nop_key = shmget(NUMBER_OF_PLAYERS_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);

  // not first player
  if (nop_key == -1){

     //get number of players
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

     //get player number
     (*nop)++;
     player_number = *nop;
     printf("player_number: %d\n", player_number);

     //get turn count
     tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), 0644);
     if (tc_key == -1){
       printf("error tc_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     tc = shmat(tc_key, 0, 0);

     //get direction
     dir_key = shmget(DIRECTION_KEY, sizeof(int), 0644);
     if (dir_key == -1){
       printf("error dir_key %d: %s\n", errno, strerror(errno));
       exit(1);
     }
     direction = shmat(dir_key, 0, 0);

     //get waiting player array
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

     //wait for start
     int spoon = fork();
     if (spoon == 0){
       while(1){
         printf("Welcome to the card game Tres!\n");
         printf("Waiting for first player to start the game!\n");
         sleep(100);
       }
     }
     memcpy(&wpa[player_number], &spoon, sizeof(spoon));
     //printf("wpa[%d], child process id: %d\n", player_number, spoon);
     //printf("wpa[%d], child process id: %d\n", player_number, wpa[player_number]);

     wpa_term = shmdt(wpa);
     if (wpa_end == -1){
       printf("error wpa_end %d: %s\n", errno, strerror(errno));
       exit(1);
     }

     wait(NULL);
  }

  //first player
  else{

    //get number of players
    nop = shmat(nop_key, 0, 0);
    if (nop == NULL){
      printf("error nop_shmat %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    //get player number
    *nop = 1;
    player_number = 1;

    //get direction
    dir_key = shmget(DIRECTION_KEY, sizeof(int), IPC_CREAT | 0644);
    if (dir_key == -1){
      printf("error dir_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    direction = shmat(dir_key, 0, 0);
    *direction = 1;

    //get waiting players array
    wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), IPC_CREAT | 0644);
    if (wpa_key == -1){
      printf("error wpa_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);

    //get turn count
    tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), IPC_CREAT | 0644);
    if (tc_key == -1){
      printf("error tc_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    tc = shmat(tc_key, 0, 0);
    *tc = 1;

    printf("Welcome to the card game Tres!\n");
    printf("Enter \"start\" into the terminal at any time to start the game!\n");
    fgets(buffer, 256, stdin);
    while (strcmp(buffer, "start\n") != 0){
      printf("Tip: If you wanted to start the game, enter \"start\"!\n");
      fgets(buffer, 256, stdin);
    }

    for (i = 2; i <= *nop; i++){
      //printf("wpa[%d] = %d\n", i, wpa[i]);
      kill(wpa[i], SIGKILL);
      //wpa[i] = 0;
    }

    wpa_term = shmdt(wpa);
    if (wpa_end == -1){
      printf("error wpa_end %d: %s\n", errno, strerror(errno));
      exit(1);
    }

  }

  printf("Fantastic! Now the game has begun!\n");
  
  /////////////// After Initialization (Game Started) /////////////////////////
  while(1) {

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

    printf("It is turn %d\n", *tc);
    printf("There are %d players\n", *nop);
    printf("I am player %d\n", player_number);

    if ((*tc % *nop) == (player_number % *nop)) {
      printf("It's your turn\n");

      //stand-in for playing cards
      sleep(3);

      //change turn
      *tc += *direction;

      //kill children except own
      for (i = 1; i <= *nop; i++) {
        //printf("killing wpa[%d] = %d\n", i, wpa[i]);
        if (i != player_number && wpa[i] != 0) {
          //printf("actual kill wpa[%d] = %d\n", i, wpa[i]);
          kill(wpa[i], SIGKILL);
          //wpa[i] = 0;
        }
      }
    }

    else {
      printf("It's not your turn yet\n");
      // printf("Pre:\n");
      // for (i = 1; i <= *nop; i++) {
      //   printf("wpa[%d] = %d\n", i, wpa[i]);
      // }
      int spoon = fork();
      if (spoon == 0){
        while(1){
          printf("Waiting for your turn...\n");
          sleep(100);
        }
      }
      memcpy(&wpa[player_number], &spoon, sizeof(spoon));
      //printf("wpa[%d], child process id: %d\n", player_number, wpa[player_number]);
      //printf("Post:\n");
      // for (i = 1; i <= *nop; i++) {
      //   printf("wpa[%d] = %d\n", i, wpa[i]);
      // }
      wait(NULL);
      //printf("Why?\n");
    }

    wpa_term = shmdt(wpa);
    if (wpa_end == -1){
      printf("error wpa_end %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    // REMOVAL CODE BELOW
    // nop_end = shmdt(nop);
    // if (nop_end == -1){
    //   printf("error nop_end %d: %s\n", errno, strerror(errno));
    //   exit(1);
    // }
    // wpa_term = shmdt(wpa);
    // if (wpa_end == -1){
    //   printf("error wpa_end %d: %s\n", errno, strerror(errno));
    //   exit(1);
    // }
    // tc_end = shmdt(tc);
    // if (tc_end == -1){
    //   printf("error tc_end %d: %s\n", errno, strerror(errno));
    //   exit(1);
    // }
    // if (player_number == 1){
    //   nop_term = shmctl(nop_key, IPC_RMID, 0);
    //   if (nop_term == -1){
    //     printf("error nop_term %d: %s\n", errno, strerror(errno));
    //     exit(1);
    //   }
    //   wpa_term = shmctl(wpa_key, IPC_RMID, 0);
    //   if (wpa_term == -1){
    //     printf("error wpa_term %d: %s\n", errno, strerror(errno));
    //     exit(1);
    //   }
    //   tc_term = shmctl(tc_key, IPC_RMID, 0);
    //   if (tc_term == -1){
    //     printf("error tc_term %d: %s\n", errno, strerror(errno));
    //     exit(1);
    //   }
    // }
  }
  return 0;

}
