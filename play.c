#include "play.h"

int * wpa;

struct card * play(struct card * top, struct hand * h1){
  //h1 is player's hand
  int turn_end_shm;
  int * turn_end;
  turn_end_shm = shmget(TURN_END_KEY, TURN_END_SEG_SIZE, IPC_CREAT | 0777);
  if (turn_end_shm == -1){
    printf("error turn_end_shm %d: %s\n", errno, strerror(errno));
    exit(1);
  }
  turn_end = shmat(turn_end_shm, 0, 0);
  *turn_end = 0;
  while(! * turn_end){
    char input[50];

    //print current game info
    printf("Top card: ");
    print_card(top);
    printf("\n");
    print_hand(h1);

    //prompts user for a move and stores the shit in input
    printf("Your move: ");
    fflush(stdout);
    fgets(input, 50, stdin);
    * strchr(input, '\n') = 0;

    //if quit: free all malloc shits and exits while loop
    if (strcmp(input, "quit") == 0){
      h1 = free_hand(h1);
      top = free_card(top);
      shmdt(turn_end);
      shmctl(turn_end_shm, IPC_RMID, 0);
      return str_to_card("QQ");
    }

    //if draw: draws cards appropriately:
    //1 if shm for draw is 0; draws shit stored in shm otherwise
    if (strcmp(input, "draw") == 0){
      int draw_shm;
      int * draw_val;
      draw_shm = shmget(DRAW_KEY, DRAW_SEG_SIZE, 0777);
      draw_val = shmat(draw_shm, 0, 0);
      if (*draw_val == 0){
        draw_n(1, h1);
      }
      else{
        draw_n(*draw_val, h1);
        *draw_val = 0;
      }
      shmdt(draw_val);
      * turn_end = 1;
    }

    //if entry is not draw/quit
    else{
      //interprets input as dumb hand to check for valid input
      top = play_cards(input, top, h1);
    }

    printf("\n");

    //if win condition (hand empty):
    //frees shit and exits while loop
    // if (h1->size == 0){
    //   h1 = free_hand(h1);
    //   top = free_card(top);
    //   printf("Congratulations. You win!\n");
    //   shmdt(turn_end);

    //   //make top card color 'W' to let other players know
    //   int topc_shm = shmget(TOPC_KEY, TOP_SEG_SIZE, 0777);
    //   char * topc = shmat(topc_shm, 0, 0);
    //   *topc = 'W';

    //   //make top card type equal to player number
    //   int topt_shm = shmget(TOPT_KEY, TOP_SEG_SIZE, 0777);
    //   char * topt = shmat(topt_shm, 0, 0);
    //   *topt = player_number + '0';

    //   //get wpa
    //   int wpa_key = shmget(WAITING_PLAYERS_ARRAY_KEY, sizeof(wpa), 0777);
    //   if (wpa_key == -1){
    //    printf("error wpa_key %d: %s\n", errno, strerror(errno));
    //    exit(1);
    //   }
    //   int * wpa = (int *) shmat(wpa_key, 0, 0);
    //   if (wpa == NULL){
    //    printf("error wpa_shmat %d: %s\n", errno, strerror(errno));
    //    exit(1);
    //   }

    //   //kill children
    //   for (int i = 1; i <= *nop; i++) {
    //     kill(wpa[i], SIGKILL);
    //   }


    //   //insert removal code here

    //   return NULL;
    // }
  }
  shmdt(turn_end);
  shmctl(turn_end_shm, IPC_RMID, 0);
  return top;
}

//playing cards, accepts string input, top card, and h
struct card * play_cards(char * input, struct card * top, struct hand * h){

  //gets shm for dumb draw shit
  int draw_shm;
  int * draw_val;
  int turn_end_shm;
  int * turn_end;
  draw_shm = shmget(DRAW_KEY, DRAW_SEG_SIZE, 0777);
  draw_val = shmat(draw_shm, 0, 0);

  turn_end_shm = shmget(TURN_END_KEY, TURN_END_SEG_SIZE, 0777);
  turn_end = shmat(turn_end_shm, 0, 0);
  //if player needs to draw cards/stack plus or whatever shit:
  if (*draw_val != 0){
    shmdt(draw_val);
    return play_cards_plus(input, top, h);
  }

