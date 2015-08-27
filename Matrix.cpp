#include "stdafx.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>


//Prints the input matrix
void PrintMat(int **__matInput, int __RowCount, int __ColCount){
	char *_strPrint = (char *)malloc(sizeof(char) * 1000);
	char *_num2str =  (char *)malloc(sizeof(char) * 10);
	_num2str[0] = '\0';
	_strPrint[0] = '\0';
	for (int i = 0; i < __RowCount;i++){
		for (int j =0; j<__ColCount;j++){
			
			printf("%4d", __matInput[i][j]);
		}
		printf("\n");
	}
}

//Creates a dynamic 2d matrix and sets all the elements to -1
int **InitializeMatrix(int __RowCount, int __ColCount){
	int **_matOutput = (int **)malloc(sizeof(int *)*__RowCount);
	for (int i = 0; i<__ColCount; i++){
		_matOutput[i] = (int *)malloc(sizeof(int )*__ColCount);
	}
	for (int i = 0; i< __RowCount; i++){
		for (int j = 0; j < __ColCount; j ++){
			_matOutput[i][j] = -1;
		}
	}
	return _matOutput;
}