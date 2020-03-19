#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <ctime>
#define SIZE 12
#define TIMES 1000000

int C_Only(short* array1, short* array2, short* arrayResult);
int Assembly_Only(short* array1, short* array2, short* arrayResult);
int AssemblyMMX_Only(short* array1, short* array2, short* arrayResult);
void init(short* array) {
	for (int i = 0; i < SIZE; i++) {
		array[i] = rand() % SIZE + 0;
		printf(" %d |", array[i]);
	}
	printf("\t\t\tDONE!");
}
short* zeroing(short* arr) {
	for (int i = 0; i < SIZE; i++) {
		arr[i] = 0;
	}
	return arr;
}
void chekArr(short* arr) {
	for (int i = 0; i < SIZE; i++) {
		printf("%d|", arr[i]);
	}
	printf("\n");
}

int main() {
	srand(time(0));
	short* mas1 = new short[SIZE];
	short* mas2 = new short[SIZE];
	short* masResult = new short[SIZE];
	printf("-------| INITIALIZATION OF THE FIRST ARRAY |-------\n\n");
	init(mas1);
	printf("\n\n-------| INITIALIZATION OF THE SECOND ARRAY |-------\n\n");
	init(mas2);

//For 'C' code
	printf("\n\n\n\n1) RESULT FOR 'C' CODE\n\n");
	printf("\t\t\t C code runtime: %d (millisec)", C_Only(mas1, mas2, masResult));
	printf("\n-----------------------------------------------------------------");
//For 'Assembly' code
	printf("\n\n2) RESULT FOR 'Assembly' CODE\n\n");
	masResult = zeroing(masResult);
	printf("\t\t\t Assembly code runtime: %d (millisec)", Assembly_Only(mas1, mas2, masResult));
	printf("\n-----------------------------------------------------------------");
//For 'Assembly MMX' code
	printf("\n\n3) RESULT FOR 'Assembly MMX' CODE\n\n");
	masResult = zeroing(masResult);
	printf("\t\t\t Assembly MMX code runtime: %d (millisec)", AssemblyMMX_Only(mas1, mas2, masResult));
	printf("\n-----------------------------------------------------------------");
//Finish
	printf("\n\nPress any key ->");
	getchar();
	return 0;
}

int C_Only(short* array1, short* array2, short* arrayResult) {
	unsigned int start_time = clock(); // начальное время
	for (int count = 0; count < TIMES; count++) {
		for (int i = 0; i < SIZE; i++) {
			arrayResult[i] = std::min(array1[i], array2[i]);
			//printf("%d, ", arrayResult[i]);
		}
		//printf("\n\n");
		//chekArr(arrayResult);
	}
	unsigned int end_time = clock(); // конечное время
	unsigned int result_time = (end_time - start_time); // искомое время
	chekArr(arrayResult);
	return result_time;
}

int Assembly_Only(short* array1, short* array2, short* arrayResult) {
	unsigned int start_time = clock(); // начальное время
	int sizeArr;
	for (int count = 0; count < TIMES; count++) {
		sizeArr = SIZE;
		_asm
		{
			PUSHA
			XOR ESI, ESI
			XOR ECX, ECX

			L1:
				MOV EDI, array1
				MOV DX, WORD PTR[ESI + EDI]

				MOV EDI, array2
				MOV BX, WORD PTR[ESI + EDI]

				CMP DX, BX
				JGE L2
				MOV EDI, arrayResult
				MOV WORD PTR[ESI + EDI], DX

				ADD ESI, 2
				SUB sizeArr, 1
				JNZ L1
				JMP L3

			L2:
				MOV EDI, arrayResult
				MOV WORD PTR[ESI + EDI], BX
				ADD ESI, 2
				SUB sizeArr, 1
				JNZ l1

			L3 :
				POPA
		}
	}
	unsigned int end_time = clock(); // конечное время
	unsigned int result_time = (end_time - start_time); // искомое время
	chekArr(arrayResult);
	return result_time;
}

int AssemblyMMX_Only(short* array1, short* array2, short* arrayResult) {
	unsigned int start_time = clock(); // начальное время
	int sizeArr;
	for (int count = 0; count < TIMES; count++) {
		sizeArr = SIZE;
		_asm
		{
				PUSHA

				XOR ESI, ESI
				PXOR MM2, MM2

			L1 :
				MOV EDI, array1
				MOVQ MM0, [ESI + EDI]

  				MOV EDI , array2
				MOVQ MM1, [ESI + EDI]

				PMINUB MM0,MM1
				MOV EDI, arrayResult
				MOVQ[ESI + EDI], MM0

				ADD ESI, 8
				SUB sizeArr, 4
				JNZ L1
				
				EMMS
				POPA
		}
	}
	unsigned int end_time = clock(); // конечное время
	unsigned int result_time = (end_time - start_time); // искомое время
	chekArr(arrayResult);
	return result_time;
}
