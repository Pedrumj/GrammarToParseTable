// ParseTable.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Matrix.h"
#include <Windows.h>
#include "ParsingTable.h"

/* this is an example for calling the Generate function of the ParsingTable.h file. The input grammer is defined below:
E->TE'
E'->+TE'
E'->Eps
T->FT'
T'->*FT'
T'->Eps
F->(E)
F->id


int main()
{
	//The grammer matrix
	int **Grammer = InitializeMatrix(8, 11);
	//The index of the Nonterminal for each row of the grammer matrix
	int *Rows = (int *)malloc(sizeof(int)*8);
	int **_output;
	//E->TE'
	Grammer[0][1]=1;
	Grammer[0][2]=0;
	Rows[0] =0;
	//E'->+TE'
	Grammer[1][1]=2;
	Grammer[1][2]=1;
	Grammer[1][5]=0;
	Rows[1] =1;
	//E'->Eps
	Grammer[2][10]=0;
	Rows[2] = 1;
	//T->FT'
	Grammer[3][3]=1;
	Grammer[3][4]=0;
	Rows[3] =2;
	//T'->*FT'
	Grammer[4][3]=2;
	Grammer[4][4]=1;
	Grammer[4][6]=0;
	Rows[4] =3;
	//T'->Eps
	Grammer[5][10]=0;
	Rows[5] = 3;
	//F->(E)
	Grammer[6][0]=1;
	Grammer[6][8]=0;
	Grammer[6][9]=2;
	Rows[6] =4;
	//F->id
	Grammer[7][7]=0;
	Rows[7] =4;


	_output=Generate(Grammer, Rows, 8, 5, 5);
	PrintMat(_output, 5, 6);
	return 0;
}

