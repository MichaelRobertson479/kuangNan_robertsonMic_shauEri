// headers
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

// headers
// card
struct cards {
  char color[10];
  char type[10];
};
// represent card
void print_card(struct cards * card){
  printf("%s %s, ", card->color, card->type);
}
// hand
struct hands {
    struct cards * card_hand[109];
    int size;
};
// hand
// represent card
struct hands * hand;
void print_hand(struct hands * hand){
  int i;
  for (i = 0; i < hand->size; i++){
    print_card(*(hand->card_hand + i));
  }
  printf("\n");
}
struct hands * deck;
// fill deck
void add_card(struct hands * deck, struct cards * card){
  *(deck->card_hand + deck->size) = card;
  deck->size = deck->size + 1;
}
// make deck
void make_deck(struct hands * deck){
  deck->size = 0;
  char * colors[4] = {"Red", "Green", "Blue", "Yellow"};
  char * types[13] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "+2", "Reverse", "Skip"};
  int i;
  int j;
  int k;
  int l;
  for (i = 0; i < 4; i++){
    for (j = 0; j < 13; j++){
      if (strcmp(types[j], "0") == 0){
        l = 1;
      }
      else{
        l = 2;
      }
      for (k = 0; k < l; k++){
        struct cards * card = malloc(sizeof(struct cards));
        strcpy(card->color, colors[i]);
        strcpy(card->type, types[j]);
        add_card(deck, card);
      }
    }
  }
  char * special_types[2] = {"Card", "+4"};
  for (i = 0; i < 2; i++){
    for (j = 0; j < 4; j++){
      struct cards * card = malloc(sizeof(struct cards));
      strcpy(card->color, "Wild");
      strcpy(card->type, special_types[i]);
      add_card(deck, card);
    }
  }
}
int main(int argc, char *argv[]) {
    deck = malloc(sizeof(struct cards) * 180 + sizeof(int));
    make_deck(deck);
    print_hand(deck);
    printf("%d\n", deck->size);
    int i;
    // for (i = 0; i < 108; i++){
    //   free(deck->card_hand[i]);
    // }
    free(deck);
    return 0;
}
