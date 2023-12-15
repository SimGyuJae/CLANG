#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//int sum = 0;
//int N6algorithm(FILE*, int, int);
//int N4algorithm(FILE*, int, int);
//int N3algorithm(FILE*, int, int);

int main(int argc, char* argv[]) {//input12345 1
	if (argc != 3) {
		fprintf(stderr,"Usage: %s <input_filename> <algorithm_index>\n");
		return 1;
	}
	char* inputFilename = argv[1];
	int algorithmIndex = atoi(argv[2]);

	int row;
	int col;

	FILE* pFileInput = fopen(inputFilename, "r");
	if (pFileInput == NULL)
	{
		printf("The input file does not exist.\n");
		fclose(pFileInput);
		return 0;
	}

	//FILE* pFileResult = fopen("return_%s", argv[1], "w");

	//pFileInput = fopen("input00001.txt", "r");
	//

	//fscanf(pFileInput, "%d %d", &row, &col);
	


	//clock_t start_time = clock();//Ω√¿€
	//atoi(argv[2])
	//int x = 1;
	//switch (x)
	//{
	//case 1:
	//	sum = N6algorithm(pFileInput, row, col);
	//	break;
	//case 2:
	//	sum = N4algorithm(pFileInput, row, col);
	//	break;
	//case 3:
	//	sum = N3algorithm(pFileInput, row, col);
	//	break;

	//default:
	//	break;
	//}
	//
	//
	//clock_t end_time = clock();


	//fprintf(pFileResult, "%s\n", "input00001.txt");//line1
	//fprintf(pFileResult, "%d\n", 1);//line2
	//fprintf(pFileResult, "%d\n", row);	//line3
	//fprintf(pFileResult, "%d\n", col);	//line4
	//fprintf(pFileResult, "%d\n", sum);	//line5
	//fprintf(pFileResult,"%.1f",(double)(end_time - start_time)/CLOCKS_PER_SEC);//line6
	
	fclose(pFileInput);
	//fclose(pFileResult);
	return;
}

//int N6algorithm(FILE* pFileInput, int row, int col) {
//	int sum = 0;
//	for (int i = 0; i < row; i++) {
//		for (int j = 0; j < col; j++) {
//
//		}
//	}
//	return sum;
//
//}
//
//int N4algorithm(FILE* pFileInput, int row, int col) {
//	int sum = 0;
//	return sum;
//}
//
//int N3algorithm(FILE* pFileInput, int row, int col) {
//	int sum = 0;
//	return sum;
//}