  //if player does not need to draw cards
  else{

    //takes input and dumps input cards into a "playing" hand
    struct hand * playing;
    playing = create_hand(0);
    if (!add_str(input, playing)){
      printf("Error: Invalid input. Check format of cards.\n");
      playing = free_hand(playing);
      return top;
    }
    if (!valid_play(playing, h, top)){
      printf("Error: Invalid cards. Check sequence of cards.\n");
      playing = free_hand(playing);
      return top;
    }
    if (playing->size > 0){

      //check for skips and reverses
      for (int i = 0; i < playing->size; i++) {

        //if card is a skip
        if(playing->cards[i]->type == 'S') {

          //get turn count
          int tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), 0777);
          if (tc_key == -1){
            printf("error tc_key %d: %s\n", errno, strerror(errno));
            exit(1);
          }
          int * tc = shmat(tc_key, 0, 0);

          //get direction
          int dir_key = shmget(DIRECTION_KEY, sizeof(int), 0777);
          if (dir_key == -1){
            printf("error dir_key %d: %s\n", errno, strerror(errno));
            exit(1);
          }
          int * direction = shmat(dir_key, 0, 0);

          //change the turn an extra time
          *tc += *direction;
        }

        //if card is a reverse
        if(playing->cards[i]->type == 'R') {

          //get turn count
          int tc_key = shmget(TURN_COUNTER_KEY, sizeof(int), 0777);
          if (tc_key == -1){
            printf("error tc_key %d: %s\n", errno, strerror(errno));
            exit(1);
          }
          int * tc = shmat(tc_key, 0, 0);

          //get direction
          int dir_key = shmget(DIRECTION_KEY, sizeof(int), 0777);
          if (dir_key == -1){
            printf("error dir_key %d: %s\n", errno, strerror(errno));
            exit(1);
          }
          int * direction = shmat(dir_key, 0, 0);

          //change the direction
          *direction *= -1;
        }
      }

      * draw_val += count_draws(playing);
      free_card(top);
      top = remove_handh(playing, h);
      remove_card(top, playing);
      *turn_end = 1;
    }
    playing = free_hand(playing);
    shmdt(draw_val);
  }
  shmdt(turn_end);
  return top;
}

struct card * play_cards_plus(char * input, struct card * top, struct hand * h){
  int draw_shm;
  int * draw_val;
  int turn_end_shm;
  int * turn_end;

  turn_end_shm = shmget(TURN_END_KEY, TURN_END_SEG_SIZE, 0777);
  turn_end = shmat(turn_end_shm, 0, 0);
  draw_shm = shmget(DRAW_KEY, DRAW_SEG_SIZE, 0777);
  draw_val = shmat(draw_shm, 0, 0);
  struct hand * playing;
  playing = create_hand(0);
  if (!add_str(input, playing)){
    printf("error: invalid input. check format of cards.\n");
    playing = free_hand(playing);
    shmdt(turn_end);
    return top;
  }
  if (!valid_play_plus(playing, h, top)){
    printf("error: invalid cards. check sequence of cards.\n");
    playing = free_hand(playing);
    shmdt(turn_end);
    return top;
  }
  if (playing->size > 0){
    * draw_val += count_draws(playing);
    free_card(top);
    top = remove_handh(playing, h);
    remove_card(top, playing);
    *turn_end = 1;
  }
  shmdt(turn_end);
  playing = free_hand(playing);
  return top;
}

int count_draws(struct hand *p){
  int count;
  int i;
  count = 0;
  for (i = 0; i < p->size; i++){
    count += is_plus(p->cards[i]) * 2;
  }
  return count;
}

int valid_play_plus(struct hand *p, struct hand * h, struct card * top){
  if (contains_repeats(p, h)){
    return 0;
  }
  int i;
  for (i = 0; i < p->size; i++){
    if (!is_plus(p->cards[i])){
      return 0;
    }
  }
  return 1;
}

int valid_play(struct hand * p, struct hand * h, struct card * top){
  //check for repeats
  //we may be able to improve runtime but whatever :')'
  if (contains_repeats(p, h)){
    return 0;
  }

  //check for colors/numbers, make sure sum isnt 0
  int c;
  int t;
  c = colors_match(top, p);
  t = types_match(top, p);
  if (c + t == 0){
    return 0;
  }

  return 1;
}

//also check for all pluses, or all normals
int colors_match(struct card * c, struct hand * p){
  int i;
  char col;
  col = c->color;
  for (i = 0; i < p->size; i++){
    if (col != p->cards[i]->color){
      return 0;
    }
  }
  return gen_types_match(c, p);
}

int gen_types_match(struct card * c, struct hand * p){
  int i;
  int t0;
  t0 = is_plus(p->cards[0]);
  for (i = 1; i < p->size; i++){
    if (t0 != is_plus(p->cards[i])){
      return 0;
    }
  }
  t0 = is_skip(p->cards[0]);
  for (i = 1; i < p->size; i++){
    if (t0 != is_skip(p->cards[i])){
      return 0;
    }
  }
  t0 = is_rev(p->cards[0]);
  for (i = 1; i < p->size; i++){
    if (t0 != is_rev(p->cards[i])){
      return 0;
    }
  }
  return 1;
}

int types_match(struct card * c, struct hand * p){
  int i;
  char t;
  t = c->type;
  for (i = 0; i < p->size; i++){
    if (t != p->cards[i]->type){
      return 0;
    }
  }
  return 1;
}

int contains_repeats(struct hand * p, struct hand * h){
  int i;
  for (i = 0; i < p->size; i++){
    if (count_cards(p->cards[i], p) > count_cards(p->cards[i], h)){
      return 1;
    }
  }
  return 0;
}


struct hand * create_hand(int n){
  struct hand * h = malloc(sizeof(struct hand));
  h->size = 0;
  draw_n(n, h);
  return h;
}
