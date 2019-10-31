/************************************************************
 * Giannis Siriopoulos
 ************************************************************/
#include "grid.h"

/* update value of i,j to n */
void grid_update_value(Grid_T *g, int i, int j, int n)
{
    g->elts[i][j].val = n;
}

/* return value of i,j */
int grid_read_value(Grid_T g, int i, int j)
{
    return g.elts[i][j].val;
}

/* set (to 1) choice n for elt i,j */
void grid_set_choice(Grid_T *g, int i, int j, int n)
{
    g->elts[i][j].choices.num[n] = 1;
}

/* clear (to 0) choice n for elt i,j */
void grid_clear_choice(Grid_T *g, int i, int j, int n)
{
    g->elts[i][j].choices.num[n] = 0;
}

/* true if choice n for elt i,j is valid */
int grid_choice_is_valid(Grid_T g, int i, int j, int n)
{
    return g.elts[i][j].choices.num[n] == 1;
}

/* remove n from choices of elt i,j and adjust count only if n is a
   valid choice for elt i,j */
void grid_remove_choice(Grid_T *g, int i, int j, int n)
{
    if (grid_choice_is_valid(*g, i, j, n))
        g->elts[i][j].choices.count--;
    grid_clear_choice(g, i, j, n);
}

/*return the count for choices for elt i, j*/
int grid_read_count(Grid_T g, int i, int j)
{
    return g.elts[i][j].choices.count;
}
/*set the count of choices*/
void grid_set_count(Grid_T *g, int i, int j)
{
    g->elts[i][j].choices.count = 9;
}
/*clear the count of choices*/
void grid_clear_count(Grid_T *g, int i, int j)
{
    g->elts[i][j].choices.count = 0;
}

/*return the val of flag for uniqueness*/
int grid_read_unique(Grid_T g)
{
    return g.unique;
}

/*set unique to 1*/
void grid_set_unique(Grid_T *g)
{
    g->unique = 1;
}

/*set unique to 0*/
void grid_clear_unique(Grid_T *g)
{
    g->unique = 0;
}

/*checks subgrid: returns 1 if there are no duplicates or 0 if there are duplicates*/
int check_subgrid(Grid_T g, int n)
{
    int i, j, k = 0;
    int E[9];

    /*pass all elements from a subgrid to an Array*/
    for (i = (n / 3) * 3; i < ((n / 3) * 3) + 3; i++)
    {
        for (j = (n % 3) * 3; j < ((n % 3) * 3) + 3; j++)
        {
            E[k] = grid_read_value(g, i, j);
            k++;
        }
    }

    /*cecks array for duplicates*/
    for (i = 0; i < 9; i++)
    {
        for (j = i + 1; j < 9; j++)
        {
            if (E[i] == E[j])
                return 0;
        }
    }

    return 1;
}

/*checks row: returns 1 if there are no duplicates or 0 if there are duplicates*/
int check_row(Grid_T g, int n)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = i + 1; j < 9; j++)
        {
            if (grid_read_value(g, n, i) == grid_read_value(g, n, j))
                return 0;
        }
    }

    return 1;
}

/*checks column: returns 1 if there are no duplicates or 0 if there are duplicates*/
int check_col(Grid_T g, int n)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = i + 1; j < 9; j++)
        {
            if (grid_read_value(g, i, n) == grid_read_value(g, j, n))
                return 0;
        }
    }

    return 1;
}

/*returns subgrid that position (i,j) belongs in an array*/
void subgrid(Grid_T g, int i, int j, int array[9])
{
    int a, b;
    int k = 0;

    for (a = (i / 3) * 3; a < ((i / 3) * 3) + 3; a++)
    {
        for (b = (j / 3) * 3; b < ((j / 3) * 3) + 3; b++)
        {
            array[k] = grid_read_value(g, a, b);
            k++;
        }
    }
}