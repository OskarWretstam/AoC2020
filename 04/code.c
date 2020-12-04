#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STR_BUFFER_SIZE 30

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   if(fp == NULL)
   {
      printf("Failed to open file for reading.\n");
      return 1;
   }

   char c;
   char id_buffer[STR_BUFFER_SIZE];
   char val_buffer[STR_BUFFER_SIZE];
   int key_chars = 0, val_chars = 0;
   int ctrl_char = 0;
   int prev_nl = 0, keyval_done = 0, key_done = 0;
   int nof_pass = 0, nof_ok1 = 0, nof_ok2 = 0;
   int valid_ids = 0, valid_keyval = 0;
   int check_sum;
   int str_len;
   memset(val_buffer, '\0', STR_BUFFER_SIZE);
   memset(id_buffer, '\0', STR_BUFFER_SIZE);

   while(1)
   {
      ctrl_char = 0;
      c = fgetc(fp);
      if(c == '\n' || c == EOF)
      {
         if(prev_nl == 1 || c == EOF)
         {
            nof_pass++;
            if(valid_ids == 7)
            {
               nof_ok1++;
            }
            if(valid_keyval == 7)
            {
               nof_ok2++;
            }
            valid_ids = 0;
            valid_keyval = 0;

            if(c == EOF)
            {
               break;
            }
         }

         prev_nl = 1;
         keyval_done = 1;
         ctrl_char = 1;
      }
      else if(c == ':')
      {
         key_done = 1;
         ctrl_char = 1;
         prev_nl = 0;
      }
      else if(c == ' ')
      {
         keyval_done = 1;
         ctrl_char = 1;
         prev_nl = 0;
      }

      if(keyval_done == 1)
      {
         if(strcmp(id_buffer, "byr") == 0)
         {
            valid_ids++;
            if(atoi(val_buffer) >= 1920 && atoi(val_buffer) <= 2002)
            {
               valid_keyval++;
            }
         }
         else if(strcmp(id_buffer, "iyr") == 0)
         {
            valid_ids++;
            if(atoi(val_buffer) >= 2010 && atoi(val_buffer) <= 2020)
            {
               valid_keyval++;
            }
         }
         else if(strcmp(id_buffer, "eyr") == 0)
         {
            valid_ids++;
            if(atoi(val_buffer) >= 2020 && atoi(val_buffer) <= 2030)
            {
               valid_keyval++;
            }
         }
         else if(strcmp(id_buffer, "hgt") == 0)
         {
            valid_ids++;
            str_len = strlen(val_buffer);
            if(strcmp(&val_buffer[str_len-2], "cm") == 0)
            {
               val_buffer[str_len-2] = '\0';
               if(atoi(val_buffer) >= 150 && atoi(val_buffer) <= 193)
               {
                  valid_keyval++;
               }
            }
            else if(strcmp(&val_buffer[str_len-2], "in") == 0)
            {
               val_buffer[str_len-2] = '\0';
               if(atoi(val_buffer) >= 59 && atoi(val_buffer) <= 76)
               {
                  valid_keyval++;
               }
            }
         }
         else if(strcmp(id_buffer, "hcl") == 0)
         {
            valid_ids++;
            if(val_buffer[0] == '#' && strlen(val_buffer) == 7)
            {
               check_sum = 0;
               for(int i = 1; i < 7; i++)
               {
                  if(isxdigit(val_buffer[i]) == 0)
                  {
                     check_sum++;
                  }
               }
               if(check_sum == 0)
               {
                  valid_keyval++;
               }
            }
         }
         else if(strcmp(id_buffer, "ecl") == 0)
         {
            valid_ids++;
            if(strcmp(val_buffer, "amb") == 0 ||
               strcmp(val_buffer, "blu") == 0 ||
               strcmp(val_buffer, "brn") == 0 ||
               strcmp(val_buffer, "gry") == 0 ||
               strcmp(val_buffer, "grn") == 0 ||
               strcmp(val_buffer, "hzl") == 0 ||
               strcmp(val_buffer, "oth") == 0)
            {
               valid_keyval++;
            }
         }
         else if(strcmp(id_buffer, "pid") == 0)
         {
            valid_ids++;
            if(strlen(val_buffer) == 9)
            {
               check_sum = 0;
               for(int i = 0; i < 9; i++)
               {
                  if(isdigit(val_buffer[i]) == 0)
                  {
                     check_sum++;
                  }
               }
               if(check_sum == 0)
               {
                  valid_keyval++;
               }
            }
         }

         keyval_done = 0;
         key_done = 0;
         memset(val_buffer, '\0', STR_BUFFER_SIZE);
         memset(id_buffer, '\0', STR_BUFFER_SIZE);
         val_chars = 0;
         key_chars = 0;
      }
      else if(key_done == 1 && ctrl_char == 0)
      {
         val_buffer[val_chars++] = c;
         prev_nl = 0;
      }
      else if(ctrl_char == 0)
      {
         id_buffer[key_chars++] = c;
         prev_nl = 0;
      }
   }

   printf("Parsed: %d out of which: %d was valid wrt. key and: %d wrt. key & val\n", nof_pass, nof_ok1, nof_ok2);

   fclose(fp);
}
