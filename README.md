# SudokuSolutionValidator

Introduction :

Sudoku is a logic based number placement puzzle game. The standard sudoku is a table made of 9 rows , 9 columns and 9 3x3 boxes.The puzzle starts with given numbers in various positions and the players goal is to complete the table such that each row, column and box contains every number from 1 to 9 exactly once.Although 9x9 sudoku validation is very common, larger variation exist to increase the puzzle difficulty.

Objective :

The objective of this project is to design a multi-threaded application(27 threads) in C++ language to determine whether the solution to the sudoku puzzle is valid.There are several ways of multithreading this application.We would be using the below approach :
●	Nine threads to check each of the 9 columns contains the digit 1 through 9
●	Nine threads to check each of the 9 rows contains the digit 1 through 9
●	Nine threads to check that each  of nine 3x3 subgrids contain the digits 1 through 9
This would result in a total of 27 separate threads  for validating a sudoku puzzle.
