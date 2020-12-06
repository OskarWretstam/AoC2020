#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOF_QUESTIONS 26
#define ASCII_LOWER_CASE_A 97

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   if(fp == NULL)
   {
      printf("Failed to open file for reading.\n");
      return 1;
   }

   // Buffer number of yes answers to any question, by subtracting ASCII_LOWER_CASE_A
   // 0 = a and 25 = z
   int char_cnt[NOF_QUESTIONS];
   memset(char_cnt, 0, NOF_QUESTIONS*sizeof(int));
   char c;
   int prev_newline;
   int nof_groups = 0, nof_persons_in_grp = 0;
   int anyone_yes = 0, all_yes = 0;

   while(1)
   {
      c = fgetc(fp);
      if(c == '\n' || c == EOF)
      {
         if(prev_newline)
         {
            // \n\n or \n EOF means we have all the data for a group
            nof_groups++;

            for(int i = 0; i < NOF_QUESTIONS; i++)
            {
               if(char_cnt[i] > 0)
               {
                  // At least one answered yes to question i - increment part 1 counter
                  anyone_yes++;
               }
               if(char_cnt[i] == nof_persons_in_grp)
               {
                  // Everyone answered yes to question i - increment part 2 counter
                  all_yes++;
               }
            }

            // Reset
            memset(char_cnt, 0, NOF_QUESTIONS*sizeof(int));
            nof_persons_in_grp = 0;
         }
         else
         {
            // singe \n, new person in the group
            nof_persons_in_grp++;
         }

         if( c == EOF )
         {
            // File is done
            break;
         }

         // Was not EOF, we had a newline
         prev_newline = 1;
      }
      else
      {
         // Read a character
         prev_newline = 0;
         // Increment at the appropriate postion
         char_cnt[c-ASCII_LOWER_CASE_A]++;
      }

   }

   fclose(fp);
   printf("Number of groups: %d part 1, anyone yes: %d part 2, all yes: %d\n", nof_groups, anyone_yes, all_yes);

}
