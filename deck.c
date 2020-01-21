#include "deck.h"

void add_card(struct card * c, struct hand * h){
  *(h->cards + h->size) = c;
  h->size += 1;
}

int add_str(char * str, struct hand * h){
  while (str != NULL){
    char * substr = strsep (&str, " ");
    if (strlen(substr) != 2){
      return 0;
    }
    if (strchr("RGBY",*(substr))==NULL || strchr("0123+RS",*(substr + 1))==NULL){
      return 0;
    }

    add_card(str_to_card(substr), h);
  }

  return 1;
}

struct card * str_to_card(char * str){
  struct card * c = malloc(sizeof(struct card));
  c->color = str[0];
  c->type = str[1];
  return c;
}

struct card * free_card(struct card * c){
  free(c);
  return NULL;
}
struct hand * free_hand(struct hand * h){
  int i;
  for (i = 0; i < h->size; i++){
    h->cards[i] = free_card(h->cards[i]);
  }

  free(h);
  return NULL;
}

int count_cards(struct card * c, struct hand * h){
  int count, i;
  count = 0;
  for (i = 0; i < h->size; i++){
    if (card_cmp(c, h->cards[i]) == 0){
      count++;
    }
  }
  return count;
}


//returns 0 if identical, -1 otherwise
//modify if we are implementing card sorting
int card_cmp(struct card * c0, struct card * c1){
  if (c0->color == c1->color && c0->type == c1->type){
    return 0;
  }
  return -1;
}

struct card * remove_hand(struct hand * h0, struct hand * h1){
  int i, max;
  struct card * c;
  max = h0->size;
  for (i = 0; i < max; i++){
    c = remove_card(h0->cards[i], h1);

  }
  return c;
}

struct card * remove_handh(struct hand * h0, struct hand * h1){
  int i, max;
  struct card * c;
  max = h0->size;
  for (i = 0; i < max; i++){
    c = remove_cardh(h0->cards[i], h1);

  }
  return c;
}

struct card * remove_card(struct card * c, struct hand * h){
  int i, max;
  max = h->size;
  for (i = max - 1; i > -1; i--){
    if (card_cmp(c, h->cards[i]) == 0){
      break;
    }
  }
  if (i == -1){
    printf("Card not removed.\n");
    return NULL;
  }
  //h->cards[i] = free_card(h->cards[i]);
  h->cards[i] = h->cards[max - 1];
  h->cards[max - 1] = NULL;
  h->size--;
  return c;
}

struct card * remove_cardh(struct card * c, struct hand * h){
  int i, max;
  max = h->size;
  for (i = max - 1; i > -1; i--){
    if (card_cmp(c, h->cards[i]) == 0){
      break;
    }
  }
  if (i == -1){
    printf("Card not removed.\n");
    return NULL;
  }
  h->cards[i] = free_card(h->cards[i]);
  h->cards[i] = h->cards[max - 1];
  h->cards[max - 1] = NULL;
  h->size--;
  return c;
}

int is_plus(struct card * c){
  if (c->type == '+'){
    return 1;
  }
  return 0;
}

int is_skip(struct card * c){
  if (c->type == 'S'){
    return 1;
  }
  return 0;
}
int is_rev(struct card * c){
  if (c->type == 'R'){
    return 1;
  }
  return 0;
}
