#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LEN 256
#define MAX_INPUT 201
int subset_sum(char* input_file, char* output_file);
int compare(const void* a, const void* b);
int main() {
    int file_num;
    FILE* fp;
    fopen_s(&fp, "SS_test_command.txt", "r");
    if (fp == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }
    fscanf_s(fp, "%d", &file_num);

    char** input_files = (char**)malloc(file_num * sizeof(char*));
    char** output_files = (char**)malloc(file_num * sizeof(char*));

    for (int i = 0; i < file_num; i++) {
        input_files[i] = (char*)malloc(MAX_FILENAME_LEN * sizeof(char));
        output_files[i] = (char*)malloc(MAX_FILENAME_LEN * sizeof(char));
        fscanf_s(fp, "%s %s", input_files[i], MAX_FILENAME_LEN - 1, output_files[i], MAX_FILENAME_LEN - 1);
    }
    fclose(fp);

    // Check if the filenames were correctly read
    for (int i = 0; i < file_num; i++) {
        subset_sum(input_files[i], output_files[i]);
    }
    for (int i = 0; i < file_num; i++) {
        free(input_files[i]);
        free(output_files[i]);
    }
    free(input_files);
    free(output_files);
    return 0;
}

int subset_sum(char* input_file, char* output_file) {
    int n; // input 개수
    int L=0; // 목표 sum
    int subsetSize = 0; // 결과 1일 시, subset size 저장
    FILE* fp;//input file read
    fopen_s(&fp , input_file, "r");
    if (fp == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }
    fscanf_s(fp, "%d", &n);
    // 배열 input
    int* arr = (int*)malloc(n * sizeof(int));
    //정보읽기
    for (int i = 0; i < n; i++) {
        fscanf_s(fp, "%d", &arr[i]);
    }
    fscanf_s(fp, "%d", &L);
    fclose(fp);
    //2차원 table 동적할당
    bool ** dp = (bool**)malloc((n+1) * sizeof(bool*));
    for (int i = 0; i < n+1; i++) {
        dp[i] = (bool*)malloc((L+1) * sizeof(bool));
    }
    //결과 index 저장 array 동적할당
    int* result = (int*)malloc((L + 1) * sizeof(int));
    //dp
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true;
    }
    for (int j = 1; j <= L; j++) {
        dp[0][j] = false;
    }
    for (int i= 1; i <= n; i++) {
        for (int j = 1; j <= L; j++) {
            dp[i][j] = dp[i - 1][j];
            if (j  >= arr[i - 1]) {
                dp[i][j] = dp[i][j] || dp[i - 1][j - arr[i - 1]];
            }
        }
    }
    //subset sum problem result 저장
    int i = n, j = L;
    while (i > 0 && j > 0) {
        if (dp[i - 1][j]) {
            i--;
        }
        else {
            result[subsetSize++] = i - 1; //index 저장
            j -= arr[i - 1];
            i--;
        }
    }
    qsort(result, subsetSize, sizeof(int), compare);
    //output text file write
    FILE* fp_out;
    fopen_s(&fp_out, output_file, "w");
    if (fp_out == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }
    fprintf_s(fp,"%d\n", dp[n][L]);
    if (dp[n][L]) {
        fprintf_s(fp, "%d\n", subsetSize);
        for (int i = 0; i < subsetSize; i++) {
            fprintf_s(fp, "%d\n", result[i]);
        }
    }
    fclose(fp_out);
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
    free(arr);
    free(result);
    return 0;
}
int compare(const void* a, const void* b) {
    int num1 = *(int*)a;
    int num2 = *(int*)b;
    if (num1 < num2)
        return -1;
    if (num1 > num2)
        return 1;
    return 0;
}