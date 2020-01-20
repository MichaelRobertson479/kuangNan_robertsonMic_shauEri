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

     //wait for game to start
     int spoon = fork();
     if (spoon == 0){
       while(1){
         printf("Welcome to the card game Tres!\n");
         printf("Waiting for first player to start the game!\n");
         sleep(100);
       }
     }

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

     //get waiting players array
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
     memcpy(&wpa[player_number], &spoon, sizeof(int));

     //wait for child to die (for game to start)
     wait(NULL);
  }

  //first player
  else{

    //create number of players
    nop = shmat(nop_key, 0, 0);
    if (nop == NULL){
      printf("error nop_shmat %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    //get player number
    *nop = 1;
    player_number = 1;

    //create direction
    dir_key = shmget(DIRECTION_KEY, sizeof(int), IPC_CREAT | 0644);
    if (dir_key == -1){
      printf("error dir_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }    
    direction = shmat(dir_key, 0, 0);
    *direction = 1;

    //create waiting players array
    wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), IPC_CREAT | IPC_EXCL | 0644);
    if (wpa_key == -1){
      printf("error wpa_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    int * wpa = (int *) shmat(wpa_key, 0, 0);

    //create turn count
    tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), IPC_CREAT | 0644);
    if (tc_key == -1){
      printf("error tc_key %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    tc = shmat(tc_key, 0, 0);
    *tc = 1;

    //input to start the game
    printf("Welcome to the card game Tres!\n");
    printf("Enter \"start\" into the terminal at any time to start the game!\n");
    fgets(buffer, 256, stdin);
    while (strcmp(buffer, "start\n") != 0){
      printf("Tip: If you wanted to start the game, enter \"start\"!\n");
      fgets(buffer, 256, stdin);
    }

    //tell other players that game has started
    for (i = 2; i <= *nop; i++){
      kill(wpa[i], SIGKILL);
      wpa[i] = 0;
    }
  }

  //show game has started, and player number
  printf("Fantastic! Now the game has begun!\n");
  printf("Your are player %d\n", player_number);


/////////////// After Initialization (Game Started) /////////////////////////

  //needs to be replaced with while the win condition (0 cards in hand) is not met
  while(1) {

    //display current game state

    if ((*tc % *nop) == (player_number % *nop)) {
      printf("It's your turn\n");

      //stand-in for playing cards
      sleep(3);

      //change turn
      *tc += *direction;

      //kill children except own
      for (i = 1; i <= *nop; i++) {
        if (i != player_number) {
          printf("i: %d\n", i);
          kill(wpa[i], SIGKILL);
          wpa[i] = 0;
        }
      }
      printf("gets here\n");
    }

    else {
      printf("It's not your turn yet\n");

      //save current turn locally
      int turn_local = *tc;

      //say whose turn it is now
      printf("It is player %d's turn\n", *tc % *nop);

      // //just wait till your turn
      // while ((*tc % *nop) != (player_number % *nop)) {

      //   //if turn changed, exit loop
      //   if(*tc > turn_local) {
      //       break;
      //   }

      //   //otherwise just wait
      //   sleep(1);
      // }


      int spoon = fork();
      memcpy(&wpa[player_number], &spoon, sizeof(int));
      if (spoon == 0){
        while(1){
          printf("Waiting for your turn...\n");
          sleep(100);
        }
      }

      wait(NULL);
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
