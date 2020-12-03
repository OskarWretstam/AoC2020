#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 40
#define NOF_SLOPES_CHECKED 5

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   if(fp == NULL)
   {
      printf("Failed to open data file.\n");
      return 1;
   }

   int no_parsed = 0, no_trees[NOF_SLOPES_CHECKED];
   memset(no_trees, 0, NOF_SLOPES_CHECKED*sizeof(int));
   char line[MAX_LINE_LENGTH];
   int line_len = 0;
   int x = 0, y = 0;
   char tree = '#';
   int dx[NOF_SLOPES_CHECKED] = {1, 3, 5, 7, 1};
   int dy[NOF_SLOPES_CHECKED] = {1, 1, 1, 1, 2};
   long long unsigned product;

   for(int i = 0; i < NOF_SLOPES_CHECKED; i++)
   {
      while(!feof(fp) && fscanf(fp, "%s\n", line) == 1)
      {
         no_parsed++;

         if(y%dy[i] == 0)
         {
            line_len = strlen(line);

            if(line[x%line_len] == '#')
            {
               no_trees[i]++;
            }

            x += dx[i];
         }
         y++;
      }

      printf("Parsed: %d lines, Found: %d trees.\n", no_parsed, no_trees[i]);

      // Reset some variables
      no_parsed = 0;
      x = 0;
      y = 0;
      rewind(fp);

      if(i == 0)
      {
         product = no_trees[i];
      }
      else
      {
         product *= no_trees[i];
      }
   }

   printf("The product is: %llu\n", product);

   fclose(fp);
   return 0;
}
