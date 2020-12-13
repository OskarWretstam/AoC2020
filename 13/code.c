#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_LEN 20
#define NUMBER_OF_BUSES 9

int main(void)
{
   // Parsing files everyday..
   FILE *fp = fopen("data.txt", "r");
   char c;

   // Buffer for the numbers to be read
   char buffer[BUFFER_LEN];
   int chars_in_buffer = 0;
   memset(buffer, '\0', BUFFER_LEN);

   // Buses are defined by how often they arrive
   int buses[NUMBER_OF_BUSES];
   // Delay relative to the first bus in our timetable
   int delays[NUMBER_OF_BUSES];
   memset(delays, 0, NUMBER_OF_BUSES*sizeof(int));

   // Tracking variables
   int tot_delay = 0;
   int nof_buses = 0;

   // Our time of arrival
   int arrival_time;

   // Intialise arrival time......
   while(1)
   {
      c = fgetc(fp);
      if(c == '\n')
      {
         break;
      }
      buffer[chars_in_buffer++] = c;
   }
   // Got it
   arrival_time = atoi(buffer);

   // Reset our nice buffer
   chars_in_buffer = 0;
   memset(buffer, '\0', BUFFER_LEN);

   // Intialise buses and delays
   while(1)
   {
      c = fgetc(fp);

      if(c == ',' || c == EOF)
      {
         if(chars_in_buffer > 0)
         {
            // Found a , and read numers, a bus is done
            delays[nof_buses] = tot_delay;
            buses[nof_buses++] = atoi(buffer);
            chars_in_buffer = 0;
            memset(buffer, '\0', BUFFER_LEN);

            // Completing a bus means more delay
            tot_delay++;

            if(c == EOF)
            {
               // Bus was completed by EOF dne
               break;
            }
         }
      }
      else if(c == 'x')
      {
         // x marks more delay
         tot_delay++;
      }
      else
      {
         // Numbers!
         buffer[chars_in_buffer++] = c;
      }
   }

   int min_wait_id, min_wait_time = 100; // Unacceptable...
   float tmp;

   // Looking for our best departure
   for(int i = 0; i < NUMBER_OF_BUSES; i++)
   {
      // How many departures since the dawn of time?
      tmp = (float)(arrival_time)/buses[i];
      // Is the next pretty close?
      if(ceil(tmp)*buses[i]-arrival_time < min_wait_time)
      {
         min_wait_time = ceil(tmp)*buses[i]-arrival_time;
         min_wait_id = buses[i];
      }
   }

   printf("Task 1, Bus ID times wait time: %d\n", min_wait_id*min_wait_time);

   printf("Task 2, print this is wolframAplha, to lazy to implement the CRT solver....\n");
   for(int i = 0; i < NUMBER_OF_BUSES; i++)
   {
      printf("(t + %d) mod %d = 0, ", delays[i], buses[i]);
   }
   printf("\n\n");


   return 0;
}
