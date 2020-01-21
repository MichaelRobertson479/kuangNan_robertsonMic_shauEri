#include "headers.h"


void add_card(struct card * c, struct hand * h);
int add_str(char * str, struct hand * h);
struct card * str_to_card(char * str);
struct card * free_card(struct card * c);
struct hand * free_hand(struct hand * h);
int count_cards(struct card * c, struct hand * h);
int card_cmp(struct card * c0, struct card * c1);
struct card * remove_hand(struct hand * h0, struct hand * h1);
struct card * remove_card(struct card * c, struct hand * h);
struct card * remove_handh(struct hand * h0, struct hand * h1);
struct card * remove_cardh(struct card * c, struct hand * h);
int is_plus(struct card * c);
int is_skip(struct card * c);
int is_rev(struct card * c);
