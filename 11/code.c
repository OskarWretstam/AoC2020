#include <stdio.h>
#include <string.h>

#define NOF_TASKS 2
#define GIVE_UP 500
#define FRAME 'x'
#define EMPTY 'L'
#define OCCUPIED '#'
// The file with added x's as a frame
#define NOF_ROWS 93
#define NOF_COLS 98
#define FILENAME "data.txt"

// Used for test_data.txt
// #define NOF_ROWS 12
// #define NOF_COLS 12
// #define FILENAME "test_data.txt"

#define NOF_ELEMENTS NOF_ROWS*NOF_COLS


int count_occupied(
   char *state,        // The char matrix state of the seats
   int ix,             // Currently tested position
   int proximit_limit  // Limits number of chairs away to check
   )
{
   int cnt = 0;
   int i;

   // Walk upwards
   i = 1;
   while(state[ix-i*NOF_COLS] != FRAME && state[ix-i*NOF_COLS] != EMPTY && i != proximit_limit)
   {
      if(state[ix-i*NOF_COLS] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   // Walk downwards
   i = 1;
   while(state[ix+i*NOF_COLS] != FRAME && state[ix+i*NOF_COLS] != EMPTY && i != proximit_limit)
   {
      if(state[ix+i*NOF_COLS] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   // Walk to the left
   i = 1;
   while(state[ix-i] != FRAME && state[ix-i] != EMPTY && i != proximit_limit)
   {
      if(state[ix-i] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   // Walk to the right
   i = 1;
   while(state[ix+i] != FRAME && state[ix+i] != EMPTY && i != proximit_limit)
   {
      if(state[ix+i] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   // Diagonally right and up
   i = 1;
   while(state[ix-i*NOF_COLS+i] != FRAME && state[ix-i*NOF_COLS+i] != EMPTY && i != proximit_limit)
   {
      if(state[ix-i*NOF_COLS+i] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   // Diagonally left and up
   i = 1;
   while(state[ix-i*NOF_COLS-i] != FRAME && state[ix-i*NOF_COLS-i] != EMPTY && i != proximit_limit)
   {
      if(state[ix-i*NOF_COLS-i] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   // Diagonally left and down
   i = 1;
   while(state[ix+i*NOF_COLS-i] != FRAME && state[ix + i*NOF_COLS-i] != EMPTY && i != proximit_limit)
   {
      if(state[ix+i*NOF_COLS-i] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   // Diagonally right and down
   i = 1;
   while(state[ix+i*NOF_COLS+i] != FRAME && state[ix + i*NOF_COLS+i] != EMPTY && i != proximit_limit)
   {
      if(state[ix+i*NOF_COLS+i] == OCCUPIED)
      {
         cnt++;
         break;
      }
      i++;
   }

   return cnt;
}

int main(void)
{
   // We need one working state and one to add results to
   char state[NOF_ELEMENTS*2];
   // Task variations
   int nof_neighbor_rules[2] = {4, 5};
   int proximity_limit[2] = {2, -1}; // -1 used for no limit
   // Use mod of nof_loops to index the state array correctly
   int nof_loops, mod_remainder, offset_sign;
   // More helpers
   int occupied_neighbors;
   // File reading variables
   FILE *fp;
   char c;
   int nof_input_chars;
   // Tracking occupied seats in each state to detect steady state
   int nof_occupied_tot[2];

   for(int k = 0; k < NOF_TASKS; k++)
   {
      // Start of with everything as frame for convenience
      memset(state, FRAME, NOF_ELEMENTS*2);

      // Initialise first state from file data
      fp = fopen(FILENAME, "r");
      nof_input_chars = 0;
      while(1)
      {
         c = fgetc(fp);
         if(c == EOF)
         {
            break;
         }
         if(c != '\n')
         {
            state[nof_input_chars++] = c;
         }
      }
      fclose(fp);

      memset(nof_occupied_tot, 0, NOF_TASKS*sizeof(int));
      nof_loops = 0;
      while(nof_loops <= GIVE_UP)
      {
         // 0, 1, 0, 1...
         mod_remainder = nof_loops%2;
         // 1, -1, 1, -1...
         offset_sign = 1-2*mod_remainder;
         nof_loops++;
         nof_occupied_tot[!mod_remainder] = 0;

         // Rows will be offet by a complete state every other loop
         for(int i = 1+NOF_ROWS*mod_remainder; i < NOF_ROWS-1+NOF_ROWS*mod_remainder; i++)
         {
            // Columns are unaffacted by the double state arrary
            for(int j = 1; j < NOF_COLS-1; j++)
            {
               // First check if we are at a position that should be dealt with
               if(state[i*NOF_COLS+j] == EMPTY || state[i*NOF_COLS+j] == OCCUPIED)
               {
                  // Count occupied neighbors
                  occupied_neighbors = count_occupied(state, i*NOF_COLS+j, proximity_limit[k]);

                  // Rules
                  if(state[i*NOF_COLS+j] == EMPTY && occupied_neighbors == 0)
                  {
                     // Replace at current position +/- nof elements in a state
                     state[i*NOF_COLS+j+NOF_ELEMENTS*offset_sign] = OCCUPIED;
                     // Increment nof occupied for the next state
                     nof_occupied_tot[!mod_remainder]++;
                  }
                  else if(state[i*NOF_COLS+j] == OCCUPIED && occupied_neighbors >= nof_neighbor_rules[k])
                  {
                     state[i*NOF_COLS+j+NOF_ELEMENTS*offset_sign] = EMPTY;
                  }
                  else
                  {
                     // Transfer unchanged positions and potentially increment occupieds
                     if(state[i*NOF_COLS+j] == OCCUPIED)
                     {
                        nof_occupied_tot[!mod_remainder]++;
                     }
                     state[i*NOF_COLS+j+NOF_ELEMENTS*offset_sign] = state[i*NOF_COLS+j];
                  }
               }
               else
               {
                  // Transfer floor and frame, really just neccessary for first invokation
                  state[i*NOF_COLS+j+NOF_ELEMENTS*offset_sign] = state[i*NOF_COLS+j];
               }
            }
         }

         if(nof_occupied_tot[0] == nof_occupied_tot[1])
         {
            // We are done :)
            break;
         }
      }

      // I don't like infinite loops
      if(nof_loops >= GIVE_UP)
      {
         return 1;
      }

      printf("Task %d, occupied steady state: %d resolved in %d loops\n", k, nof_occupied_tot[0], nof_loops);
   }

   return 0;
}
