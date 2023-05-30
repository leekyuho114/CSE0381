#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_FILENAME_LEN 256
#define MAX_INPUT 201
#define INF 1e9
int index; // chords 배열 index(for recursion)
typedef struct Point {
    double x;
    double y;
} Point;
int minimal_triangle(char* input_file, char* output_file);
double dist(Point p1, Point p2, int a, int b); 
double min(double a, double b);
void find_chord(int ** index_dp, int **chords, int i, int s);
int compare(const void* x, const void* y);
int main() {
	int file_num;
	FILE* fp;
    fopen_s(&fp, "PT_test_command.txt", "r");
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
        minimal_triangle(input_files[i], output_files[i]);
    }
    for (int i = 0; i < file_num; i++) {
        free(input_files[i]);
        free(output_files[i]);
    }
    free(input_files);
    free(output_files);
	return 0;
}

int minimal_triangle(char* input_file, char* output_file) {
    int vertex_num;
    FILE* fp;
    fopen_s(&fp, input_file, "r");
    if (fp == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }
    fscanf_s(fp, "%d", &vertex_num);
    //2차원 table 동적할당
    double** dp = (double**)malloc(MAX_INPUT * sizeof(double*)); 
    for (int i = 0; i < MAX_INPUT; i++) {
        dp[i] = (double*)malloc(MAX_INPUT * sizeof(double));
    }
    //chord print를 위한 k값 저장 table
    int** index_dp = (int**)malloc(MAX_INPUT * sizeof(int*));
    for (int i = 0; i < MAX_INPUT; i++) {
        index_dp[i] = (int*)malloc(MAX_INPUT * sizeof(int));
    }
    //chords 저장 array
    int** chords = (int**)malloc((vertex_num-3) * sizeof(int*));
    for (int i = 0; i < vertex_num - 3; i++) {
        chords[i] = (int*)malloc(2 * sizeof(int));
    }
    // vertex 배열 points
    Point* points = (Point*)malloc(vertex_num * sizeof(Point)); 
    
    for (int i = 0; i < vertex_num; i++) {
        fscanf_s(fp, "%lf %lf", &points[i].x, &points[i].y);
    }
    fclose(fp);


    for (int s = 0; s < 4; s++) {
        for (int i = 0; i <= vertex_num - s; i++) {
            dp[i][s] = 0;
        }
    }

    for (int s = 4; s <= vertex_num; s++) {
        for (int i = 0; i <= vertex_num - s; i++) {
            dp[i][s] = INFINITY;
            for (int k = 1; k <= s - 2; k++) {
                double temp = dp[i][k + 1] + dp[i + k][s - k]
                    + dist(points[i], points[i + k], i, i + k) + dist(points[i + k], points[i + s - 1], i + k, i + s - 1);
                if (temp < dp[i][s]) {
                    dp[i][s] = temp;
                    index_dp[i][s] = k;
                }
            }
        }
    }
    index = 0;
    find_chord(index_dp, chords,0, vertex_num);
    qsort(chords, index, sizeof(int*), compare);
    
    //output text file write
    FILE* fp_out;
    fopen_s(&fp_out, output_file, "w");
    if (fp_out == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }
    fprintf_s(fp_out, "%lf\n", dp[0][vertex_num]);
    for (int i = 0; i < index; i++) {
        fprintf_s(fp_out,"%d %d\n", chords[i][0], chords[i][1]);
    }
    fclose(fp_out);

    free(points);
    for (int i = 0; i < MAX_INPUT; i++) {
        free(dp[i]);
    }
    free(dp);

    for (int i = 0; i < MAX_INPUT; i++) {
        free(index_dp[i]);
    }
    free(index_dp);
    
    for (int i = 0; i < vertex_num-3; i++) {
        free(chords[i]);
    }
    free(chords);
    return 0;
}
double dist(Point p1, Point p2, int a, int b) {
    if (a == b + 1 || a == b - 1) {// 만약 인접 point면 0 return
        return 0;
    }
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}
double min(double a, double b) {
    return a < b ? a : b;
}
void find_chord(int** index_dp, int** chords, int i, int s) {//n은 chord배열 inex
    int v1_i, v1_s, v2_i, v2_s;
    v1_i = i;
    v1_s = index_dp[i][s] + 1;
    v2_i = i + index_dp[i][s];
    v2_s = s - index_dp[i][s];
    if (!(v1_s == 2 || v1_s == 3)) { // v2의 s가 2,3 이 아닌 경우
        chords[index][0] = v1_i;
        chords[index][1] = v1_i + v1_s - 1;
        index++;
        find_chord(index_dp, chords, v1_i, v1_s);
    }
    else if (v1_s == 3) {// v2의 s가 3인 경우 (2는 무시)
        chords[index][0] = v1_i;
        chords[index][1] = v1_i + v1_s - 1;
        index++;
    }
    if (!(v2_s == 2 || v2_s == 3)) { // v2의 s가 2,3 이 아닌 경우
        chords[index][0] = v2_i;
        chords[index][1] = v2_i + v2_s - 1;
        index++;
        find_chord(index_dp, chords,v2_i, v2_s);
    }
    else if (v2_s == 3) {// v2의 s가 3인 경우 (2는 무시)
        chords[index][0] = v2_i;
        chords[index][1] = v2_i + v2_s - 1;
        index++;
    }
}
int compare(const void* x, const void* y) {
    int* v1 = *(int**)x;
    int* v2 = *(int**)y;

    // 먼저 x 값에 대해 오름차순으로 정렬
    if (v1[0] < v2[0]) {
        return -1;
    }
    else if (v1[0] > v2[0]) {
        return 1;
    }

    // x 값이 같은 경우, y 값에 대해 오름차순으로 정렬
    if (v1[1] < v2[1]) {
        return -1;
    }
    else if (v1[1] > v2[1]) {
        return 1;
    }

    return 0;
}
