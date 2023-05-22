#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INPUT_FILE_NAME "input_data_rd_32.bin"
#define OUTPUT_FILE_NAME "output.bin"
/******************************************************************************************************/
#include <Windows.h>
#define CHECK_TIME_START(start,freq) QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(start,end,freq,time) QueryPerformanceCounter((LARGE_INTEGER*)&end); time = (float)((float)(end - start) / (freq * 1.0e-3f))

__int64 _start, _freq, _end;
float compute_time;
/******************************************************************************************************/
unsigned int* temp;
int Merge_sort(unsigned int* data, int left, int right);
void merge(unsigned int* data, int left, int middle, int right);
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

	if (Merge_sort(array, 0, len-1)) {
		printf("sorting error");
	}

	CHECK_TIME_END(_start, _end, _freq, compute_time); //time check end
	printf("\n^^^ Time for generating = %.3fms\n\n", compute_time);

	
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
int Merge_sort(unsigned int* data, int left, int right) {
	int middle;
	if (left < right) {
		middle = left + (right - left) / 2;

		Merge_sort(data, left, middle);
		Merge_sort(data, middle + 1, right);

		merge(data, left, middle, right);
	}
	return 0;
}
void merge(unsigned int* data, int left, int middle, int right) {
	int i = left, j = middle + 1, k = 0;
	temp = (unsigned int*)malloc(sizeof(unsigned int) * (right - left + 1));
	while (i <= middle && j <= right) { //merge 여기서 정렬
		if (data[i] <= data[j]) {
			temp[k] = data[i];
			k++;
			i++;
		}
		else {
			temp[k] = data[j];
			k++;
			j++;
		}
	}
	while (i <= middle) { // 남은 부분 삽입
		temp[k] = data[i];
		k++;
		i++;
	}
	while (j <= right) {
		temp[k] = data[j];
		k++;
		j++;
	}
	for (i = left; i <= right; i++) { //data에 삽입
		data[i] = temp[i - left];
	}
	free(temp);
}
