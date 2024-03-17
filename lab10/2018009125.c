#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);


void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size + 1);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	printShortestPath(g);

	deleteGraph(g);
}
Graph *createGraph(int size)
{
	
	Graph *g;
	g = (Graph*)malloc(sizeof(Graph));															//graph���� �� �ʱ�ȭ.
	g->size = size;
	g->nodes = (Node*)malloc(sizeof(Node) * size);
	g->vertices = (int**)malloc(sizeof(int *) * size);
	for ( int i = 0; i < size; i++ )
	{
		g->vertices[i] = (int *)malloc(sizeof(int) * size);
	}
	for ( int i = 0; i < size; i++ )
	{
		for ( int j = 0; j < size; j++ )
		{
			g->vertices[i][j] = 0;
		}
	}																							//vertices�� ��� ��Ҹ� 0���� �ʱ�ȭ.-> ���߿� 0���� �ƴ����� adjacent�� �ľ�
	for ( int i = 0; i < size; i++ )
	{
		g->nodes[i].vertex = i;
		g->nodes[i].dist = INF;
		g->nodes[i].prev = -1;
	}																							//��� ��� �ʱ�ȭ ó�� ���¿��� dist�� ��� INF . prev�� -1, vertex�� ����ȣ
	g->nodes[1].dist = 0;																		//���ܷ� source�� node[1]�� dist�� 0���� �ʱ�ȭ
	return g;

}

void deleteGraph(Graph *g)
{
	//vertice node graph������ free
	for ( int i = 0; i < g->size; i++ )
	{
		free(g->vertices[i]);
	}
	free(g->vertices);
	free(g->nodes);
	free(g);

}
//���ͽ�Ʈ�� ���� �� ����Ʈ
void printShortestPath(Graph *g)
{
	Heap *heap = createMinHeap(g->size-1);				//minheap����.
	Node u;												
	int i;
	for ( int i = 0; i < g->size-1; i++ )
	{
		heap->Element[i] = g->nodes[i + 1];				//heap element���� �׷����� nodes���� �״�� �����ؿ�.
	}
	while ( heap->Size > 0 )							//heap->Size�� 0�̵Ǹ� ����.
	{
		u = deleteMin(heap);							//ó�� while���� ������ ������ source�� 1�� u�� return 
		for (  i = 1; i < g->size; i++ )				//adjacent search
		{
			if ( g->vertices[u.vertex][i] > 0  )											//vertices���� 0���� ũ�� adjacent �߰�!
			{
				if ( g->nodes[i].dist > u.dist + g->vertices[u.vertex][i] )					//like d[i] > d[u] + w[u,i]
				{
					g->nodes[i].dist = u.dist + g->vertices[u.vertex][i];					//d[i] = d[u] + w[u,i]
					g->nodes[i].prev = u.vertex;											//pred[i] = u
					insertToMinHeap(heap, g->nodes[i].vertex, g->nodes[i].dist);			//decrease_key
				}
			}
		}
	}
	for ( i = 2; i < g->size; i++ )													//source�� ������ ������ ����� source�� ������ ��� ���
	{
		int count = 0;
		int j = i;
		while ( g->nodes[j].prev != -1 )											//prev =-1�� �� ���� prev���� Ž��
		{
			count++;
			j = g->nodes[j].prev;
		}
		if ( g->nodes[j].vertex != 1 ) {										//�ҽ��� �ƴ϶�� ��ΰ� �������� �����Ƿ� error
			fprintf(fout, "can not reach to node %d\n", i); 
			continue;
		}
		int *arr = (int*)malloc(sizeof(int) * count);							//����� ���� �迭 
		int count1 = count;
		j = i;
		while ( count1 > 0 )													//source�� ���� �ش� �������� ��� ����
		{
			arr[count1 - 1] = g->nodes[j].prev;
			j = g->nodes[j].prev;
			count1--;
		}
		for ( int k = 0; k < count; k++ )									//source�κ��� �ش�������� ��� ���
		{
			fprintf(fout, "%d->", arr[k]);
		}
		fprintf(fout, "%d (cost : %d)\n", i, g->nodes[i].dist);			
		free(arr);				
	}
	deleteMinHeap(heap);													//free(heap)
	
}

Heap *createMinHeap(int heapSize)
{												//�Ϲ����� heap �ʱ�ȭ����
	Heap *heap;
	heap = (Heap *)malloc(sizeof(Heap));
	heap->Capacity = heapSize;
	heap->Size = heapSize;
	heap->Element = (Node *)malloc(sizeof(Node) * heapSize);
	return heap;

}
void deleteMinHeap(Heap *minHeap)			//free(heap)�� ���� helper_function  
{
	free(minHeap->Element);
	free(minHeap);

}
void insertToMinHeap(Heap *minHeap, int vertex, int distance)	//decrease key
{
	int i = 0;
	for ( i = 0; i < minHeap->Size; i++ )						//dist�� �ٲ� ��� Ž��
	{
		if ( minHeap->Element[i].vertex == vertex )
		{
			minHeap->Element[i].dist = distance;
			break;
		}
	}
	Node child,parent, tmp;
	while ( i > 0 )													//dist�� update�� ���� ���� root���� �������
	{
		child = minHeap->Element[i];
		parent = minHeap->Element[(i - 1) / 2];
		if ( child.dist < parent.dist )								//child�� �θ𺸴� �۴ٸ� ��ü�ؾߵ�
		{
			tmp = child;
			minHeap->Element[i] = parent;
			minHeap->Element[(i - 1) / 2] = tmp;
			i = (i - 1) / 2;
		}
		else
			break;													//�θ� child���� �۴ٸ� break�ϰ� ����
	}

}
Node deleteMin(Heap *minHeap)									
{
	Node deleted_node = minHeap->Element[0];								//delete�� ���
	int i = 0;
	minHeap->Element[0] = minHeap->Element[minHeap->Size - 1];			//�迭�� ��������带 �迭�� ù��°�� ������	
	minHeap->Size--;
	int left, right, min;
	Node tmp;
																			
	while ( i < minHeap->Size )														//root�κ��� leaf���� Ž��					
	{
		left = (2 * i + 1);																		//left_child�� index
		right = (2 * i + 2);																	//right_child�� index
		if ( left < minHeap->Size && right < minHeap->Size )									//�ڽ��� �Ѵ� �����Ѵٸ�
		{
			if ( minHeap->Element[left].dist < minHeap->Element[right].dist ) min = left;		//�ڽ� ���� dist�� ������ �ڽ� ������
			else min = right;
			if ( minHeap->Element[min].dist < minHeap->Element[i].dist )							//�θ𺸴� �ڽ��� dist�� ���۴ٸ�
			{
				tmp = minHeap->Element[min];
				minHeap->Element[min] = minHeap->Element[i];
				minHeap->Element[i] = tmp;

			}
			else break;

		}
		else if ( left < minHeap->Size )														//�ڽ��� ���ʸ� �ִ°�� �����ʸ� �ִ°��� ����.
		{
			min = left;
			if ( minHeap->Element[min].dist < minHeap->Element[i].dist )						//�ڽİ� �θ� dist��
			{
				tmp = minHeap->Element[min];
				minHeap->Element[min] = minHeap->Element[i];
				minHeap->Element[i] = tmp;

			}
			else break;
		}
		else break;
	}

	return deleted_node;																		//ó���� �����Ҵ� delete�� ��� return

}

