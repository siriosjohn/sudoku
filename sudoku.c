/************************************************************
 * Giannis Siriopoulos
 ************************************************************/

#include "sudoku.h"

Grid_T sudoku_read()
{
    Grid_T *grid = malloc(sizeof(Grid_T));
    int i = 0, j = 0, val;
    grid_set_unique(grid);
    while (i < 9)
    {
        if (j == 9)
        {
            i++;
            j = 0;
            continue;
        }
        scanf("%d", &val);
        grid_update_value(grid, i, j, val);
        j++;
    }
    return *grid;
}

/* Print the sudoku puzzle defined by g to stream s in the same format
   as expected by sudoku_read(). */
void sudoku_print(FILE *s, Grid_T g)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            fprintf(s, "%d ", grid_read_value(g, i, j));
        }
        fprintf(s, "\n");
    }
}

/* Print all row, col, sub-grid errors/conflicts found in puzzle g;
   some errors may be reported more than once. */
void sudoku_print_errors(Grid_T g)
{
    int i;
    for (i = 0; i < 9; i++)
        if (check_row(g, i) == 0)
            printf("-Row %d has duplicate numbers\n", i);

    for (i = 0; i < 9; i++)
        if (check_col(g, i) == 0)
            printf("-Column %d has duplicate numbers\n", i);

    for (i = 0; i < 9; i++)
        if (check_subgrid(g, i) == 0)
            printf("-Subgrid %d has duplicate numbers\n", i);
}

/* Return true iff puzzle g is correct. */
int sudoku_is_correct(Grid_T g)
{
    int i;
    for (i = 0; i < 9; i++)
        if (check_row(g, i) == 0)
            return 0;

    for (i = 0; i < 9; i++)
        if (check_col(g, i) == 0)
            return 0;

    for (i = 0; i < 9; i++)
        if (check_subgrid(g, i) == 0)
            return 0;

    return 1;
}

/*Initilizes available choices for each position of the grid*/
static void sudoku_init_choices(Grid_T *g)
{
    int i, j, k;
    int block[9];

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (grid_read_value(*g, i, j) != 0)
                continue;

            subgrid(*g, i, j, block);

            for (k = 1; k < 10; k++)
                grid_set_choice(g, i, j, k);

            grid_set_count(g, i, j);
            for (k = 0; k < 9; k++)
            {
                grid_remove_choice(g, i, j, grid_read_value(*g, i, k)); /*checks row*/
                grid_remove_choice(g, i, j, grid_read_value(*g, k, j)); /*checks col*/
                grid_remove_choice(g, i, j, block[k]);                  /*checks subgrid*/
            }
        }
    }
}

/*returns one choice from a random position with at least one available choice*/
static int sudoku_try_next(Grid_T g, int *row, int *col)
{
    int i, j, n, best_choice = 9;
    srand(getpid());
    i = rand() % 9;
    for (n = 0; n < 9; n++)
    {
        for (j = 0; j < 9; j++)
        {
            if (grid_read_value(g, i, j) != 0)
                continue;
            if (grid_read_count(g, i, j) == 0)
                return 0;
            if (grid_read_count(g, i, j) < best_choice)
            {
                *row = i;
                *col = j;
                best_choice = grid_read_count(g, i, j);
            }
        }
        i++;
        if (i == 9)
            i = 0;
    }
    for (n = 1; n < 10; n++)
    {
        if (grid_choice_is_valid(g, *row, *col, n) == 1)
            return n;
    }

    return 0;
}

/*sets n as element value and removes it from the available choices
  returns the number of available choices for that element*/
static int sudoku_update_choice(Grid_T *g, int i, int j, int n)
{
    grid_remove_choice(g, i, j, n);
    grid_update_value(g, i, j, n);
    return grid_read_count(*g, i, j);
}

