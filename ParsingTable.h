#include "stdafx.h"

//See documentation for information about the input parameters
int **Generate(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount);



//returns an array with the production items in the selected row of the grammer in the order they appear in the production. 
//The array will be terminated with a "-1"
//__Grammer: Grammer matrix
//__rowIndex: Production row to return
//__countColumns: Columns in grammer matrix
//__countOutput: The number of valid indices in the output
int * GetProduction(int **__Grammer, int __rowIndex, int __countColumns, int *__countOutput);


