#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

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
void *check_grid(void *value);

// function for the check_rows function.
void *check_rows(void *value);

// function for the check_cols function.
void *check_cols(void *value);


int main(void)
{

const size_t N= 9;
    //====== The Sudoku puzzle to be validated =======
    int sudoku[9][9] ={};
   string filename;
   cout<< "Please enter the input file name which contains the sudoku puzzle to be validated : ";
   cin >> filename;
   
     std::ifstream file(filename);
     std::string line;

     
     for(size_t i=0; i<N && std::getline(file,line); i++)
     {
     	std::istringstream is (line);
     	size_t  j=0;
     	while(j<N && is >> sudoku[i][j]) j++;
     }
     
          cout<< "sudoku puzzle to be validated : "<<endl;
     
     for(int x=0; x<9; x++)
     {
     	for(int y=0; y<9;y++)
     	{
     		cout<<sudoku[x][y]<<(y + 1 < 9 ? " , " :"\n");
     	}
     }
     cout<<endl;
     
         
    cout<<"Sudoku validation using 27 threads : " <<endl<<endl;
    
    // Starting time for execution with 27 threads
    steady_clock::time_point start_time_threads = steady_clock::now();

    pthread_t threads[num_threads];

    int threadIndex = 0;

    // ====== Create the threads ======
    //Create one thread each for the 9 3x3 grid, one thread each for the 9 columns and one thread each for the 9 rows. There will be a total of 27 thread created
 
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

            //  Declaration of the parameter for the column check threads
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
            auto elapsed_time_threads = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_threads - start_time_threads);

            cout << endl << "Total time to validate sudoku solution using 27 threads: " << elapsed_time_threads.count() << " milliseconds" << endl;
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
 */
void *check_grid(void * value)
{
    parameters *data = (parameters *) value;
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


//Checks each row if it contains all digits 1-9.


void *check_rows(void *value)
{
    parameters *data = (parameters *) value;
    int row = data->row;

    int validaterow[10] = {0};
    for (int j = 0; j < 9; j++)
    {
        int val = data->grid[row][j];
        if (validaterow[val] != 0)
            pthread_exit(NULL);
        else
            validaterow[val] = 1;
    }

    // If the execution has reached this point, then the row is valid.
    result[9 + row] = 1; // Maps the row to an index in the second set of 9 indices of the result array
    pthread_exit(NULL);
    cout<<"validation successfull";
}


// Checks each column if it contains all digits 1-9.

void *check_cols(void *value)
{
    parameters *data = (parameters *) value;
    //int startRow = data->row;
    int col = data->col;

    int validatecolumn[10] = {0};
    for (int i = 0; i < 9; i++)
    {
        int val = data->grid[i][col];
        if (validatecolumn[val] != 0)
            pthread_exit(NULL);
        else
            validatecolumn[val] = 1;
    }

    // If the execution has reached this point, then the column is valid.
    result[18 + col] = 1; // Maps the column to an index in the third set of 9 indices of the result array
    pthread_exit(NULL);
 }

