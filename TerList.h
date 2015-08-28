#include "stdafx.h"
//Generates a parsing table for an input LL(1) grammer. See documentation for details on how to call the function

// a linked list of terminals/nonterminals
struct TerList{
	struct TerList *Next;
	int Value;
};

struct TerList **InitializeArr(int __size);
int Add(struct TerList *__ptrHeader, int __intValue);
void PrintTerList(struct TerList **__ptrHeader, int __RowCount);
