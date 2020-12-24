#include <stdio.h>
#include <string.h>

#define MAX_NOF_COORDS 5000
#define LINE_LEN 50
#define GIVE_UP 100
#define NOF_WORLDS 2

typedef enum
{
   WHITE_SIDE = 0,
   BLACK_SIDE = 1,
   SIDE_DUMMY = 0xFFFFFFFF
} side_up_t;

// A coordinate in hexagonal grid
typedef struct
{
   side_up_t side;
   int x;
   int y;
   int z;
   int dist;
} hexa_coord_t;

typedef struct
{
   hexa_coord_t tile[MAX_NOF_COORDS];
} world_t;

int is_new_tile(hexa_coord_t *new, hexa_coord_t *list, int n)
{
   // Calculate distance for this new tile
   new->dist = new->x*new->x+new->y*new->y+new->z*new->z;

   for(int i = 0; i < n; i++)
   {
      // Check first for coordinate at the same distance
      if(new->dist == list[i].dist)
      {
         // Is it a perfect match?
         if(new->x == list[i].x &&
            new->y == list[i].y &&
            new->z == list[i].z)
         {
            return i;
         }
      }
   }

   return -1;
}

void span_world(world_t *w, int *xn, int *xx, int *yn, int *yx, int *zn, int *zx, int nof_used_tiles)
{
   // Set to something outside of this world
   *xn = 50; *xx = -50; *yn = 50; *yx = -50; *zn = 50; *zx = -50;

   for(int i = 0; i < nof_used_tiles; i++)
   {
      // x
      if(w->tile[i].x < *xn)
      {
         *xn = w->tile[i].x;
      }
      if(w->tile[i].x > *xx)
      {
         *xx = w->tile[i].x;
      }

      // y
      if(w->tile[i].y < *yn)
      {
         *yn = w->tile[i].y;
      }
      if(w->tile[i].y > *yx)
      {
         *yx = w->tile[i].y;
      }

      // z
      if(w->tile[i].z < *zn)
      {
         *zn = w->tile[i].z;
      }
      if(w->tile[i].z > *zx)
      {
         *zx = w->tile[i].z;
      }
   }
}

// Hehe
int valid_point(world_t *w, int x, int y, int z, int nof_tiles)
{
   for(int i = 0; i < nof_tiles; i++)
   {
      if(w->tile[i].x == x &&
         w->tile[i].y == y &&
         w->tile[i].z == z)
      {
         return 1;
      }
   }
   return 0;
}

int cnt_neighbors(world_t *w, int x, int y, int z, int nof_tiles)
{
   int cnt = 0;

   // First do our plane
   cnt += valid_point(w, x-1, y, z+1, nof_tiles);
   cnt += valid_point(w, x, y+1, z+1, nof_tiles);
   cnt += valid_point(w, x+1, y+1, z, nof_tiles);
   cnt += valid_point(w, x+1, y, z-1, nof_tiles);
   cnt += valid_point(w, x, y-1, z-1, nof_tiles);
   cnt += valid_point(w, x-1, y-1, z, nof_tiles);

   return cnt;
}

int new_tile(world_t *w, int x, int y, int z, int *nof_tiles)
{
   w->tile[*nof_tiles].x = x;
   w->tile[*nof_tiles].y = y;
   w->tile[*nof_tiles].z = z;
   *nof_tiles = *nof_tiles + 1;
}

