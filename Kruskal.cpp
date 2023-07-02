#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
typedef struct {
    int src;
    int dest;
    int weight;
}Edge;

typedef struct {
    int V;
    int E;
    Edge* edgeList;
}Graph;

typedef struct {
    int parent;
    int rank;
    int64_t total_weight;
    int numV;
}Subset;

typedef struct {
    Edge* heapList;
    int maxSize;
    int size;
}Minheap;
//MST
Graph* create_graph(int V, int E);
void findMST(const char* outPath, Graph* graph);
//disjoint set
int Find(Subset* subsets, int x);
void Union(Subset* subsets, int a, int b, int64_t weight);
//min heap
Minheap* create_minheap(int maxSize);
void insertEdge(Minheap* minheap, Edge edge);
Edge deleteEdge(Minheap* minheap);
void heapify(Minheap *minheap, int idx);
void swapEdges(Edge* x, Edge* y);
int main() {
    FILE* fp;
    char path[256]; // file path
    char input_f[256]; // input file name
    char output_f[256]; // output file name
    char inPath[512]; // input path
    char outPath[512]; // output path
    int n_vertices, n_edges, max_weight;

    //commands.txt ó��
    fopen_s(&fp, "commands.txt", "r");
    if (fp == NULL) {
        printf("Failed to open commands file.\n");
        return 1;
    }
    if (fgets(path, sizeof(path), fp) == NULL) {
        printf("Failed to read file.\n");
        fclose(fp);
        return 1;
    }
    if (path[strlen(path) - 1] == '\n') { // ���� ����
        path[strlen(path) - 1] = '\0';
    }
    if (fgets(input_f, sizeof(input_f), fp) == NULL) {
        printf("Failed to read file.\n");
        fclose(fp);
        return 1;
    }
    if (input_f[strlen(input_f) - 1] == '\n') { // ���� ����
        input_f[strlen(input_f) - 1] = '\0';
    }
    if (fgets(output_f, sizeof(output_f), fp) == NULL) {
        printf("Failed to read file.\n");
        fclose(fp);
        return 1;
    }
    if (output_f[strlen(output_f) - 1] == '\n') { // ���� ����
        output_f[strlen(output_f) - 1] = '\0';
    }
    fclose(fp);

    //graph ���� read 
    sprintf_s(inPath, "%s\\%s", path, input_f);
    //sprintf_s(outPath, "%s", output_f);
    sprintf_s(outPath, "%s\\%s", path, output_f);
    FILE* ip;
    fopen_s(&ip, inPath, "r");
    if (ip == NULL) {
        printf("Failed to open input file.\n");
        return 1;
    }
    char line[256];
    if (fgets(line, sizeof(line), ip) != NULL) {
        sscanf_s(line, "%d %d %d", &n_vertices, &n_edges, &max_weight);
    }
    //graph ����
    int j = 0;
    Graph* graph = create_graph(n_vertices, n_edges);
    while (fgets(line, sizeof(line), ip) != NULL) {
        sscanf_s(line, "%d %d %d", &graph->edgeList[j].src , &graph->edgeList[j].dest, &graph->edgeList[j].weight);
        j++;
    }
    fclose(ip);
    /* // �ּ�ó��
    clock_t start_time, end_time;
    double execution_time;
    start_time = clock();
    */
    findMST(outPath, graph);
    /*
    end_time = clock();
    execution_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("time : %.2f\n", execution_time);
    */
    free(graph->edgeList);
    free(graph);
    return 0;
}

