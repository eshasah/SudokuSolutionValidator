#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
using namespace std::chrono;


// Prototype for the single thread sudoku check function.
int sudoku_checker(int sudoku[9][9]);

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
     
     
     // Starting time for single thread execution
    steady_clock::time_point start_time_single_thread = steady_clock::now();
 cout<<"Sudoku validation using single thread : " <<endl<<endl;
    if(sudoku_checker(sudoku))
        printf("Sudoku solution is invalid\n");
    else
        printf("Sudoku solution is valid\n");

    // Compute and return the elapsed time in milliseconds.
    steady_clock::time_point end_time_single_thread = steady_clock::now();
    auto elapsed_time_single_thread = std::chrono::duration_cast<std::chrono::microseconds>(end_time_single_thread - start_time_single_thread);

    cout << endl << "Total time using single thread: " << elapsed_time_single_thread.count() << " microseconds" << endl << endl;
  
}
 //Checks each column/row if it contains all digits 1-9.

int check_line(int input[9])
{
    int validateline[10] = {0};
    for (int i = 0; i < 9; i++)
    {
        int val = input[i];
        if (validateline[val] != 0)
            return 1;
        else
            validateline[val] = 1;
    }
    return 0;
}


 //Checks each 3*3 grid if it contains all digits 1-9.

int check_grid(int sudoku[9][9])
{
    int temp_row, temp_col;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            temp_row = 3 * i;
            temp_col = 3 * j;
            int validategrid[10] = {0};

            for(int p=temp_row; p < temp_row+3; p++)
            {
                for(int q=temp_col; q < temp_col+3; q++)
                {
                    int val = sudoku[p][q];
                    if (validategrid[val] != 0)
                        return 1;
                    else
                        validategrid[val] = 1;
                }
            }
        }
    }
    return 0;
}


 // Checks if the sudoku solution is valid or not without using the PThreads function.

int sudoku_checker(int sudoku[9][9])
{
    for (int i=0; i<9; i++)
    {
        /* check row */
        if(check_line(sudoku[i]))
        {
        	return 1;
        }

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


