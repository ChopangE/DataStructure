#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);
void checkIndegree(Graph* G, int* idg);
void sort_node(int* arr, int n);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);							//nodes에 숫자배열이 들어가게된다.

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
	Topsort(G);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}

void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}
Graph *CreateGraph(int *nodes, int n) {
	Graph *graph;														//graph 초기화
	graph = (Graph *)malloc(sizeof(Graph));
	sort_node(nodes, n);											//bubble_sort알고리즘을 통해 nodes를 정렬.
	graph->node = nodes;
	graph->size = n;
	graph->matrix = (int **)malloc(sizeof(int *) * graph->size);			//matrix가 이중포인터이므로 두번에 걸쳐서 malloc진행
	for ( int i = 0; i < n; i++ )
	{
		graph->matrix[i] = (int *)malloc(sizeof(int) * graph->size);
	}
	for ( int i = 0; i < n; i++ )
	{
		for ( int j = 0; j < n; j++ )
		{
			graph->matrix[i][j] = 0;										//matrix의 모든 요소들을 0으로 초기화
		}
	}
	return graph;
}
void InsertEdge(Graph *G, int a, int b) {
	int i, j;
	for ( i = 0; i < G->size; i++ )
	{
		if ( G->node[i] == a )break;							//a와 b의 node에서의 index를 찾은다음 matrix의 해당 index를 1로 설정.
	}
	for ( j = 0; j < G->size; j++ )
	{
		if ( G->node[j] == b )break;
	}
	G->matrix[i][j] = 1;
}
void PrintGraph(Graph *G) {																//단순한 출력의 과정
	fprintf(fout, "   ");
	for ( int i = 0; i < G->size; i++ )
	{
		if ( i == G->size - 1 )
		{
			fprintf(fout, "%d\n", G->node[i]);
			break;
		}
		fprintf(fout, "%d  ", G->node[i]);
	}
	for ( int i = 0; i < G->size; i++ )
	{
		fprintf(fout, "%d  ", G->node[i]);
		for ( int j = 0; j < G->size; j++ ){
			if ( j == G->size - 1 ){
				fprintf(fout, "%d\n", G->matrix[i][j]);
			}
			else fprintf(fout, "%d  ", G->matrix[i][j]);
		}
	}
	fprintf(fout, "\n");
}
void DeleteGraph(Graph *G) {
	for ( int i = 0; i < G->size; i++ )											//matrix가 이중포인터이므로 두번에 걸쳐 free한다 나머지는 평소와 같이 free진행
	{
		free(G->matrix[i]);
	}
	free(G->matrix);
	free(G->node);
	free(G);
}
void Topsort(Graph *G) {
	Queue *que;
	int *idg;
	int *lst;
	int v,i,check_cycle;
	int cur = 0;
	check_cycle = G->size;
	idg = (int *)malloc(sizeof(int) * G->size);					//indegree의 정보를 담은 배열
	lst = (int *)malloc(sizeof(int) * G->size);					//dequeue순서대로 이 배열에 삽입. 출력을 위해 따로 설정해놓은 배열
	for (  i = 0; i < G->size; i++ ) idg[i] = 0;
	que = MakeNewQueue(G->size);
	checkIndegree(G, idg);
	for ( i = 0; i < G->size; i++ )
	{
		if ( idg[i] == 0 )
		{
			Enqueue(que, G->node[i]);
			idg[i]--;
		}
	}
	if(IsEmpty(que)){	
		fprintf(fout,"sorting error : cycle!\n");
		DeleteQueue(que);
		free(lst);
		free(idg);
		return;
	}
	while ( !IsEmpty(que) )
	{
		v = Dequeue(que);
		check_cycle--;
		for ( i = 0; i < G->size; i++ )
		{
			if ( G->node[i] == v )break;						//단순 index찾기
		}
		lst[cur++] = v;
		for ( int j = 0; j < G->size; j++ )
		{
			if ( G->matrix[i][j] == 1 )idg[j]--;				//indegree값 조정
			if ( idg[j] == 0 ) { 
				Enqueue(que, G->node[j]);						//indegree값이 0이 됐으면 enqueue진행
				idg[j]--;										//이미 enqueue를 겪은 key의 indegree는 -1이 된다.
			}
		}
		if ( IsEmpty(que) && (check_cycle > 0 ))						//cycle이 존재하는 상황
		{
			fprintf(fout,"sorting error : cycle!\n");
			DeleteQueue(que);
			free(lst);
			free(idg);
			return;
		}

	}
	for ( int j = 0; j < G->size; j++ )									//cycle이 존재하지 않는 경우 lst의 모든 요소들을 차례대로 출력.
	{
		if ( j == G->size - 1 )
		{
			fprintf(fout, "%d\n", lst[j]);
		}
		else fprintf(fout, "%d ", lst[j]);
	}
	DeleteQueue(que);
	free(lst);
	free(idg);
}
Queue *MakeNewQueue(int X) {							//새로운 queue를 생성하는 함수, 평범하게 초기화를 진행한다.
	Queue *que;
	que = (Queue *)malloc(sizeof(Queue));
	que->max_queue_size = X;
	que->first = 0;
	que->rear = 0;
	que->qsize = 0;
	que->key = (int *)malloc(sizeof(int) * X);
	return que;
}
int IsEmpty(Queue *Q) {
	if ( (Q->first == Q->rear) && Q->qsize == 0 ) return 1;				//first와 rear가 같으면 아직 full인지 empty인지모르지만 size가 0이면 empty이다.
	else return 0;
}
int Dequeue(Queue *Q) {														//단순한 dequeue과정 원형 queue로 구현하였다. 
	int t;
	t = Q->key[Q->rear++];
	Q->rear = Q->rear % Q->max_queue_size;
	Q->qsize--;
	return t;
}
void Enqueue(Queue *Q, int X) {												//단순한 enqueue과정
	Q->key[Q->first++] = X;
	Q->first = Q->first % Q->max_queue_size;
	Q->qsize++;
}
void DeleteQueue(Queue *Q) {							
	free(Q->key);
	free(Q);
}
void checkIndegree(Graph *G, int *idg) {								//indegree값을 계산하는 함수
	for ( int i = 0; i < G->size; i++ )
	{
		for ( int j = 0; j < G->size; j++ )
		{
			if ( G->matrix[i][j] == 1 )idg[j]++;
		}
	}
}
void sort_node(int *arr, int n) {											//가장 단순한 bubble sort를 통해arr을 정렬하는 함수
	int t;
	for ( int i = 0; i < n; i++ )
	{
		for ( int j = 0; j < n - 1; j++ )
		{
			if ( arr[j] > arr[j + 1] )
			{
				t = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = t;
			}
		}
	}
}