Graph* create_graph(int V, int E) { //graph_init
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edgeList = (Edge*)malloc(E*sizeof(Edge));
    return graph;
}
void findMST(const char* outPath, Graph* graph) {
    int connected = 0;
    int V = graph->V;
    int E = graph->E;
    Edge* edgeArr = graph->edgeList;
    Minheap* minheap = create_minheap(E);
    for (int i = 0; i < E; i++) {
        insertEdge(minheap, edgeArr[i]);
    }
    Subset* subsets;
    subsets = (Subset*)malloc(graph->V * sizeof(Subset));
    for (int i = 0; i < V; i++) {
        subsets[i].parent = i;
        subsets[i].rank = 0;
        subsets[i].total_weight = 0; // parent�� total_weight ����, connected component�� ����
        subsets[i].numV = 1;
    }
    int edgeCnt = 0;
    int kscd = 0;
    long long int totalWeight = 0;
    int verNum = 0;
    while (edgeCnt < V - 1 && minheap->size >0) { //kruskal
        Edge nextE = deleteEdge(minheap);
        int x = Find(subsets, nextE.src);
        int y = Find(subsets, nextE.dest);
        kscd += 1;
        if (x != y) {
            edgeCnt++;
            Union(subsets, x, y, nextE.weight);
        }
    }
    
    for (int i = 0; i < V; i++) {
        if (subsets[i].parent == i) { // disjoint set���� root �� ���
            connected += 1;
        }
    }
    FILE* op;
    fopen_s(&op, outPath, "w");
    if (op == NULL) {
        printf("Failed to open output file. %d\n",errno);
        perror("���� : ");
        return;
    }
    fprintf_s(op, "%d\n", connected);
    for (int i = 0; i < V; i++) {
        if (subsets[i].parent == i) { // disjoint set���� root �� ���
            fprintf_s(op,"%d %lld\n", subsets[i].numV, subsets[i].total_weight);
        }
    }
    //printf("kscanned : %d\n",kscd);//�ּ�ó��
    
    fclose(op);
    free(subsets);
}
int Find(Subset* subsets , int x) {// ��ξ���
    if (x == subsets[x].parent) {
        return x;
    }
    else {
        subsets[x].parent = Find(subsets, subsets[x].parent);
        return subsets[x].parent;
    }
}

void Union(Subset* subsets, int a, int b, int64_t weight) {
    int aRoot = Find(subsets, a);
    int bRoot = Find(subsets, b);
    if (subsets[aRoot].rank > subsets[bRoot].rank) {// �� ����� parent�� ���� ��� ����ó���� MST �Լ����� ��
        subsets[bRoot].parent = aRoot;
        subsets[aRoot].total_weight += subsets[bRoot].total_weight; // �� subset total weight ����
        subsets[aRoot].total_weight += weight; //�����߰��� edge weight ����
        subsets[aRoot].numV += subsets[bRoot].numV; // set�� ���Ե� vertex ���� ������Ʈ
    }
    else if (subsets[aRoot].rank < subsets[bRoot].rank) {
        subsets[aRoot].parent = bRoot;
        subsets[bRoot].total_weight += subsets[aRoot].total_weight;
        subsets[bRoot].total_weight += weight;
        subsets[bRoot].numV += subsets[aRoot].numV;
    }
    else {
        subsets[bRoot].parent = aRoot;
        subsets[aRoot].rank++;
        subsets[aRoot].total_weight += subsets[bRoot].total_weight;
        subsets[aRoot].total_weight += weight;
        subsets[aRoot].numV += subsets[bRoot].numV;
    }
}

Minheap* create_minheap(int maxSize) {
    Minheap* minheap = (Minheap*)malloc(sizeof(Minheap));
    minheap->maxSize = maxSize;
    minheap->size = 0;
    minheap->heapList = (Edge*)malloc(maxSize * sizeof(Edge));
    return minheap;
}

void insertEdge(Minheap* minheap, Edge edge) {
    minheap->size++;
    int i = minheap->size - 1;
    while (i > 0 && edge.weight < minheap->heapList[(i - 1) / 2].weight) {//parent�� ��, target index i�� ����
        minheap->heapList[i] = minheap->heapList[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minheap->heapList[i] = edge;
}

Edge deleteEdge(Minheap* minheap) {
    Edge min = minheap->heapList[0];//������ ��带 root�� �ø��� heapify
    minheap->heapList[0] = minheap->heapList[minheap->size-1];
    minheap->size--;
    heapify(minheap, 0);
    return min;
}

void heapify(Minheap* minheap, int idx) {//heap���� ��������
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    // left right �� �� ���� ���� swap, recusive�ϰ� ȣ��
    if (left < minheap->size && minheap->heapList[left].weight < minheap->heapList[smallest].weight) {
        smallest = left;
    }
    if (right < minheap->size && minheap->heapList[right].weight < minheap->heapList[smallest].weight) {
        smallest = right;
    }
    if (smallest != idx) {// idx change -> swap and recall
        swapEdges(&(minheap->heapList[idx]), &(minheap->heapList[smallest]));
        heapify(minheap, smallest);
    }
}
void swapEdges(Edge* x, Edge* y) {
    Edge temp = *x;
    *x = *y;
    *y = temp;
}