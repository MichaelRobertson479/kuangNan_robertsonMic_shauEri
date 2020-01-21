#include "display.h"

//print start game text
void print_intro(){
  printf("Welcome to fake tres!\n");
  printf("\nInstructions:\n");
  printf("To play one or more cards, enter the cards that you would like to play ");
  printf("separated by a space, in the order that you would like to play them in.\n");
  printf("Note: you may play multiple cards if they are of the same color or of ");
  printf("the same type as the top card.\n");
  printf("To draw, enter \"draw\" when prompted.\n");
  printf("To quit, enter \"quit\".\n");
  printf("Refer to the README thing for more detailed instructions.\n");
  printf("Enjoy!\n\n");
}

//print each card (ex: R7)
void print_card(struct card * c){
  printf("%c%c", c->color, c-> type);
}

//print each hand, followed by newline (ex:hand: R1 B2 G3 Y+\n)
void print_hand(struct hand * h){
  int i;
  printf("Hand: ");
  for (i = 0; i < h->size; i++){
    print_card(*(h->cards + i));
    printf(" ");
  }
  printf("\n");
}
