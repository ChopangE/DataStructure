#include<stdio.h>
#include<stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(maxHeap, key);
				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

Heap* CreateHeap(int heapSize){																
	Heap* heap;																	//heap�� �����մϴ�.
	heap = (Heap *)malloc(sizeof(Heap));
	if ( heap == NULL ) return NULL;
	heap->Capacity = heapSize;													//capacity�� heapSize�� �����մϴ�.
	heap->Size = 0;																//�ƹ��͵� ���� ���¸� 0���� �����Ͽ����ϴ�. �� heap->Size�� ���� heap�� ����ִ� Elements�� ���Դϴ�.
	heap->Elements = (int *)malloc(sizeof(int) * heapSize);						//Elements�� �Ҵ�޾ҽ��ϴ�. �迭�μ� ���˴ϴ�.

}

void Insert(Heap *heap, int value){
	if ( IsFull(heap) )																	//heap�� Full�̸� �����޽����� ������ϴ�.
	{
		fprintf(fout, "insert error : heap is full\n");
		return;
	}
	if ( Find(heap, value) )															//heap�� �̹� value�� �����Ұ�� �����޽����� ������ϴ�.
	{
		fprintf(fout, "insert error : %d is already in the heap\n",value);
		return;
	}
	int i = heap->Size;																	//i�� Elements �迭�� ������ index�� �˴ϴ�.
	heap->Elements[heap->Size++] = value;												//Elements�� ������ index�� value�� �����ϰ� Size�� �ϳ� �ø��ϴ�. 
	fprintf(fout, "insert %d\n", value);												//���Լ��� �޽����� ���ϴ�.
	while ( i != 0 )																	//percolate up�� �����մϴ�. root�� ���� �� ���� �����մϴ�.
	{
		int parent = heap->Elements[(i - 1) / 2];										//���Ե� value�� �θ��Դϴ�.
		int child = heap->Elements[i];													//���Ե� value�Դϴ�.
		if ( parent < child )															//���� ���Ե� value�� �� ũ�ٸ� �θ�� �ٲ��ִ� �۾��� �����ϰ� index(i)�� �θ�� �ٲߴϴ�.
		{
			heap->Elements[i] = parent;
			heap->Elements[(i - 1) / 2] = child;
			i = (i - 1) / 2;
		}																				//���� child�� �� �۴ٸ� �״�� percolate up�� ����˴ϴ�.
		else
			break;
	}
}

int Find(Heap* heap, int value){
	
	for ( int i = 0; i < heap->Size; i++ )												//�迭�� �� ���ʺ��� ������� Ž���� �����մϴ� ���߿� �߰ߵǸ� 1�� return�ϰ� ������ 0�� return�մϴ�.
	{
		if ( heap->Elements[i] == value )return 1;
	}
	return 0;
}

int DeleteMax(Heap* heap){
	if ( heap->Size == 0 )
	{
		fprintf(fout, "delete error : heap is empty\n");							//heap�� size�� 0 �� heap�� �ƹ��͵� ������� ���� ��� �����޽����� ������ϴ�.
		return -INF;
	}
	int del_element = heap->Elements[0];											//������ ����Դϴ�.
	heap->Size--;																	//size�� �ϳ� ������ root��ġ�� �� ������ index�� �ִ� value�� �ø��ϴ�.
	heap->Elements[0] = heap->Elements[heap->Size];
	int i = 0;	
	while ( i < heap->Size )													//percolate down�� �����Դϴ�.
	{
		int left = i * 2 + 1;												//�����ڽ��Դϴ�.
		int right = i * 2 + 2;												//�������ڽ��Դϴ�.
		int max;
		if ( left < heap->Size && right < heap->Size )					//�ڽ� �Ѵ� �����ϴ°���Դϴ�.
		{
			if ( heap->Elements[left] > heap->Elements[right] ) max = left;		//�ڽĵ��߿� ������ ū�� Ȯ���ϰ� max�� �����մϴ� �� max�� index�� ����Ű���ִ� �θ��� value���� Ŭ��� ���� ��ġ�� �ٲߴϴ�.
			else max = right;
			if ( heap->Elements[max] > heap->Elements[i] )
			{
				int swap = heap->Elements[max];
				heap->Elements[max] = heap->Elements[i];
				heap->Elements[i] = swap;
				i = max;													//index�� �ڽ��߿� �� �Ǵ� �ڽ����� �ű�� loop�� �����մϴ�.
			}
			else
				break;													//�ڽĺ��� �θ� �� ũ�Ƿ� �����մϴ�.
		}
		else if ( left < heap->Size && right >= heap->Size )		//���� �ڽĸ� �����ϴ°���Դϴ�.
		{
			max = left;
			if ( heap->Elements[max] > heap->Elements[i] )			//�ٲ��ʿ䰡 �ִ��� Ȯ���� leaf�� �����߱� ������ break�մϴ�.
			{
				int swap = heap->Elements[max];
				heap->Elements[max] = heap->Elements[i];
				heap->Elements[i] = swap;
			}
			break;
		}
		else                                                       //�ڽ��� �������� �ʴ� ����Դϴ�. �ٷ� break�մϴ�.
			break;
	}
	return del_element;											//ó���� �����Ǿ��� value�� return�մϴ�.
}

void PrintHeap(Heap* heap){
	if(heap->Size == 0){													//empty���� Ȯ���մϴ�.
		fprintf(fout, "print error : heap is empty\n");
		return;
	}
	for ( int i = 0; i < heap->Size; i++ )										//�迭�� ó������ ������ ����մϴ� �ڵ����� level order�� ����˴ϴ�.
	{
		if(i == heap->Size - 1)fprintf(fout, "%d", heap->Elements[i]);
		else fprintf(fout, "%d ", heap->Elements[i]);
	}
	fprintf(fout, "\n");	
}

int IsFull(Heap* heap){															//size�� capacity�� ������� �� ���� ��� 1�� return�մϴ�.�ƴϸ� 0�� return�մϴ�.
	if ( heap->Size == heap->Capacity ) return 1;
	return 0;
}
