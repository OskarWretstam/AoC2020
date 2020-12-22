#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define START_DECK_SIZE 25
// #define START_DECK_SIZE 5

#define MAX_DECK_SIZE (2*START_DECK_SIZE)

#define LINE_LEN 11

#define ROUND_MEM_SIZE 5000

static int mem[ROUND_MEM_SIZE];

typedef struct
{
   int list[MAX_DECK_SIZE];
   int deck_size;
   int first;
   int last;
} deck_t;

void init_deck(deck_t *d)
{
   char line[LINE_LEN];
   char *s_ptr;
   int no = 0;

   for(s_ptr = fgets(line, LINE_LEN, stdin); (s_ptr!=NULL && line[0]!='\n'); s_ptr = fgets(line, LINE_LEN, stdin))
   {
      if(isdigit(line[0]))
      {
         d->list[no++] = atoi(line);
         d->last = no-1;
         d->deck_size++;
      }
   }
}

void print_deck(deck_t *d, char* name)
{
   int i;
   printf("%s's deck of %2d cards: ", name, d->deck_size);
   if(d->deck_size > 0)
   {
      for(i = d->first; i != d->last; i++)
      {
         i = i%MAX_DECK_SIZE;
         if(i == d->last)
         {
            break;
         }
         printf("%2d, ", d->list[i]);
      }
      printf("%2d\n", d->list[i]);
   }
   else
   {
      printf("\n");
   }
}

void card_handover(deck_t *w, deck_t *l)
{
   // The cards to add to winners end of list
   int winner_card = w->list[w->first];
   int looser_card = l->list[l->first];

   // Increment both players first card
   w->first++;
   w->first = w->first%MAX_DECK_SIZE;
   w->deck_size--;
   l->first++;
   l->first = l->first%MAX_DECK_SIZE;
   l->deck_size--;

   // Add cards to winners deck
   w->last++;
   w->last = w->last%MAX_DECK_SIZE;
   w->list[w->last] = winner_card;
   w->deck_size++;

   w->last++;
   w->last = w->last%MAX_DECK_SIZE;
   w->list[w->last] = looser_card;
   w->deck_size++;
}

int post_game_calc(deck_t *d)
{
   int sum = 0;
   int factor = MAX_DECK_SIZE;
   int i;
   for(i = d->first; i != d->last; i++)
   {
      i = i%MAX_DECK_SIZE;
      if(i == d->last)
      {
         break;
      }
      sum += factor*d->list[i];
      factor--;
   }
   sum += factor*d->list[i];

   return sum;
}

int play_game(deck_t *p1, deck_t *p2, int task)
{
   int p1_mem[ROUND_MEM_SIZE];
   int p2_mem[ROUND_MEM_SIZE];
   int nof_remember = 0, p1_config, p2_config;
   int p1_round_winner;

   while((p1->deck_size != 0) && (p2->deck_size != 0))
   {
      p1_config = post_game_calc(p1);
      p2_config = post_game_calc(p2);
      for(int i = 0; i < nof_remember; i++)
      {
         if(p1_mem[i] == p1_config &&
            p2_mem[i] == p2_config)
         {
            // We have been here before, P1 wins
            return 1;
         }
      }
      // Add this config to memory
      p1_mem[nof_remember] = p1_config;
      p2_mem[nof_remember] = p2_config;
      nof_remember++;
      if(nof_remember == ROUND_MEM_SIZE)
      {
         printf("Round memory is full..\n");
         while(1){sleep(1);}
      }

      if(p1->list[p1->first] <= p1->deck_size-1 &&
         p2->list[p2->first] <= p2->deck_size-1 &&
         task == 2)
      {
         // Create new decks and play again..
         deck_t p1_new, p2_new;
         memcpy(&p1_new, p1, sizeof(deck_t));
         p1_new.deck_size = p1->list[p1->first];
         p1_new.first = p1->first+1;
         p1_new.first = p1_new.first%MAX_DECK_SIZE;
         p1_new.last = p1_new.first+p1_new.deck_size-1;
         p1_new.last = p1_new.last%MAX_DECK_SIZE;

         memcpy(&p2_new, p2, sizeof(deck_t));
         p2_new.deck_size = p2->list[p2->first];
         p2_new.first = p2->first+1;
         p2_new.first = p2_new.first%MAX_DECK_SIZE;
         p2_new.last = p2_new.first+p2_new.deck_size-1;
         p2_new.last = p2_new.last%MAX_DECK_SIZE;

         p1_round_winner = play_game(&p1_new, &p2_new, task);
      }
      else if(p1->list[p1->first] > p2->list[p2->first])
      {
         // Player 1 wins!
         p1_round_winner = 1;
      }
      else
      {
         // Player 2 wins!
         p1_round_winner = 0;
      }

      if(p1_round_winner)
      {
         card_handover(p1, p2);
      }
      else
      {
         card_handover(p2, p1);
      }
   }

   return (p2->deck_size == 0);
}

int main(int argc, char *argv[])
{
   int task = 2;
   deck_t p1, p2;
   memset(&p1, 0, sizeof(deck_t));
   memset(&p2, 0, sizeof(deck_t));

   init_deck(&p1);
   init_deck(&p2);

   int p1_won = play_game(&p1, &p2, task);

   if(p1_won == 1)
   {
      printf("Winner Player1, score: %d\n", post_game_calc(&p1));
   }
   else
   {
      printf("Winner Player2, score: %d\n", post_game_calc(&p2));
   }

   return 0;
}
