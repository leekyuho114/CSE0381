#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/******************************************************************************************************/
#include <Windows.h>
#define CHECK_TIME_START(start,freq) QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(start,end,freq,time) QueryPerformanceCounter((LARGE_INTEGER*)&end); time = (float)((float)(end - start) / (freq * 1.0e-3f))

__int64 _start, _freq, _end;
float compute_time;
/******************************************************************************************************/
#define INPUT_FILE_NAME "input_data_rd_1024.bin"
#define OUTPUT_FILE_NAME "output.bin"
int Insertion_sort(unsigned int *data, int left, int right);
int main() {
	FILE* fp;
	unsigned int len;
	errno_t err;
	unsigned int* array;
	srand((unsigned int)time(NULL));
	fp = NULL;
	err = fopen_s(&fp, INPUT_FILE_NAME, "rb");
	if (err!=0) {
		printf("Error opening file: %d\n",err);
		return 1;
	}
	fread(&len, sizeof(unsigned int), 1, fp);
	array = (unsigned int*)malloc(len * sizeof(unsigned int));
	if (array == NULL) {
		printf("memory alloc fail");
		return 1;
	}
	fread(array, sizeof(unsigned int), len, fp);
	CHECK_TIME_START(_start, _freq); // time check start

	if (Insertion_sort(array, 0, len-1)) {
		printf("sorting error");
	}

	CHECK_TIME_END(_start, _end, _freq, compute_time); //time check end

	printf("\n^^^ Time for generating = %.3fms\n\n", compute_time);
	FILE* fp2 = NULL;
	err = fopen_s(&fp2, OUTPUT_FILE_NAME, "wb");
	if (err != 0) {
		printf("Error opening file: %d\n", err);
		return 1;
	}
	for (int i = 0; i < len ; i++) {
		fwrite(&array[i], sizeof(unsigned int), 1, fp); // 4 byte ¾¿ ÆÄÀÏ¿¡ write
	}

	fclose(fp2);
	fclose(fp); 
	free(array);

	return 0;
}
int Insertion_sort(unsigned int* data, int left, int right) {
	int i, j, temp;
	for (i = left + 1; i <= right; i++) {
		temp = data[i];
		j = i - 1;
		while (j >= left && data[j] > temp) {
			data[j + 1] = data[j];
			j--;
		}
		data[j + 1] = temp;
	}
	return 0;
}
