#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#define MAXSIZE 100000000

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void quick_sort(int start, int end, int arr[]) {
    if (start >= end) return;
    int pivot = end, low = start, ptr = start;
    while (ptr < pivot) {
        if (arr[ptr] < arr[pivot]) {
            if (low != ptr) {
                swap(&arr[low], &arr[ptr]);
            }
            low++;
        }
        ptr++;
    }
    if (low != pivot) {
        swap(&arr[low], &arr[pivot]);
    }
    quick_sort(start, low - 1, arr);
    quick_sort(low + 1, end, arr);
}

void merge_sort(int start, int end, int arr[], int tmp[]) {
    if (start >= end) return;
    int mid = (start + end) / 2;

    merge_sort(start, mid, arr, tmp);
    merge_sort(mid + 1, end, arr, tmp);

    int idx1 = start, idx2 = mid + 1, idxtmp = start;
    while (idx1 <= mid && idx2 <= end) {
        if (arr[idx1] < arr[idx2]) {
            tmp[idxtmp++] = arr[idx1++];
        }
        else {
            tmp[idxtmp++] = arr[idx2++];
        }
    }
    while (idx1 <= mid) {
        tmp[idxtmp++] = arr[idx1++];
    }
    while (idx2 <= end) {
        tmp[idxtmp++] = arr[idx2++];
    }

    int i;
    for (i = start; i <= end; i++) {
        arr[i] = tmp[i];
    }
}


void OPTIMIZEDquick_sort(int start, int end, int arr[]) {
    if (start >= end) return;
    arr[end] = 0;
    int pivot = end, low = start, ptr = start;

    while (ptr < pivot) {
        if (arr[ptr] < arr[pivot]) {
            if (low != ptr) {
                swap(&arr[low], &arr[ptr]);
            }
            low++;
        }
        ptr++;
    }
    if (low != pivot) {
        swap(&arr[low], &arr[pivot]);
    }
    quick_sort(start, low - 1, arr);
    quick_sort(low + 1, end, arr);
}

int* Sort(int* arr, int cnt, int algorithmIndex) {
    

    if (algorithmIndex == 1) {//INSERTION SORT
        int i, j;
        int order = 1;
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
        

    } else if (algorithmIndex == 2) {//QUICK SORT
        quick_sort(0,cnt - 1, arr);

    } else if (algorithmIndex == 3) {//MERGE SORT
        int tmp[30000];
        merge_sort(0, cnt - 1, arr, tmp);
    } else if (algorithmIndex == 4) {//OPTI SORT
        OPTIMIZEDquick_sort(0, cnt - 1, arr);
    }
    
    
    return arr;
}

int main(int argc, char* argv[]) {
    int cnt = 0;
    int num = 0;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_filename> <algorithm_index>\n", argv[0]);
        return 1;
    }
    char* inputFileName = argv[1];
    int algorithmIndex = atoi(argv[2]);
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

    clock_t start = clock();                                //측정 시작
    int* sorted = Sort(arr, cnt, algorithmIndex);           //알고리즘
    clock_t end = clock();                                  //측정 종료

    char outputFileName[100];

    snprintf(outputFileName, sizeof(outputFileName), "result_%d_%s",algorithmIndex,inputFileName);
    FILE* outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error: Unable to open output file.\n");
        return 1;
    }
    /*char outputFileName[] = "result_2_input_000010000_000010000_000000001.txt";
    FILE* outputFile = fopen(outputFileName, "w");*/
    fprintf(outputFile, "%s\n", inputFileName);                                 //line 1    inputFileName
    fprintf(outputFile, "%d\n", algorithmIndex);                                //line 2    algorithmIndex
    fprintf(outputFile, "%d\n", cnt);                                           //line 3    input size (the number of elements in the original list) 
    fprintf(outputFile, "%lf\n", (double)(end - start) / CLOCKS_PER_SEC);       //line 4    running time in seconds

    for (int i = 0; i < cnt; i++)
    {
        fprintf(outputFile, "%d ", sorted[i]);                                  //line 5    the sorted list
    }
    //fprintf(outputFile, "\n최적화");
    fclose(outputFile);
    free(arr);
    arr = NULL;
    return 0;
}

