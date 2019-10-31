/********************************************************************
   grid.h

   Provides definition of grid ADT used for solving sudoku puzzles and
   functions required to access and modify elements of the structure.
*********************************************************************/
#include <stdio.h>

typedef struct grid_s
{
  int unique; /* if 1 after solving, puzzle has unique solution */
  struct
  {
    int val; /* value of element i,j */
    struct
    {
      int count;   /* number of possible choices for element i,j */
      int num[10]; /* map of choices: num[k]==1 if k is a valid choice
                      for element i,j, else num[k]=0 */
    } choices;     /* struct choices is used while solving the
		      puzzle and is otherwise undefined */
  } elts[9][9];    /* sudoku grid elements; 0<=i,j<=9 */
} Grid_T;

/* update value of i,j to n */
void grid_update_value(Grid_T *g, int i, int j, int n);
/* return value of i,j */
int grid_read_value(Grid_T g, int i, int j);

/* set (to 1) or clear (to 0) choice n for elt i,j */
void grid_set_choice(Grid_T *g, int i, int j, int n);
void grid_clear_choice(Grid_T *g, int i, int j, int n);
/* true if choice n for elt i,j is valid */
int grid_choice_is_valid(Grid_T g, int i, int j, int n);
/* remove n from ch oices of elt i,j and adjust count only if n is a
  valid choice for elt i,j */
void grid_remove_choice(Grid_T *g, int i, int j, int n);

/* return count, set (to 9), or clear (to 0) count for elt i, j */
int grid_read_count(Grid_T g, int i, int j);
void grid_set_count(Grid_T *g, int i, int j);
void grid_clear_count(Grid_T *g, int i, int j);

/* return val, set (to 1), or clear (to 0) unique flag for g */
int grid_read_unique(Grid_T g);
void grid_set_unique(Grid_T *g);
void grid_clear_unique(Grid_T *g);

/*returns 1 if there are no duplicates or 0 if there are duplicates*/
int check_subgrid(Grid_T g, int n);
int check_row(Grid_T g, int n);
int check_col(Grid_T g, int n);

void subgrid(Grid_T g, int i, int j, int array[9]);