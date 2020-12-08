#include <stdio.h>
#include <string.h>

#define NOP "nop"
#define ACC "acc"
#define JMP "jmp"

#define INSTR_LEN 3
// Hm, how to "remember" executed lines in a fancier way?
#define NOF_MEM_LINE 300
// Europe
#define THE_FINAL_LINE_COUNT 626

// Well it said in the task it should be global, so lets
static int accumulator = 0;

// Some ways to exit from the code
typedef enum
{
   MEM_FUL = 0,    // My crappy line memory is full
   REPEAT  = 1,    // The same line is executed again
   DONE    = 3     // The final line was executed
} exit_status_t;

int test_execute(
   FILE *fp,
   int to_change // <1 to not alter instructions
   )
{
   char instr[INSTR_LEN+1];
   int  data;
   // Always execute from beginning of the file
   int line_nmbr = 0;
   // Where are we heading
   int target_line = 1;

   // The crappy memory that was mentioned ealier
   int nof_executed_lines = 0;
   int line_mem[NOF_MEM_LINE];
   memset(line_mem, 0, NOF_MEM_LINE*sizeof(int));

   // nof_nop_jmp = to change, time to change
   int nof_nop_jmp = 0;

   while(1)
   {
      fscanf(fp, "%s %d\n", instr, &data);
      line_nmbr++;
      if(line_nmbr == THE_FINAL_LINE_COUNT+1)
      {
         // We got there in the end
         printf("Returning DONE after %d instructions executed\n", nof_executed_lines);
         return DONE;
      }

      // Skip a bunch of lines in case of a jmp
      if(line_nmbr == target_line)
      {
         // I have a memory of this place?
         for(int i = 0; i < nof_executed_lines; i++)
         {
            if(line_nmbr == line_mem[i])
            {
               // Task 1: nice, Task 2: crap
               printf("Returning REPEAT after %d instructions executed\n", nof_executed_lines);
               return REPEAT;
            }
         }

         if(nof_executed_lines == NOF_MEM_LINE-1)
         {
            // I hope we don't get here
            return MEM_FUL;
         }
         // Remember remeber the eight of december
         line_mem[nof_executed_lines++] = line_nmbr;


         if(strcmp(instr, NOP) == 0)
         {
            // Increment nof nop and jmps encounterd
            nof_nop_jmp++;
            if(nof_nop_jmp == to_change)
            {
               printf("Replacing, NOP->JMP at line %d after %d instructions executed\n", line_nmbr, nof_executed_lines);
               // Time for change, behave as a jmp
               target_line += data;
               if(line_nmbr > target_line)
               {
                  // Does not really make sense to rewind to the top.
                  // On the other hand it is simple
                  rewind(fp);
                  line_nmbr = 0;
               }
            }
            else
            {
               // Just a regular nop
               target_line++;
            }
         }
         else if(strcmp(instr, ACC) == 0)
         {
            // Well
            target_line++;
            accumulator += data;
         }
         else if(strcmp(instr, JMP) == 0)
         {
            // Again, nof nops and jmps increased
            nof_nop_jmp++;
            if(nof_nop_jmp == to_change)
            {
               printf("Replacing, JMP->NOP at line %d after %d instructions executed\n", line_nmbr, nof_executed_lines);
               // Change into the incredible nop
               target_line++;
            }
            else
            {
               // Ah, might as well jmp (might as well jmp)
               target_line += data;
               if(line_nmbr > target_line)
               {
                  rewind(fp);
                  line_nmbr = 0;
               }

            }
         }
      }
   }
}

int main(void)
{
   FILE *fp = fopen("data.txt", "r");
   if(fp == NULL)
   {
      printf("Failed to open file for reading.\n");
      return 1;
   }

   // Task 1;
   if(test_execute(fp, 0) == MEM_FUL)
   {
      printf("Crappy memory ful, increase\n");
      fclose(fp);
      return 1;
   }

   // Did not check for DONE but task says it wont occur
   printf("Task 1, accumulator is: %d\n", accumulator);

   // Reset for Task 2;
   rewind(fp);
   accumulator = 0;

   // Run the program to REPEAT while changing one jmp || nop at the time
   int to_change = 1;
   while(1)
   {
      exit_status_t stat = test_execute(fp, to_change);
      if(stat == MEM_FUL)
      {
         printf("Crappy memory ful, increase\n");
         fclose(fp);
         return 1;
      }
      else if(stat == DONE)
      {
         // Program executed to completion
         break;
      }

      // Returned REPEAT, try again by replacing the next nop or jmp
      // Remeber start from scratch every time
      to_change++;
      rewind(fp);
      accumulator = 0;
   }

   printf("Task 2, accumulator is: %d\n", accumulator);

   fclose(fp);
   return 0;
}