int main(int argc, char* argv[])
{
   // The tile stuff
   hexa_coord_t tiles[MAX_NOF_COORDS];
   memset(tiles, 0, MAX_NOF_COORDS*sizeof(hexa_coord_t));
   int nof_unique_tiles = 0;

   // The file stuff
   char line[LINE_LEN];
   char *s_ptr;
   int line_pos;

   // tmp while parsing a line
   hexa_coord_t tmp;

   int is_new;

   for(s_ptr = fgets(line, LINE_LEN, stdin);
       (s_ptr != NULL && line[0] != '\n');
       s_ptr = fgets(line, LINE_LEN, stdin))
   {
      // Reset some stuff
      memset(&tmp, 0, sizeof(hexa_coord_t));
      line_pos = 0;

      while(line[line_pos] != '\n')
      {
         // First check the potential dual-char dirs
         if(line[line_pos] == 's')
         {
            if(line[line_pos+1] == 'w')
            {
               tmp.y--;
               tmp.z--;
            }
            else if(line[line_pos+1] == 'e')
            {
               tmp.x++;
               tmp.z--;
            }
            else
            {
               printf("Unrecognized chracter after s\n");
               return 1;
            }
            line_pos+=2;
         }
         else if(line[line_pos] == 'n')
         {
            if(line[line_pos+1] == 'w')
            {
               tmp.x--;
               tmp.z++;
            }
            else if(line[line_pos+1] == 'e')
            {
               tmp.y++;
               tmp.z++;
            }
            else
            {
               printf("Unrecognized chracter after n\n");
               return 1;
            }
            line_pos+=2;
         }
         else if(line[line_pos] == 'w')
         {
            tmp.x--;
            tmp.y--;
            line_pos++;
         }
         else if(line[line_pos] == 'e')
         {
            tmp.x++;
            tmp.y++;
            line_pos++;
         }
         else
         {
            printf("Read an unrecognized character.\n");
            return 1;
         }
      }

      // Returns -1 if new else index to flip
      is_new = is_new_tile(&tmp, tiles, nof_unique_tiles);

      if(is_new == -1)
      {
         // Add the tile
         memcpy(&tiles[nof_unique_tiles], &tmp, sizeof(hexa_coord_t));
         // Flip it
         tiles[nof_unique_tiles].side = !tiles[nof_unique_tiles].side;
         nof_unique_tiles++;
         if(nof_unique_tiles == MAX_NOF_COORDS)
         {
            printf("Too many tiles stored.\n");
            return 1;
         }
      }
      else
      {
         tiles[is_new].side = !tiles[is_new].side;
      }
   }

   // Count the added tiles, both colors
   int nof_black = 0, nof_white = 0;
   for(int i = 0; i < nof_unique_tiles; i++)
   {
      if(tiles[i].side == WHITE_SIDE)
      {
         nof_white++;
      }
      else if(tiles[i].side == BLACK_SIDE)
      {
         nof_black++;
      }
      else
      {
         printf("Unrecognized side on tile\n");
         return 1;
      }
   }
   printf("Task1: %d\n", nof_black);

   // Think we are supposed to continue with the current state into task 2
   world_t world[2];
   int nof_copied = 0;
   // Move black tiles only into the world structure from day 17 part 1
   // that tested for neighbors in three dimensions
   for(int i = 0; i < nof_unique_tiles; i++)
   {
      if(tiles[i].side == BLACK_SIDE)
      {
         memcpy(&world[0].tile[nof_copied], &tiles[i], sizeof(hexa_coord_t));
         nof_copied++;
      }
   }

   // This is just day 17 part 1
   int Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
   int nof_cycles = 0;
   int current_world_ix;
   int cnt;
   int nof_used_tiles[NOF_WORLDS] = {0, 0};
   nof_used_tiles[0] = nof_black;

   while(nof_cycles < GIVE_UP)
   {
      current_world_ix = nof_cycles%NOF_WORLDS;
      // Reset the other world
      memset(&world[!current_world_ix], 0, sizeof(world_t));
      nof_used_tiles[!current_world_ix] = 0;

      // Span this world
      span_world(&world[current_world_ix], &Xmin, &Xmax, &Ymin, &Ymax, &Zmin, &Zmax, nof_used_tiles[current_world_ix]);
      // Cycle update
      nof_cycles++;

      // Loop over entire world + 1 additional in every direction to check for new spawns
      for(int x = Xmin-1; x <= Xmax+1; x++)
      {
         for(int y = Ymin-1; y <= Ymax+1; y++)
         {
            for(int z = Zmin-1; z <= Zmax+1; z++)
            {
               // Count black tile neighbors for every tile in world span
               cnt = cnt_neighbors(&world[current_world_ix],x,y,z, nof_used_tiles[current_world_ix]);

               if(valid_point(&world[current_world_ix], x, y, z, nof_used_tiles[current_world_ix]))
               {
                  // Tile is black
                  if(cnt > 0 && cnt <= 2)
                  {
                     // Tile should still be black
                     new_tile(&world[!current_world_ix], x, y, z, &nof_used_tiles[!current_world_ix]);
                  }
               }
               else if(cnt == 2)
               {
                  // White tile fliped
                  new_tile(&world[!current_world_ix], x, y, z, &nof_used_tiles[!current_world_ix]);
               }
            }
         }
      }
   }
   printf("Task2: %d\n", nof_used_tiles[!current_world_ix]);

   return 0;
}
