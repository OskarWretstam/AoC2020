#include <stdio.h>
#include <string.h>

#define NOF_FOODS 42
#define MAX_NOF_INGREDIENTS_IN_FOOD 200
#define MAX_NOF_ALLERGEN_IN_FOOD 5
#define LINE_LEN 600
#define KEY_LEN 15
#define ALERG_SEPARATOR "contains"
#define NOF_UNIQUE_ALERGS 50
#define MAX_INGRED_CANDIDATES 10

typedef struct
{
   char name[KEY_LEN];
} ingred_t;

typedef struct
{
   char name[KEY_LEN];
} alerg_t;

typedef struct
{
   char name[KEY_LEN];
   int  resolved;
   int  nof_candidates;
   int  candidate_ix[MAX_INGRED_CANDIDATES];
   ingred_t candidates[MAX_INGRED_CANDIDATES];
} alerg_match_t;

typedef struct
{
   int nof_ingred;
   ingred_t ingred[MAX_NOF_INGREDIENTS_IN_FOOD];
   int nof_alerg;
   alerg_t alerg[MAX_NOF_ALLERGEN_IN_FOOD];
} food_t;


void file_parse(food_t *f)
{
   // Line parsing stuff
   char line[LINE_LEN];
   char *s_ptr;
   int is_alergens;
   int nof_foods_init = 0;

   for(s_ptr = fgets(line, LINE_LEN, stdin); (s_ptr != NULL && line[0] != '\n'); s_ptr = fgets(line, LINE_LEN, stdin))
   {
      is_alergens = 0;
      for(s_ptr = strtok(line, " (),\n"); s_ptr != NULL; s_ptr = strtok(NULL, " (),\n"))
      {
         if(strcmp(s_ptr, ALERG_SEPARATOR) == 0)
         {
            is_alergens = 1;
         }
         else if(is_alergens == 0)
         {
            // An ingredient token
            strncpy(f[nof_foods_init].ingred[f[nof_foods_init].nof_ingred++].name, s_ptr, KEY_LEN);
         }
         else
         {
            // An alergent token
            strncpy(f[nof_foods_init].alerg[f[nof_foods_init].nof_alerg++].name, s_ptr, KEY_LEN);
         }
      }
      nof_foods_init++;
   }
}

int unique_alergs_init(alerg_match_t *a, food_t *f)
{
   int nof_unique_alergs = 0;
   int new;
   for(int i = 0; i < NOF_FOODS; i++)
   {
      for(int j = 0; j < f[i].nof_alerg; j++)
      {
         new = 1;
         for(int k = 0; k < nof_unique_alergs; k++)
         {
            if(strcmp(f[i].alerg[j].name, a[k].name) == 0)
            {
               new = 0;
               break;
            }
         }
         if(new == 1)
         {
            strncpy(a[nof_unique_alergs].name, f[i].alerg[j].name, KEY_LEN);
            nof_unique_alergs++;
         }
      }
   }

   return nof_unique_alergs;
}

int food_contains_alergent(char* n, food_t f)
{
   for(int i = 0; i < f.nof_alerg; i++)
   {
      if(strcmp(n, f.alerg[i].name) == 0)
      {
         return 1;
      }
   }

   return 0;
}

void join_ingredients(int *ix_list, int list_len, food_t *f, alerg_match_t *a_m)
{
   char *current_ingred;
   int is_valid_loop, is_valid_all;
   // Loop over all ingredients for first food in ix list
   for(int i = 0; i < f[ix_list[0]].nof_ingred; i++)
   {
      is_valid_all = 0;
      current_ingred = f[ix_list[0]].ingred[i].name;
      // printf("TESTING INGRED: %s\n", current_ingred);
      // Loop over all other food list and check for a match
      for(int j = 1; j < list_len; j++)
      {
         is_valid_loop = 0;
         for(int k = 0; k < f[ix_list[j]].nof_ingred; k++)
         {
            if(strncmp(current_ingred, f[ix_list[j]].ingred[k].name, KEY_LEN) == 0)
            {
               // printf("exists in food %d\n", ix_list[j]);
               is_valid_loop = 1;
               break;
            }
         }
         if(is_valid_loop == 0)
         {
            break;
         }
         if(j == list_len-1)
         {
            is_valid_all = 1;
         }
      }

      if(is_valid_all == 1)
      {
         strncpy(a_m->candidates[a_m->nof_candidates].name, current_ingred, KEY_LEN);
         a_m->candidate_ix[a_m->nof_candidates] = 1;
         a_m->nof_candidates++;
      }
   }
}

