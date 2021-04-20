
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <chrono>

#define num_threads 27

using namespace std;
using namespace std::chrono;
/*
 class that holds the parameters passed to a thread.
*/

class parameters{
public :
int row;
int col;
int (* grid)[9];
};

/*
    Initialize the array which worker threads can update to 1 if the
    corresponding region of the sudoku puzzle they were responsible
    for is valid.
*/
int result[num_threads] = {0};

// function for 3x3 square function.
void *check_grid(void *params);

// function for the check_rows function.
void *check_rows(void *params);

// function for the check_cols function.
void *check_cols(void *params);


/***************
 * ENTRY POINT *
 **************/
int main(void)
{
    //====== The Sudoku puzzle to be validated =======
    int sudoku[9][9] =
    {
            {6, 2, 4, 5, 3, 9, 1, 8, 7},
            {5, 1, 9, 7, 2, 8, 6, 3, 4},
            {8, 3, 7, 6, 1, 4, 2, 9, 5},
            {1, 4, 3, 8, 6, 5, 7, 2, 9},
            {9, 5, 8, 2, 4, 7, 3, 6, 1},
            {7, 6, 2, 3, 9, 1, 4, 5, 8},
            {3, 7, 1, 9, 5, 6, 8, 4, 2},
            {4, 9, 6, 1, 8, 2, 5, 7, 3},
            {2, 8, 5, 4, 7, 3, 9, 1, 6}
     };
     
     cout<< "sudoku puzzle to be validated : "<<endl;
     
     for(int i=0; i<9; i++)
     {
     	for(int j=0; j<9;j++)
     	{
     		cout<<sudoku[i][j]<<(j + 1 < 9 ? " , " :"\n");
     	}
     }
     cout<<endl;
     
 
    // Starting time for execution with 27 threads
    steady_clock::time_point start_time_threads = steady_clock::now();

    pthread_t threads[num_threads];

    int threadIndex = 0;

    // ====== Create the threads ======
    //Create one thread each for the 9 3x3 grid, one thread each for the 9 columns and one thread each for the 9 rows. There will be a total of 27 thread created
    //Function call parameters are Thread identifier, thread attributes, function the thread executes, parameters passed to function
    //Syntax for pthread_create function is pthread_create(pthread_t * thread, pthread_attr_t * attr, void * (*start_routine)(void *), void * arg);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {

            if (i%3 == 0 && j%3 == 0)
            {
                parameters *gridData = (parameters *) new(parameters);
                gridData->row = i;
                gridData->col = j;
                gridData->grid = sudoku;
                pthread_create(&threads[threadIndex++], NULL, check_grid, gridData);
            }

            // ====== Declaration of the parameter for the row check threads =======
            if (j == 0)
            {
                parameters *rowData = (parameters *) new(parameters);
                rowData->row = i;
                rowData->col = j;
                rowData->grid = sudoku;
                pthread_create(&threads[threadIndex++], NULL, check_rows, rowData);
            }

            // ====== Declaration of the parameter for the column check threads =======
            if (i == 0)
            {
                parameters *columnData = (parameters *) new(parameters);
                columnData->row = i;
                columnData->col = j;
                columnData->grid = sudoku;
                pthread_create(&threads[threadIndex++], NULL, check_cols, columnData);
            }
        }
    }

    // ======= Wait for all threads to finish their tasks =======
    //Parameters are Thread identifier and the return value of the function executed by the thread
    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);


    // If any of the entries in the valid array are 0, then the Sudoku solution is invalid
    for (int i = 0; i < num_threads; i++)
    {
        if (result[i] == 0)
        {
            cout << "Sudoku solution is invalid" << endl;

            // Compute and return the elapsed time in milliseconds.
            steady_clock::time_point end_time_threads = steady_clock::now();
            duration<double> elapsed_time_threads = duration_cast<duration<double>>(end_time_threads - start_time_threads);

            cout << endl << "Total time to validate sudoku solution using 27 threads: " << elapsed_time_threads.count() << " seconds" << endl;
            return 1;
        }
    }
    cout << "Sudoku solution is valid" << endl;

    // Compute and return the elapsed time in milliseconds.
    steady_clock::time_point end_time_threads = steady_clock::now();
    duration<double> elapsed_time_threads = duration_cast<duration<double>>(end_time_threads - start_time_threads);

    cout << endl << "Total time to validate sudoku solution using 27 threads: " << elapsed_time_threads.count() << " s" << endl;

}

/*
 * Checks if a square of size 3x3 contains all numbers from 1-9.
 * There is an array called validarray[10] initialized to 0.
 * For every value in the square, the corresponding index in validarray[] is checked for 0 and set to 1.
 * If the value in validarray[] is already 1, then it means that the value is repeating. So, the solution is invalid.
 *
 * @param   void *      The parameters (pointer).
 */
