#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LENGTH 12
#define NOF_BOARDING_PASS 869

int cmpfunc (const void * a, const void * b)
{
   if(*(unsigned int *)a < *(unsigned int*)b)
   {
      return -1;
   }
   else if(*(unsigned int *)a > *(unsigned int*)b)
   {
      return 1;
   }
   return 0;
}

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   int nof_boarding_pass = 0;
   char line[LINE_LENGTH];
   unsigned int id_arr[NOF_BOARDING_PASS];
   memset(id_arr, 0, NOF_BOARDING_PASS*sizeof(unsigned int));

   while(fgets(line, LINE_LENGTH, fp) != NULL)
   {
      // Do not include trailing newline in the loop
      for(int i = 0; i < LINE_LENGTH-2; i++)
      {
         if(line[i] == 'B' || line[i] == 'R')
         {
            // OR add ones at the appropriate positions
            id_arr[nof_boarding_pass] |= 1<<(LINE_LENGTH-3-i);
         }
      }
      nof_boarding_pass++;
   }

   // Sort ascending order
   qsort(id_arr, nof_boarding_pass, sizeof(unsigned int), cmpfunc);

   // Find the missing ID
   int j;
   for(j = 0; j<nof_boarding_pass-1; j++)
   {
      if(id_arr[j+1]-id_arr[j] != 1)
      {
         break;
      }
   }

   // Max value at last postion, missing value is after loop exit idx
   printf("The largest id: %u and the missing_no: %d\n", id_arr[nof_boarding_pass-1], id_arr[j]+1);
}
