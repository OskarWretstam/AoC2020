#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STR_BUFFER_SIZE 30

typedef enum
{
   INVALID       = 0,
   VALID_KEY     = 1,
   VALID_KEY_VAL = 2,
   VALID_DUMMY   = 0xFFFFFFF
} valid_t;

valid_t check_key_val(FILE* fp, char start_char)
{
   char key_buffer[STR_BUFFER_SIZE];
   char val_buffer[STR_BUFFER_SIZE];
   char c;
   int key_chars = 0, val_chars = 0;
   memset(val_buffer, '\0', STR_BUFFER_SIZE);
   memset(key_buffer, '\0', STR_BUFFER_SIZE);

   // Put the char that got us here
   key_buffer[key_chars++] = start_char;

   // Finish reading the key
   while(1)
   {
      c = fgetc(fp);
      if(c == ':')
      {
         break;
      }
      key_buffer[key_chars++] = c;
   }

   // Read the value
   while(1)
   {
      c = fgetc(fp);
      if(c == ' ' || c == '\n')
      {
         break;
      }
      val_buffer[val_chars++] = c;
   }

   int str_len, check_sum;
   // Check results
   if(strcmp(key_buffer, "cid") == 0)
   {
      // This ignored, to speed up :)
      return INVALID;
   }
   if(strcmp(key_buffer, "byr") == 0)
   {
      if(atoi(val_buffer) >= 1920 && atoi(val_buffer) <= 2002)
      {
         return VALID_KEY_VAL;
      }

      return VALID_KEY;
   }
   else if(strcmp(key_buffer, "iyr") == 0)
   {
      if(atoi(val_buffer) >= 2010 && atoi(val_buffer) <= 2020)
      {
         return VALID_KEY_VAL;
      }

      return VALID_KEY;
   }
   else if(strcmp(key_buffer, "eyr") == 0)
   {
      if(atoi(val_buffer) >= 2020 && atoi(val_buffer) <= 2030)
      {
         return VALID_KEY_VAL;
      }

      return VALID_KEY;
   }
   else if(strcmp(key_buffer, "hgt") == 0)
   {
      str_len = strlen(val_buffer);
      if(strcmp(&val_buffer[str_len-2], "cm") == 0)
      {
         val_buffer[str_len-2] = '\0';
         if(atoi(val_buffer) >= 150 && atoi(val_buffer) <= 193)
         {
            return VALID_KEY_VAL;
         }
      }
      else if(strcmp(&val_buffer[str_len-2], "in") == 0)
      {
         val_buffer[str_len-2] = '\0';
         if(atoi(val_buffer) >= 59 && atoi(val_buffer) <= 76)
         {
            return VALID_KEY_VAL;
         }
      }

      return VALID_KEY;
   }
   else if(strcmp(key_buffer, "hcl") == 0)
   {
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
            return VALID_KEY_VAL;
         }
      }

      return VALID_KEY;
   }
   else if(strcmp(key_buffer, "ecl") == 0)
   {
      if(strcmp(val_buffer, "amb") == 0 ||
         strcmp(val_buffer, "blu") == 0 ||
         strcmp(val_buffer, "brn") == 0 ||
         strcmp(val_buffer, "gry") == 0 ||
         strcmp(val_buffer, "grn") == 0 ||
         strcmp(val_buffer, "hzl") == 0 ||
         strcmp(val_buffer, "oth") == 0)
      {
         return VALID_KEY_VAL;
      }

      return VALID_KEY;
   }
   else if(strcmp(key_buffer, "pid") == 0)
   {
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
            return VALID_KEY_VAL;
         }
      }

      return VALID_KEY;
   }

   return INVALID;
}

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   if(fp == NULL)
   {
      printf("Failed to open file for reading.\n");
      return 1;
   }

   char c;

   // Intra passport running numbers
   int nof_passports = 0, nof_valid_wrt_key = 0, nof_valid_wrt_key_val = 0;
   // Inter passport running numbers
   int nof_valid_keys = 0, nof_valid_values = 0;

   while(1)
   {
      c = fgetc(fp);
      if(c == '\n' || c == EOF)
      {
         // We finished testing a passport check what we have so far
         nof_passports++;
         if(nof_valid_keys == 7)
         {
            nof_valid_wrt_key++;
            if(nof_valid_values == 7)
            {
               nof_valid_wrt_key_val++;
            }
         }

         if( c == EOF )
         {
            // File is done
            break;
         }

         // Reset inter passport numbers
         nof_valid_values = 0;
         nof_valid_keys = 0;
      }
      else
      {
         // A key value pair is stared, do some work in the function
         valid_t valid = check_key_val(fp, c);
         if(valid == VALID_KEY_VAL)
         {
            nof_valid_keys++;
            nof_valid_values++;
         }
         else if(valid == VALID_KEY)
         {
            nof_valid_keys++;
         }
      }

   }

   printf("Parsed: %d out of which: %d was valid wrt. key and: %d wrt. key & val\n", nof_passports, nof_valid_wrt_key, nof_valid_wrt_key_val);

   fclose(fp);
}