void match_alergent(alerg_match_t *a_m, food_t *f)
{
   int food_ix[NOF_FOODS];
   memset(food_ix, 0, NOF_FOODS*sizeof(int));
   int nof_foods_to_join = 0;

   for(int i = 0; i < NOF_FOODS; i++)
   {
      if(food_contains_alergent(a_m->name, f[i]))
      {
         food_ix[nof_foods_to_join++] = i;
      }
   }
   join_ingredients(food_ix, nof_foods_to_join, f, a_m);
}

int main(int argc, char *argv[])
{
   // Food abstraction stuff
   food_t food[NOF_FOODS];
   memset(food, 0, NOF_FOODS*sizeof(food_t));

   file_parse(food);

   // Create a list of unique alergents to look up
   alerg_match_t alerg_list[NOF_UNIQUE_ALERGS];
   memset(alerg_list, 0, NOF_UNIQUE_ALERGS*sizeof(alerg_match_t));
   int nof_unique_alergs = unique_alergs_init(alerg_list, food);

   // Go through the list of unique alergents and match them
   // with ingredient candidates
   for(int i = 0; i < nof_unique_alergs; i++)
   {
      match_alergent(&alerg_list[i], food);
   }

   int resolved_alergents = 0;
   char *resolved_ingred;
   int start_move;

   // Work while everything is not resolved
   while(resolved_alergents < nof_unique_alergs)
   {
      // Check if some alergent is done
      for(int i = 0; i < nof_unique_alergs; i++)
      {
         // Skip the ones that are done and done means 1 valid candidate left
         if(alerg_list[i].resolved == 0 && alerg_list[i].nof_candidates == 1)
         {
            alerg_list[i].resolved = 1;
            resolved_alergents++;
            resolved_ingred = alerg_list[i].candidates[0].name;

            // Remove the resolved ingredient
            for(int j = 0; j < nof_unique_alergs; j++)
            {
               // Should be another alergant
               if(i != j)
               {
                  // Only need to check un-resolved alergant
                  if(alerg_list[j].resolved == 0)
                  {
                     start_move = 0;
                     // Go through candidate lists
                     for(int k = 0; k < alerg_list[j].nof_candidates; k++)
                     {
                        // Check for match
                        if(strncmp(resolved_ingred, alerg_list[j].candidates[k].name, KEY_LEN) == 0)
                        {
                           start_move = 1;
                        }
                        else if(start_move == 1) // We have had the match start shifting data
                        {
                           strncpy(alerg_list[j].candidates[k-1].name, alerg_list[j].candidates[k].name, KEY_LEN);
                        }
                     }
                     if(start_move == 1)
                     {
                        // A candidate was removed
                        alerg_list[j].nof_candidates--;
                     }
                  }
               }
            }
         }
      }
   }

   // Count total number of unsafe ingredients in list of foods
   unsigned safe_ingreds = 0;
   int is_safe;
   for(int i = 0; i < NOF_FOODS; i++)
   {
      for(int j = 0; j < food[i].nof_ingred; j++)
      {
         is_safe = 1;
         for(int k = 0; k < nof_unique_alergs; k++)
         {
            if(strncmp(food[i].ingred[j].name, alerg_list[k].candidates[0].name, KEY_LEN) == 0)
            {
               is_safe = 0;
               break;
            }
         }
         if(is_safe == 1)
         {
            safe_ingreds++;
         }
      }
   }

   printf("Number of safe ingredients: %u\n", safe_ingreds);

   // For task 2 we need a list of unsafe ingredients sorted by alergent
   alerg_match_t tmp;
   for(int i = 0; i < nof_unique_alergs; i++)
   {
      for(int j = i+1; j < nof_unique_alergs; j++)
      {
         if(strncmp(alerg_list[i].name, alerg_list[j].name, KEY_LEN) > 0)
         {
            tmp = alerg_list[j];
            alerg_list[j] = alerg_list[i];
            alerg_list[i] = tmp;
         }
      }
   }

   // Print on correct format
   for(int i = 0; i < nof_unique_alergs; i++)
   {
      printf("%s,", alerg_list[i].candidates[0].name);
   }
}
