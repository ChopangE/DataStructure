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
	parseInput(nodes, numbers, n);							//nodes�� ���ڹ迭�� ���Եȴ�.

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
	Graph *graph;														//graph �ʱ�ȭ
	graph = (Graph *)malloc(sizeof(Graph));
	sort_node(nodes, n);											//bubble_sort�˰����� ���� nodes�� ����.
	graph->node = nodes;
	graph->size = n;
	graph->matrix = (int **)malloc(sizeof(int *) * graph->size);			//matrix�� �����������̹Ƿ� �ι��� ���ļ� malloc����
	for ( int i = 0; i < n; i++ )
	{
		graph->matrix[i] = (int *)malloc(sizeof(int) * graph->size);
	}
	for ( int i = 0; i < n; i++ )
	{
		for ( int j = 0; j < n; j++ )
		{
			graph->matrix[i][j] = 0;										//matrix�� ��� ��ҵ��� 0���� �ʱ�ȭ
		}
	}
	return graph;
}
void InsertEdge(Graph *G, int a, int b) {
	int i, j;
	for ( i = 0; i < G->size; i++ )
	{
		if ( G->node[i] == a )break;							//a�� b�� node������ index�� ã������ matrix�� �ش� index�� 1�� ����.
	}
	for ( j = 0; j < G->size; j++ )
	{
		if ( G->node[j] == b )break;
	}
	G->matrix[i][j] = 1;
}
void PrintGraph(Graph *G) {																//�ܼ��� ����� ����
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
	for ( int i = 0; i < G->size; i++ )											//matrix�� �����������̹Ƿ� �ι��� ���� free�Ѵ� �������� ��ҿ� ���� free����
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
	idg = (int *)malloc(sizeof(int) * G->size);					//indegree�� ������ ���� �迭
	lst = (int *)malloc(sizeof(int) * G->size);					//dequeue������� �� �迭�� ����. ����� ���� ���� �����س��� �迭
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
			if ( G->node[i] == v )break;						//�ܼ� indexã��
		}
		lst[cur++] = v;
		for ( int j = 0; j < G->size; j++ )
		{
			if ( G->matrix[i][j] == 1 )idg[j]--;				//indegree�� ����
			if ( idg[j] == 0 ) { 
				Enqueue(que, G->node[j]);						//indegree���� 0�� ������ enqueue����
				idg[j]--;										//�̹� enqueue�� ���� key�� indegree�� -1�� �ȴ�.
			}
		}
		if ( IsEmpty(que) && (check_cycle > 0 ))						//cycle�� �����ϴ� ��Ȳ
		{
			fprintf(fout,"sorting error : cycle!\n");
			DeleteQueue(que);
			free(lst);
			free(idg);
			return;
		}

	}
	for ( int j = 0; j < G->size; j++ )									//cycle�� �������� �ʴ� ��� lst�� ��� ��ҵ��� ���ʴ�� ���.
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
Queue *MakeNewQueue(int X) {							//���ο� queue�� �����ϴ� �Լ�, ����ϰ� �ʱ�ȭ�� �����Ѵ�.
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
	if ( (Q->first == Q->rear) && Q->qsize == 0 ) return 1;				//first�� rear�� ������ ���� full���� empty���������� size�� 0�̸� empty�̴�.
	else return 0;
}
int Dequeue(Queue *Q) {														//�ܼ��� dequeue���� ���� queue�� �����Ͽ���. 
	int t;
	t = Q->key[Q->rear++];
	Q->rear = Q->rear % Q->max_queue_size;
	Q->qsize--;
	return t;
}
void Enqueue(Queue *Q, int X) {												//�ܼ��� enqueue����
	Q->key[Q->first++] = X;
	Q->first = Q->first % Q->max_queue_size;
	Q->qsize++;
}
void DeleteQueue(Queue *Q) {							
	free(Q->key);
	free(Q);
}
void checkIndegree(Graph *G, int *idg) {								//indegree���� ����ϴ� �Լ�
	for ( int i = 0; i < G->size; i++ )
	{
		for ( int j = 0; j < G->size; j++ )
		{
			if ( G->matrix[i][j] == 1 )idg[j]++;
		}
	}
}
void sort_node(int *arr, int n) {											//���� �ܼ��� bubble sort�� ����arr�� �����ϴ� �Լ�
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

