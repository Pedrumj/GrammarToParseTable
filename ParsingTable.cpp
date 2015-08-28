#include "stdafx.h"
#include "TerList.h"
#include <Windows.h>


//Generates a parsing table for an input LL(1) grammer. See documentation for details on how to call the function


void SetFirstAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas);
void TestPrint(int *__Output, int __RowCount, char *header);

void SetFollowAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas, struct TerList **__FollowAlphas);
int GetItemInProduction(int **__Grammer, int __RowIndex, int __ColumnCount, int __ItemIndex);


//Generates the parse table based on the input grammer. 
int **Generate(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount){
	
	// an array of linked lists. each index contains the FIRST set for the item in the grammer (Nonterminal + Terminal +Eps)
	struct TerList **_FirstAlphas = InitializeArr( __TerminalCount+__NonTerminalCount+1);
	// an array of linked lists each index contains the FOLLOW set for the Nonterminals of the grammer
	struct TerList **_FollowAs = InitializeArr(__NonTerminalCount);
	//The parse table. 
	int **_output = (int **) malloc(sizeof(int *)*__NonTerminalCount);

	int _ItemInProduction;
	struct TerList * _ptrFirstTemp;
	struct TerList * _ptrFollowTemp;
	int _flagEpsFound;
	//initialize parse table rows
	for (int i = 0; i <__NonTerminalCount;i++){
		_output[i] = (int *) malloc(sizeof(int)*(__TerminalCount+1));
	}

	// set all elements of the output matrix to -1
	for (int i = 0; i <__NonTerminalCount;i++){
		for (int j =0; j<(__TerminalCount+1);j++){
			_output[i][j] = -1;
		}
	}

	//Calcualte the FIRST sets
	SetFirstAs(__Grammer, __Rows, __RowCount, __TerminalCount, __NonTerminalCount, _FirstAlphas);
	//Calculate the FOLLOW sets
	SetFollowAs(__Grammer, __Rows, __RowCount, __TerminalCount, __NonTerminalCount, _FirstAlphas, _FollowAs);

	//loop through grammer rows
	for (int j = 0; j < __RowCount;j ++){
		// loop through nonterminals
		for (int i = 0; i<__NonTerminalCount;i++){
			if (i==__Rows[j]){
				//k = 0 would mean the first item in the production. k=1 would mean the second item in the production ...
				for (int k=0; k <__NonTerminalCount+__TerminalCount+1;k++){
					//returns the columns for the k'th item in the selected production
					_ItemInProduction =  GetItemInProduction(__Grammer, j, __NonTerminalCount+__TerminalCount+1, k);
					//if the item was not found then we have past the last item in the production and therefore the FOLLOW set
					//must be added
					if (_ItemInProduction == -1){

						_ptrFollowTemp = _FollowAs[i];
						while(_ptrFollowTemp->Next !=NULL){
							if (_ptrFollowTemp->Value==__NonTerminalCount+__TerminalCount +1){
								//if its the endmarker ($)
								_output[i][_ptrFollowTemp->Value - __NonTerminalCount-1] = j;
							}
							else
							{
								_output[i][_ptrFollowTemp->Value - __NonTerminalCount] = j;
							}

							_ptrFollowTemp = _ptrFollowTemp->Next;							
						}
						//move to next row in grammer
						k = __NonTerminalCount+__TerminalCount+1;
						i = __NonTerminalCount;
					}
					else{
						_flagEpsFound = 0;
						_ptrFirstTemp = _FirstAlphas[_ItemInProduction];
						while (_ptrFirstTemp->Next!=NULL){
							if (_ptrFirstTemp->Value !=__NonTerminalCount + __TerminalCount){
								_output[i][_ptrFirstTemp->Value-__NonTerminalCount] = j;
							}
							else{
								//if Eps is found the loop will move to the next item in the production
								_flagEpsFound =1;
							}
							_ptrFirstTemp = _ptrFirstTemp->Next;
						}
						if (_flagEpsFound ==0){
							k = __NonTerminalCount+__TerminalCount+1;
							i = __NonTerminalCount;
						}
					}
					
				}
			}
		}
	}
	return _output;
}

//Returns the __ItemIndex'th item of the produciton specified by __RowIndex. Returns -1 if not found
int GetItemInProduction(int **__Grammer, int __RowIndex, int __ColumnCount, int __ItemIndex){
	for (int i =0; i <__ColumnCount; i++){
		if (__Grammer[__RowIndex][i] == __ItemIndex){
			return i;
		}
	}
	return -1;
}

