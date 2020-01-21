#include "draw.h"

struct card * draw(){
  char colors [4] = {'R', 'G', 'B', 'Y'};
  char types [7] = {'0', '1', '2', '3', '+', 'R', 'S'};
  struct card * c = malloc(sizeof(struct card));
  c->color = colors[rand() % 4];
  c->type = types[rand() % 7];
  return c;
}

struct card * draw_top(){
  char colors [4] = {'R', 'G', 'B', 'Y'};
  char types [4] = {'0', '1', '2', '3'};
  struct card * c = malloc(sizeof(struct card));
  c->color = colors[rand() % 4];
  c->type = types[rand() % 4];
  return c;
}


int draw_n(int n, struct hand * h){
  int i;
  for (i = 0; i < n; i++){
    *(h->cards + h->size) = draw();
    h->size += 1;
  }
  return n;
}
