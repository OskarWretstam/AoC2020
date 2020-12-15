#include <stdio.h>
#include <string.h>

// Max number including task 1 and 2!
#define MAX_NOF_NUMBERS_SPOKEN 30000000
// Requested number of turns for task 1
#define NOF_TURNS_SUB_TASK 2020

// Meta information for our lookup table
typedef struct
{
   int spoken;
   int prev_spoken;
} spoken_info_t;

int main(void)
{
   // index 0 means number 0 -> lookup!
   static spoken_info_t dat[MAX_NOF_NUMBERS_SPOKEN];
   // nothing has been said yet
   memset(dat, 0, MAX_NOF_NUMBERS_SPOKEN*sizeof(spoken_info_t));

   // Pussle data
   int start_data[6] = {2,0,1,9,5,19};
   int start_data_len = 6;

   // Keep track of turns and what was last said
   int turn = 0;
   int prev_spoken;

   // Initialise lookup with pussle data
   while(turn < start_data_len)
   {
      dat[start_data[turn]].spoken = turn + 1;
      prev_spoken = start_data[turn];
      turn++;
   }

   // Keep helpers
   int spoken_before, to_speak;
   // Execute all turns
   while(turn < MAX_NOF_NUMBERS_SPOKEN)
   {
      turn++;
      // I accutually don't understand why i check agains prev_spoken here.
      // Seems like it should be spoken but w.e.
      // I guess it has something to do with the same number showing up twice?
      spoken_before = (dat[prev_spoken].prev_spoken != 0);

      if(spoken_before == 1)
      {
         // Well it said to speak the difference between turns of speaking the number
         to_speak = dat[prev_spoken].spoken - dat[prev_spoken].prev_spoken;

         // Here I get the feeling that this logic is a lot more complicated than it needs to be
         spoken_before = (dat[to_speak].spoken != 0);

         if(spoken_before == 1)
         {
            dat[to_speak].prev_spoken = dat[to_speak].spoken;
            dat[to_speak].spoken = turn;
            if(turn == MAX_NOF_NUMBERS_SPOKEN || turn == NOF_TURNS_SUB_TASK)
            {
               printf("Speak %d on turn %d\n", to_speak, turn);
            }
            prev_spoken = to_speak;
         }
         else
         {
            // to_speak is new, add to list
            dat[to_speak].spoken = turn;
            if(turn == MAX_NOF_NUMBERS_SPOKEN || turn == NOF_TURNS_SUB_TASK)
            {
               printf("Speak %d on turn %d NEW\n", to_speak, turn);
            }
            prev_spoken = to_speak;
         }
      }
      else
      {
         // Look for zero since the number was not spoken before
         // Here I use spoken..
         spoken_before = (dat[0].spoken != 0);

         if(spoken_before == 1)
         {
            // Zero was spoken before, just move around the meta data.
            // Again no idea, might be tired
            dat[0].prev_spoken = dat[0].spoken;
            dat[0].spoken = turn;
            if(turn == MAX_NOF_NUMBERS_SPOKEN || turn == NOF_TURNS_SUB_TASK)
            {
               printf("Speak %d on turn %d\n", 0, turn);
            }
            prev_spoken = 0;
         }
         else
         {
            // Zero is new, add to list, some examples had this case..
            dat[0].spoken = turn;
            if(turn == MAX_NOF_NUMBERS_SPOKEN || turn == NOF_TURNS_SUB_TASK)
            {
               printf("Speak %d on turn %d\n", 0, turn);
            }
            prev_spoken = 0;
         }

      }
   }

   return 0;
}
