#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 12

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   int nof_boarding_pass = 0, max_id = 0, min_id = 1024;
   char line[LINE_LENGTH];
   int start,stop;
   int row,col;

   int id_arr[1024];
   memset(id_arr, 0, 1024*sizeof(int));

   while(fgets(line, 12, fp) != NULL)
   {
      nof_boarding_pass++;

      start = 0; stop = 127;
      for(int i = 0; i < 7; i++)
      {
         if(line[i] == 'F')
         {
            stop = (stop-start-1)/2 + start;
         }
         else if(line[i] == 'B')
         {
            start = (stop-start+1)/2 + start;
         }
      }
      row = start;

      start = 0; stop = 7;
      for(int i = 7; i < 10; i++)
      {
         if(line[i] == 'L')
         {
            stop = (stop-start-1)/2 + start;
         }
         else if(line[i] == 'R')
         {
            start = (stop-start+1)/2 + start;
         }
      }
      col = start;

      if(row*8+col > max_id)
      {
         max_id = row*8+col;
      }
      if(row*8+col < min_id)
      {
         min_id = row*8+col;
      }
      id_arr[row*8+col] = 1;
   }

   printf("Tested %d boarding passes, max id %d\n", nof_boarding_pass, max_id);

   for(int i = min_id; i < max_id; i++)
   {
      if(id_arr[i] == 0)
      {
         printf("My seat ID is: %d\n", i);
      }
   }
}
