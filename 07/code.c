#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NOF_RULES 594
// #define NOF_RULES 7
#define BAG_DESCR_LEN 50
#define MAX_NOF_SUB_BAGS 10

typedef struct bags_done_t bags_done_t;

struct bags_done_t
{
   char bag[BAG_DESCR_LEN];
   int  contains_shiny_gold;
   int  nof_sub_bags;
   int  sub_bag_cnt;
   char subbag[BAG_DESCR_LEN*MAX_NOF_SUB_BAGS];
};

static bags_done_t done[NOF_RULES];
static int nof_tested = 0;

int test_contain(bags_done_t bag);
int count_subbags(bags_done_t bag);

int get_bag(FILE *fp, char *str, int *len)
{
   int c, read = 0;
   *len = 0;
   while(1)
   {
      c = fgetc(fp);
      if(c == ':' || c == ',' || c == '\n')
      {
         break;
      }
      if(c != ' ')
      {
         str[read++] = c;
      }
      if(isdigit(c))
      {
         *len = c - '0';
      }
   }

   if(str[read-1] != 's')
   {
      str[read++] = 's';
   }
   if(c == '\n')
   {
      return 1;
   }
   return 0;
}

int main(void)
{
   FILE* fp = fopen("data.txt", "r");
   if(fp == NULL)
   {
      printf("Failed to open file for reading.\n");
      return 1;
   }

   int tmp;
   int len;
   for(int i = 0; i < NOF_RULES; i++)
   {
      get_bag(fp, done[i].bag, &len);
      done[i].sub_bag_cnt += len;
      printf("%s\n", done[i].bag);

      while(1)
      {
         tmp = get_bag(fp, &done[i].subbag[done[i].nof_sub_bags*BAG_DESCR_LEN], &len);
         done[i].sub_bag_cnt += len;
         if(strcmp(&done[i].subbag[done[i].nof_sub_bags*BAG_DESCR_LEN], "shinygoldbag") == 0 || strcmp(&done[i].subbag[done[i].nof_sub_bags*BAG_DESCR_LEN], "shinygoldbags") == 0)
         {
            done[i].contains_shiny_gold = 1;
         }
         if(strcmp(&done[i].subbag[done[i].nof_sub_bags*BAG_DESCR_LEN], "nootherbags") != 0)
         {
            done[i].nof_sub_bags++;
         }
         if(tmp == 1)
         {
            break;
         }
      }
   }

   for(int i = 0; i < NOF_RULES; i++)
   {
      printf("%s %d %d %d ", done[i].bag, done[i].contains_shiny_gold, done[i].nof_sub_bags, done[i].sub_bag_cnt);
      for(int j = 0; j <= done[i].nof_sub_bags; j++)
      {
         printf("%s ", &done[i].subbag[j*BAG_DESCR_LEN]);
      }
      printf("\n");
   }

   // int nof = 0;
   // for(int i = 0; i < NOF_RULES; i++)
   // {
   //    // printf("testing %s\n", done[i].bag);
   //    nof += test_contain(done[i]);
   // }


   int nof2 = 0;
   for(int i = 0; i < NOF_RULES; i++)
   {
      if(strcmp(done[i].bag, "shinygoldbags") == 0)
      {
         printf("testing %s\n", done[i].bag);
         nof2 = count_subbags(done[i])-1;
         break;
      }
   }

   // printf("%d\n", nof);
   printf("%d\n", nof2);
   printf("Part 1 ctrl: %d Part 2 ctrl: %d\n", 274, 158730);

}

int count_subbags(bags_done_t bag)
{
   int sub_bag_cnt = 1;
   if(bag.nof_sub_bags == 0)
   {
      printf("no subbags, return 1\n");
      return 1;
   }
   else
   {
      for(int i = 0; i <= bag.nof_sub_bags; i++)
      {
         // printf("looking for sub bag: %s\n", &bag.subbag[i*BAG_DESCR_LEN]);
         for(int j = 0; j < NOF_RULES; j++)
         {
            if(strcmp(&bag.subbag[i*BAG_DESCR_LEN]+1, done[j].bag) == 0)
            {
               printf("subbag test %s\n", done[j].bag);
               printf("%d\n", sub_bag_cnt);
               sub_bag_cnt += (bag.subbag[i*BAG_DESCR_LEN]-'0')+count_subbags(done[j]);
            }
         }
      }
      printf("returning %d\n", sub_bag_cnt);
      return sub_bag_cnt;
   }
}

int test_contain(bags_done_t bag)
{
   if(bag.contains_shiny_gold == 1)
   {
      // printf("contains\n");
      return 1;
   }
   else if(bag.nof_sub_bags == 0)
   {
      // printf("does not contain, no sub bags\n");
      return 0;
   }
   else
   {
      for(int i = 0; i <= bag.nof_sub_bags; i++)
      {
         // printf("looking for sub bag: %s\n", &bag.subbag[i*BAG_DESCR_LEN]);
         for(int j = 0; j < NOF_RULES; j++)
         {
            if(strcmp(&bag.subbag[i*BAG_DESCR_LEN], done[j].bag) == 0)
            {
               // printf("testing sub bag: %s\n", done[j].bag);
               if(test_contain(done[j]) == 1)
               {
                  return 1;
               }
            }
         }
      }
   }

   // printf("does not contain, sub bags does not contain\n");
   return 0;
}