void *check_grid(void * params)
{
    parameters *data = (parameters *) params;
    int startRow = data->row;
    int startCol = data->col;
    int validarray[10] = {0};
    for (int i = startRow; i < startRow + 3; ++i)
    {
        for (int j = startCol; j < startCol + 3; ++j)
        {
            int val = data->grid[i][j];
            if (validarray[val] != 0)
                pthread_exit(NULL);
            else
                validarray[val] = 1;
        }
    }

    // If the execution has reached this point, then the 3x3 sub-grid is valid.
    result[startRow + startCol/3] = 1; // Maps the 3X3 sub-grid to an index in the first 9 indices of the result array
    pthread_exit(NULL);
}

/**
 * Checks each row if it contains all digits 1-9.
 * There is an array called validarray[10] initialized to 0.
 * For every value in the row, the corresponding index in validarray[] is checked for 0 and set to 1.
 * If the value in validarray[] is already 1, then it means that the value is repeating. So, the solution is invalid.
 *
 * @param   void *      The parameters (pointer).
 */

void *check_rows(void *params)
{
    parameters *data = (parameters *) params;
    int row = data->row;

    int validarray[10] = {0};
    for (int j = 0; j < 9; j++)
    {
        int val = data->grid[row][j];
        if (validarray[val] != 0)
            pthread_exit(NULL);
        else
            validarray[val] = 1;
    }

    // If the execution has reached this point, then the row is valid.
    result[9 + row] = 1; // Maps the row to an index in the second set of 9 indices of the result array
    pthread_exit(NULL);
}

/**
 * Checks each column if it contains all digits 1-9.
 * There is an array called validarray[10] initialized to 0.
 * For every value in the row, the corresponding index in validarray[] is checked for 0 and set to 1.
 * If the value in validarray[] is already 1, then it means that the value is repeating. So, the solution is invalid.
 *
 * @param   void *      The parameters (pointer).
 */
void *check_cols(void *params)
{
    parameters *data = (parameters *) params;
    //int startRow = data->row;
    int col = data->col;

    int validarray[10] = {0};
    for (int i = 0; i < 9; i++)
    {
        int val = data->grid[i][col];
        if (validarray[val] != 0)
            pthread_exit(NULL);
        else
            validarray[val] = 1;
    }

    // If the execution has reached this point, then the column is valid.
    result[18 + col] = 1; // Maps the column to an index in the third set of 9 indices of the result array
    pthread_exit(NULL);
 }

/**
 * Checks each column/row if it contains all digits 1-9.
 * There is an array called validarray[10] initialized to 0.
 * For every value in the row/column, the corresponding index in validarray[] is checked for 0 and set to 1.
 * If the value in validarray[] is already 1, then it means that the value is repeating. So, the solution is invalid.
 *
 * @param   int      the row/column to be checked.
 */
int check_line(int input[9])
{
    int validarray[10] = {0};
    for (int i = 0; i < 9; i++)
    {
        int val = input[i];
        if (validarray[val] != 0)
            return 1;
        else
            validarray[val] = 1;
    }
    return 0;
}

/**
 * Checks each 3*3 grid if it contains all digits 1-9.
 * There is an array called validarray[10] initialized to 0.
 * For every value in the row/column, the corresponding index in validarray[] is checked for 0 and set to 1.
 * If the value in validarray[] is already 1, then it means that the value is repeating. So, the solution is invalid.
 *
 * @param   void *      The parameters (pointer).
 */
int check_grid(int sudoku[9][9])
{
    int temp_row, temp_col;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            temp_row = 3 * i;
            temp_col = 3 * j;
            int validarray[10] = {0};

            for(int p=temp_row; p < temp_row+3; p++)
            {
                for(int q=temp_col; q < temp_col+3; q++)
                {
                    int val = sudoku[p][q];
                    if (validarray[val] != 0)
                        return 1;
                    else
                        validarray[val] = 1;
                }
            }
        }
    }
    return 0;
}

/**
 * Checks if the sudoku solution is valid or not without using the PThreads function.
 *
 *
 * @param   int      The sudoku solution.
 */
int sudoku_checker(int sudoku[9][9])
{
    for (int i=0; i<9; i++)
    {
        /* check row */
        if(check_line(sudoku[i]))
            return 1;

        int check_col[9];
        for (int j=0; j<9; j++)
            check_col[j] = sudoku[i][j];

        /* check column */
        if(check_line(check_col))
            return 1;

        /* check grid */
        if(check_grid(sudoku))
            return 1;
    }
    return 0;
}