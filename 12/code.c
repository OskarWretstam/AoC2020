#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <complex.h>

#define MOVE_EAST  (1 + 0*I)
// (Village people)
#define GO_WEST    (-1 + 0*I)
#define MOVE_NORTH (0 + 1*I)
#define MOVE_SOUTH (0 - 1*I)
#define ROL           1*I
#define ROR           -1*I
#define MAX_DATA_SIZE 4

int main(void)
{
   int complex first_ship_pos = 0 + 0*I;
   int complex first_ship_dir = 1 + 0*I;
   int complex second_ship_pos = 0 + 0*I;
   int complex second_ship_vel = 10 + 1*I;

   // THe usual file parsing variables
   FILE *fp = fopen("data.txt", "r");
   char c, instr;

   // For storing of the value associated with a direction
   char data[MAX_DATA_SIZE];
   int nof;

   // Helper for rotations
   int rot_deg;

   while(1)
   {
      instr = fgetc(fp);
      if(instr == EOF)
      {
         // Done done done
         break;
      }

      // Reset data string buffer for the next loop
      nof = 0;
      memset(data, '\0', MAX_DATA_SIZE);

      // Fill the data string buffer
      c = fgetc(fp);
      while(c != '\n')
      {
         data[nof++] = c;
         c = fgetc(fp);
      }

      switch(instr)
      {
         case 'E':

            // First ship
            first_ship_pos += atoi(data)*MOVE_EAST;
            // Second ship
            second_ship_vel += atoi(data)*MOVE_EAST;
            break;

         case 'S':
            // First ship
            first_ship_pos += atoi(data)*MOVE_SOUTH;
            // Second ship
            second_ship_vel += atoi(data)*MOVE_SOUTH;
            break;

         case 'W':
            // First ship
            first_ship_pos += atoi(data)*GO_WEST;
            // Second ship
            second_ship_vel += atoi(data)*GO_WEST;
            break;

         case 'N':
            // First ship
            first_ship_pos += atoi(data)*MOVE_NORTH;
            // Second ship
            second_ship_vel += atoi(data)*MOVE_NORTH;
            break;

         case 'L':
            // First ship
            rot_deg = atoi(data);
            // Perform a number of 90 deg turns to the left
            while(rot_deg > 0)
            {
               rot_deg -= 90;
               // First ship
               first_ship_dir *= ROL;
               // Second ship
               second_ship_vel *= ROL;
            }
            break;

         case 'R':
            // First ship
            rot_deg = atoi(data);
            // Perform a number of 90 deg turns to the right
            while(rot_deg > 0)
            {
               rot_deg -= 90;
               // First ship
               first_ship_dir *= ROR;
               // Second ship
               second_ship_vel *= ROR;
            }
            break;

         case 'F':
            // First ship
            first_ship_pos += atoi(data)*first_ship_dir;
            // Second ship
            second_ship_pos += atoi(data)*second_ship_vel;
            break;

         default:
            printf("Incorrect instruction: %c read\n", instr);
            return 1;
      }
   }

   printf("First ship position: %d, second ship: %d\n",
          abs(creal(first_ship_pos)) + abs(cimag(first_ship_pos)),
          abs(creal(second_ship_pos)) + abs(cimag(second_ship_pos)));

   return 0;
}
