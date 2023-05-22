#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INPUT_FILE_NAME "input_data_rd_1024.bin"
#define OUTPUT_FILE_NAME "output.bin"
/******************************************************************************************************/
#include <Windows.h>
#define CHECK_TIME_START(start,freq) QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(start,end,freq,time) QueryPerformanceCounter((LARGE_INTEGER*)&end); time = (float)((float)(end - start) / (freq * 1.0e-3f))

__int64 _start, _freq, _end;
float compute_time;
/******************************************************************************************************/
void swap(int* a, int* b);
int partition(unsigned int* data, int left, int right);
int Median_Of_Three(unsigned int* data, int left, int right);
int Quick_Sort_P(unsigned int* data, int left, int right);
int main() {
	FILE* fp;
	unsigned int len;
	errno_t err;
	unsigned int* array;
	srand((unsigned int)time(NULL));
	fp = NULL;
	err = fopen_s(&fp, INPUT_FILE_NAME, "rb");
	if (err != 0) {
		printf("Error opening file: %d\n", err);
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

	if (Quick_Sort_P(array, 0, len - 1)) {
		printf("sorting error");
	}

	CHECK_TIME_END(_start, _end, _freq, compute_time); //time check end
	printf("\n^^^ Time for generating = %.3fms\n", compute_time);

	FILE* fp2 = NULL; //output.bin
	err = fopen_s(&fp2, OUTPUT_FILE_NAME, "wb");
	if (err != 0) {
		printf("Error opening file: %d\n", err);
		return 1;
	}
	for (int i = 0; i < len; i++) {
		fwrite(&array[i], sizeof(unsigned int), 1, fp);
	}

	fclose(fp2);
	fclose(fp);
	free(array);

	return 0;
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(unsigned int* data, int left, int right) {
	int i, pivot;
	pivot = left;

	for (int i = left; i < right; i++) {
		if (data[i] < data[right]) {
			swap(&data[i], &data[pivot]);
			pivot++;
		}
	}
	swap(&data[pivot], &data[right]);
	return (pivot);
}
int Median_Of_Three(unsigned int* data, int left, int right) {
	int mid = left + (right - left) / 2;
	if (data[left] > data[mid]) {
		swap(&data[left], &data[mid]);
	}
	if (data[left] > data[right]) {
		swap(&data[left], &data[right]);
	}
	if (data[mid] > data[right]) {
		swap(&data[mid], &data[right]);
	}

	return mid;
}
int Quick_Sort_P(unsigned int* data, int left, int right) {
	if (right - left >= 4) {
		int pivot_idx = Median_Of_Three(data, left, right);
		int pivot = data[pivot_idx];
		swap(&data[pivot_idx], &data[right]);
		int partition_idx = partition(data, left, right);
		Quick_Sort_P(data, left, pivot_idx - 1);
		Quick_Sort_P(data, pivot_idx + 1, right);
	}
	else {
		for (int i = left; i <= right; i++) {
			for (int j = i + 1; j <= right; j++) {
				if (data[j] < data[i]) {
					swap(&data[i], &data[j]);
				}
			}
		}
	}
	return 0;
}