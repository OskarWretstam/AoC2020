#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PW_LENGTH 200

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   if(fp == NULL)
   {
      printf("Failed to open data file.\n");
      return 1;
   }

   int no_read = 0, no_parsed = 0, no_valid = 0;
   int min, max;
   char the_char;
   int char_cnt;
   int passwd_len;
   char passwd[MAX_PW_LENGTH];
   int task = 2;

   while(!feof(fp) && fscanf(fp, "%d-%d %c: %100s\n", &min, &max, &the_char, passwd) == 4)
   {
      no_parsed++;
      passwd_len = strlen(passwd);
      if(task == 1)
      {
         char_cnt = 0;
         for(int i = 0; i < passwd_len; i++)
         {

            if(passwd[i] == the_char)
            {
               char_cnt++;
               if(char_cnt > max)
               {
                  break;
               }
            }
         }
         if(char_cnt <= max && char_cnt >= min)
         {
            no_valid++;
         }
      }
      else if(task == 2)
      {
         min--;
         max--;
         if(max < passwd_len)
         {
            if((passwd[min] == the_char) || (passwd[max] == the_char))
            {
               if(!((passwd[min] == the_char) && (passwd[max] == the_char)))
               {
                  no_valid++;
               }
            }
         }
      }
   }

   printf("Parsed: %d lines, Found: %d valid ones.\n", no_parsed, no_valid);
   return 0;
}
