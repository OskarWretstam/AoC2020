#include <stdio.h>
#include <stdlib.h>

#define NOF_ENTRIES 200
#define THIS_YEAR 2020

int main(void)
{
   // The data we want to have a look at
   FILE *fp = fopen("data.txt", "r");
   // Take a stab of how many number we have to allocate for
   int *arr = (int *) calloc(NOF_ENTRIES, sizeof(int));
   if(fp == NULL || arr == NULL)
   {
      fclose(fp);
      free(arr);
      printf("Failed at initailisation, bad luck.\n");
      return 1;
   }

   int nof_read = 0;
   while(nof_read < NOF_ENTRIES && !feof(fp) && fscanf(fp, "%d\n", &arr[nof_read++]) == 1){;}

   if(nof_read > NOF_ENTRIES)
   {
      free(arr);
      fclose(fp);
      printf("Too many entries in file, increase allocated size or be smarter.\n");
      return 1;
   }

   printf("Read: %d entries from the file.\n", nof_read);

   int index1 = 0, index2 = 0;
   int index3 = 0, index4 = 0, index5 = 0;
   int found_pair = 0, found_triple = 0;
   for(int i = 0; i < nof_read; i++)
   {
      for(int j = i+1; j < nof_read; j++)
      {
         if(arr[i] + arr[j] == THIS_YEAR)
         {
            index1 = i;
            index2 = j;
            found_pair = 1;
            if(found_triple)
            {
               i = j = nof_read;
               break;
            }
         }
         for(int k = j+1; k < nof_read; k++)
         {
            if(arr[i] + arr[j] + arr[k] == THIS_YEAR)
            {
               index3 = i;
               index4 = j;
               index5 = k;
               found_triple = 1;
               if(found_pair == 1)
               {
                  i = j = k = nof_read;
               }
            }
         }
      }
   }

   if(index1 == 0 || index2 == 0)
   {
      free(arr);
      fclose(fp);
      printf("Failed to find two numbers that sum up to %d\n", THIS_YEAR);
      return 1;
   }

   if(index3 == 0 || index4 == 0 || index5 == 0)
   {
      free(arr);
      fclose(fp);
      printf("Failed to find three numbers that sum up to %d\n", THIS_YEAR);
      return 1;
   }

   printf("The product of the pair is: %d\n", arr[index1]*arr[index2]);
   printf("The product of the triple is: %d\n", arr[index3]*arr[index4]*arr[index5]);

   free(arr);
   fclose(fp);
}
