#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAXSIZE 100000000


int main(int argc, char* argv[]) {
    int cnt = 0;
    int num = 0;
    char* inputFileName = argv[1];

    /*char* inputFileName = "input_000010000_000010000_000000001.txt";
    int algorithmIndex = 4;*/

    FILE* inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Unable to open input file.\n");
        return 1;
    }
    
    int* arr = NULL;            
    arr = (int*)malloc(sizeof(int) * MAXSIZE);              //동적 할당
        
    while (fscanf(inputFile, "%d", &num) != EOF)            //파일 읽기
    {
        arr[cnt] = num;
        cnt++;
    }
    fclose(inputFile);                                      //파일 닫기

    int i, j;
    int order = -1;
    for (i = 1; i < cnt; i++) {
        int key = arr[i];
        for (j = i; j > 0; j--) {
            if (order * key < order * arr[j - 1]) {
                arr[j] = arr[j - 1];
            }
            else break;
        }
        arr[j] = key;
    }
    

    char outputFileName[100];

    snprintf(outputFileName, sizeof(outputFileName), "reverse_%s",inputFileName);
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error: Unable to open output file.\n");
        return 1;
    }
    /*char outputFileName[] = "result_2_input_000010000_000010000_000000001.txt";
    FILE* outputFile = fopen(outputFileName, "w");*/
    //fprintf(outputFile, "%s\n", inputFileName);                                 //line 1    inputFileName
    //fprintf(outputFile, "%d\n", algorithmIndex);                                //line 2    algorithmIndex
    //fprintf(outputFile, "%d\n", cnt);                                           //line 3    input size (the number of elements in the original list) 
    //fprintf(outputFile, "%lf\n", (double)(end - start) / CLOCKS_PER_SEC);       //line 4    running time in seconds

    for (int i = 0; i < cnt; i++)
    {
        fprintf(outputFile, "%d ", arr[i]);                                  //line 5    the sorted list
    }
    //fprintf(outputFile, "\n최적화");
    fclose(outputFile);
    free(arr);
    arr = NULL;
    return 0;
}