//Generates the FOLLOW set for the nonterminals
void SetFollowAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas, struct TerList **__FollowAlphas){
	int _flagChanged;
	int j;
	struct TerList *_ptrFirst;
	int _itemIndex;
	int _itemFollowIndex;
	int _flagEpsFound;
	//Add endmarker $
	Add(__FollowAlphas[0], __TerminalCount + __NonTerminalCount+1);
	_flagChanged = 1;
	//The function will continue as long as changes have been made on the previous iteration
	while (_flagChanged !=0){
		_flagChanged =0;
		//loop through grammer rows
		for (int m = 0; m<__RowCount;m++){
			//i = 0 would mean the first item in the production. i = 1 would mean the second item in the production ...
			for (int i = 0;  i< __TerminalCount+__NonTerminalCount+1;i++){
				_itemIndex = GetItemInProduction(__Grammer, m, __TerminalCount+__NonTerminalCount+1, i);
				if (_itemIndex ==-1){
					// goto next row in grammer
					i = __TerminalCount+__NonTerminalCount+1;			
				}
				//Only calculate the FOLLOW set for nonterminals
				else if (_itemIndex < __NonTerminalCount){
					//j is an item after "i". j will keep incrementing as long as the current j has Eps in its FIRST set.
					for ( j = i+1; j <  __TerminalCount+__NonTerminalCount+1; j++){
						_flagEpsFound = 0;
						_itemFollowIndex = GetItemInProduction(__Grammer, m, __TerminalCount+__NonTerminalCount+1, j);
						//j is not an item in the production. Add the FOLLOW set of the production to the nonterminal
						if (_itemFollowIndex==-1){
							//Find the nonterminal associated with the production
							for (int k = 0; k < __NonTerminalCount; k++){
								if (__Rows[m] ==k){
									_ptrFirst = __FollowAlphas[k];
									while (_ptrFirst->Next !=NULL){		
										_flagChanged+=Add(__FollowAlphas[_itemIndex], _ptrFirst->Value);		
										_ptrFirst = _ptrFirst->Next;
									}
									//Only when "i" is the index of the last item in the production will the FOLLOW set of the 
									//nonterminal "i" be added to the production FOLLOW set.
									if (j == i+1){
										_ptrFirst = __FollowAlphas[_itemIndex];
										while (_ptrFirst->Next !=NULL){		
											_flagChanged+=Add(__FollowAlphas[k], _ptrFirst->Value);		
											_ptrFirst = _ptrFirst->Next;
										}
									}
								}
							}
						}
						//if the next item exists at the FIRST set of the item to the FOLLOW set of the nonterminal
						else{
							_ptrFirst = __FirstAlphas[_itemFollowIndex];
							while (_ptrFirst->Next !=NULL){
								if ( _ptrFirst->Value!=__TerminalCount+__NonTerminalCount){
									_flagChanged+=Add(__FollowAlphas[_itemIndex], _ptrFirst->Value);					
								}
								else{
									_flagEpsFound =1;
								}
								_ptrFirst = _ptrFirst->Next;
							}
							if (_flagEpsFound==0){
								j = __TerminalCount+__NonTerminalCount+1;
							}
						}
					}
				}
			}
		}
	}
	
}

//Generates the FIRST set for the items in the grammer
void SetFirstAs(int **__Grammer, int* __Rows, int __RowCount, int __TerminalCount, int __NonTerminalCount, 
				struct TerList **__FirstAlphas){
	int _flagEpsFound;
	int _itemIndex;
	int _flagChanged;
	struct TerList *_ptrFirstTemp;

	//Set the FIRST set for the Terminals
	for (int i = __NonTerminalCount ; i < __TerminalCount+ __NonTerminalCount+1;i++){
		Add(__FirstAlphas[i], i);
	}

	_flagChanged = 1;
	//Keeps going until no changes have been made on the last iteration
	while (_flagChanged !=0){
		_flagChanged =0;
		//loop grammer rows
		for (int i = 0 ; i < __RowCount; i ++){
			//Loop nonteraminals
			for (int j= 0; j < __NonTerminalCount; j ++){
				if (__Rows[i] == j){
					//k = 0 would mean the first item in the produciton, k= 1 would mean the second item in the production, ...
					for (int k = 0; k < __NonTerminalCount + __TerminalCount + 1;k ++){
						_itemIndex = GetItemInProduction(__Grammer, i, __NonTerminalCount + __TerminalCount+1, k);
						//This would mean all items in the production have Eps in their FIRST set. Therefore EPS will be added 
						// to the productions FIRST set.
						if (_itemIndex == -1){
							_flagChanged+=Add(__FirstAlphas[j],  __NonTerminalCount + __TerminalCount);
							k= __NonTerminalCount + __TerminalCount + 1;
							j = __NonTerminalCount;
						}
						else{
							_ptrFirstTemp = __FirstAlphas[_itemIndex];
							_flagEpsFound = 0;
							while (_ptrFirstTemp->Next !=NULL){
								if (_ptrFirstTemp->Value != __NonTerminalCount+__TerminalCount){
									_flagChanged+=Add(__FirstAlphas[j], _ptrFirstTemp->Value);
								}
								else{
									_flagEpsFound =1;
								}
								_ptrFirstTemp = _ptrFirstTemp->Next;
							}
							if (_flagEpsFound ==0){
								k= __NonTerminalCount + __TerminalCount + 1;
								j = __NonTerminalCount;
							}
						}
						
					}
				}
			}
		}
	
	}
	
		
}

//returns an array with the production items in the selected row of the grammer in the order they appear in the production. 
//The array will be terminated with a "-1"
//__Grammer: Grammer matrix
//__rowIndex: Production row to return
//__countColumns: Columns in grammer matrix
//__countOutput: The number of valid indices in the output
int * GetProduction(int **__Grammer, int __rowIndex, int __countColumns, int *__countOutput){
	int *_output = (int *)malloc(sizeof(int)*__countColumns);
	for (int i = 0; i < __countColumns; i++){
		_output[i] = GetItemInProduction(__Grammer, __rowIndex, __countColumns, i);
		if (_output[i] ==-1){
			*__countOutput = i;
			break;
		}
	}

	return _output;
}


//Prints an array of integers
void TestPrint(int *__Output, int __RowCount, char *header){
	printf(header);
	for (int i = 0; i<__RowCount; i++){
		printf("%d\n", __Output[i]);
	}

}