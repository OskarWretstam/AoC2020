#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOF_JOLTAGES_CONVERTERS 95 // Length with outlet and device included
#define MAX_ONE_BLOCK_LEN       5  // Max number of consecutive ones in my data


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

int ipow(int base, int exp)
{
    long long unsigned result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}


int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   unsigned int converters[NOF_JOLTAGES_CONVERTERS];

   // Input the joltage converters from our luggage
   for(int i = 1; i < NOF_JOLTAGES_CONVERTERS-1; i++)
   {
      fscanf(fp, "%u", &converters[i]);
   }
   fclose(fp);

   // Insert outlet
   converters[0] = 0;
   qsort(converters, NOF_JOLTAGES_CONVERTERS-1, sizeof(unsigned int), cmpfunc);
   // Insert device
   converters[NOF_JOLTAGES_CONVERTERS-1] = converters[NOF_JOLTAGES_CONVERTERS-2]+3;


   // Just count one and three steps
   int nof_1_jolt = 0, nof_3_jolt = 0;

   // Here is the fun stuff, count blocks of consecutive one steps
   int one_block_hash[MAX_ONE_BLOCK_LEN];
   memset(one_block_hash, 0, MAX_ONE_BLOCK_LEN*sizeof(int));
   int consec_one_steps = 0;

   for(int i = 0; i < NOF_JOLTAGES_CONVERTERS-1; i++)
   {
      if(converters[i+1]-converters[i] == 1)
      {
         nof_1_jolt++;
         consec_one_steps++;
      }
      else if(consec_one_steps > 0)
      {
         one_block_hash[consec_one_steps]++;
         consec_one_steps = 0;
      }

      if(converters[i+1]-converters[i] == 3)
      {
         nof_3_jolt++;
      }
   }

   printf("Task 1: 1-jolt: %d, 3-jolt: %d, multiplied: %d\n", nof_1_jolt, nof_3_jolt, nof_1_jolt*nof_3_jolt);

   // one_block_hash[1] = nof blocks with two consecutive ones
   // one_block_hash[2] = nof blocks with three consecutive ones

   // The base is trick, don't know a mathematical expression for it
   // Consecutive ones -> Variations to walk through
   //               2  ->  1
   //               3  ->  2
   //               4  ->  4
   //               5  ->  7
   // Skip the first element to align with one_block_hash
   int base[MAX_ONE_BLOCK_LEN] = {0, 1, 2, 4, 7};

   // Example:                1,2,3,6,7,8,9,12,13,14,17,18
   // One step block length:    3      4        3      2
   // Block variations:         2      4        2      1
   // Variations:               2*4*2*1 = 16

   long long unsigned nof_variations = 1;
   for(int i = 1; i < MAX_ONE_BLOCK_LEN; i++)
   {
      // Base is nof variations, exponent is nof occurrences
      nof_variations *= ipow(base[i], one_block_hash[i]);
   }

   printf("Total number of variations: %llu\n\n", nof_variations);

}