/*removes choice n from every element in row 'r', column 'c' and the right subgrid*/
static void sudoku_eliminate_choice(Grid_T *g, int r, int c, int n)
{
    int i, j;
    /*reomve n from every element in that row and that column*/
    for (i = 0; i < 9; i++)
    {
        grid_remove_choice(g, r, i, n);
        grid_remove_choice(g, i, c, n);
    }

    /*remove choice n from every element in the correct subgrid*/
    for (i = (r / 3) * 3; i < ((r / 3) * 3) + 3; i++)
    {
        for (j = (c / 3) * 3; j < ((c / 3) * 3) + 3; j++)
        {
            grid_remove_choice(g, i, j, n);
        }
    }
}

/* Sovle puzzle g and return the solved puzzle; if the puzzle has
   multiple solutions, return one of the possible solutions. */
Grid_T sudoku_solve(Grid_T g)
{
    int i = 0, j = 0, k;
    Grid_T tmp;
    sudoku_init_choices(&g);
    while (!sudoku_is_correct(tmp) && !sudoku_is_correct(g))
    {

        k = sudoku_try_next(g, &i, &j);
        if (k == 0)
            return g;
        if (sudoku_update_choice(&g, i, j, k) != 0)
        {
            tmp = sudoku_solve(g);
            if (sudoku_is_correct(tmp))
            {
                if (grid_read_count(tmp, i, j) != 0)
                    grid_clear_unique(&tmp);

                return tmp;
            }
            grid_update_value(&g, i, j, 0);
        }
        else
            sudoku_eliminate_choice(&g, i, j, k);
    }
    return g;
}

/* Returns true if solution g, as returned by sudoku_solve, has a
   unique choice for each step (no backtracking required). (Note, g
   must have been computed with the use of sudoku_solve.) */
int sudoku_solution_is_unique(Grid_T g)
{
    return g.unique;
}

/*generates a complete puzzle using sudoku solve*/
Grid_T sudoku_generate_complete()
{
    int i = 0, j = 0;
    Grid_T g;
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            grid_update_value(&g, i, j, 0);
        }
    }
    g = sudoku_solve(g);
    return g;
}

/* Generate and return a sudoku puzzle with "approximately" nelts
   elements having non-0 value. The smaller nelts the harder may be to
   generate/solve the puzzle. For instance, nelts=81 should return a
   completed and correct puzzle. */
Grid_T sudoku_generate(int nelts)
{
    int i, j, k;
    Grid_T g;
    g = sudoku_generate_complete();
    for (k = 81; k > nelts; k--)
    {
        i = rand() % 9;
        j = rand() % 9;
        if (grid_read_value(g, i, j) == 0)
            k++;
        grid_update_value(&g, i, j, 0);
    }
    return g;
}

int main(int argc, char const *argv[])
{
    switch (argc)
    {
    case 2:
        if (strcmp("-c", argv[1]) == 0)
        {
            Grid_T *new = (Grid_T *)malloc(sizeof(Grid_T));
            *new = sudoku_read();
            sudoku_print(stderr, *new);
            if (sudoku_is_correct(*new))
                printf("Sudoku is correct!\n");
            else
            {
                printf("Sudoku is not correct\n");
                sudoku_print_errors(*new);
            }
        }
        else
        {
            fprintf(stderr, "Wrong argument format\nTry Again!!!\n");
            return -1;
        }
        break;

    case 3:
        if (argc == 3)
        {
            Grid_T out;
            printf("Generating puzzle:\n");
            out = sudoku_generate(atoi(argv[2]));
            sudoku_print(stderr, out);
        }

        break;

    default:
        if (argc == 1)
        {
            Grid_T new;
            Grid_T solved;
            new = sudoku_read();
            printf("Starting puzzle:\n");
            sudoku_print(stderr, new);
            printf("\nsolving puzzle...\n\n");
            
            solved = sudoku_solve(new);
            printf("Solution is:\n");
            sudoku_print(stderr, solved);
            if (sudoku_is_correct(solved))
            {
                if (sudoku_solution_is_unique(solved))
                    printf("Puzzle solution is correct and unique\n");
                else
                    printf("Puzzle solution is correct but NOT unique\n");
            }
            else
            {
                sudoku_print_errors(solved);
            }
        }
    }

    return 1;
}
