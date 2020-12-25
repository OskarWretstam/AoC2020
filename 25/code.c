#include <stdio.h>

#define CARD_PUBLIC_KEY 12232269
// #define CARD_PUBLIC_KEY 5764801
#define DOOR_PUBLIC_KEY 19452773
// #define DOOR_PUBLIC_KEY 17807724
#define SUBJECT_NUMBER  7
#define MODULU_CONSTANT 20201227

int main(int argc, char *argv[])
{
   int found_card_loop_nmbr = 0;
   int found_door_loop_nmbr = 0;

   int loop_size = 0;
   unsigned long long val = 1;
   while(1)
   {
      val = val*SUBJECT_NUMBER;
      val = val%MODULU_CONSTANT;
      loop_size++;

      if(val == CARD_PUBLIC_KEY)
      {
         printf("Card loop size: %d\n", loop_size);
         found_card_loop_nmbr = loop_size;
         if(found_door_loop_nmbr > 0)
         {
            break;
         }
      }

      if(val == DOOR_PUBLIC_KEY)
      {
         printf("Door loop size: %d\n", loop_size);
         found_door_loop_nmbr = loop_size;
         if(found_card_loop_nmbr > 0)
         {
            break;
         }
      }
   }

   loop_size = 0;
   val = 1;
   int subject_number = DOOR_PUBLIC_KEY;
   while(loop_size < found_card_loop_nmbr)
   {
      val = val*subject_number;
      val = val%MODULU_CONSTANT;
      loop_size++;
   }
   printf("Encryption key from card is: %llu\n", val);

   loop_size = 0;
   val = 1;
   subject_number = CARD_PUBLIC_KEY;
   while(loop_size < found_door_loop_nmbr)
   {
      val = val*subject_number;
      val = val%MODULU_CONSTANT;
      loop_size++;
   }
   printf("Encryption key from door is: %llu\n", val);

   return 0;
}
