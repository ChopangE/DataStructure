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
	Heap* heap;																	//heap을 생성합니다.
	heap = (Heap *)malloc(sizeof(Heap));
	if ( heap == NULL ) return NULL;
	heap->Capacity = heapSize;													//capacity를 heapSize로 설정합니다.
	heap->Size = 0;																//아무것도 없는 상태를 0으로 지정하였습니다. 즉 heap->Size는 현재 heap에 들어있는 Elements의 수입니다.
	heap->Elements = (int *)malloc(sizeof(int) * heapSize);						//Elements를 할당받았습니다. 배열로서 사용됩니다.

}

void Insert(Heap *heap, int value){
	if ( IsFull(heap) )																	//heap이 Full이면 에러메시지를 띄웠습니다.
	{
		fprintf(fout, "insert error : heap is full\n");
		return;
	}
	if ( Find(heap, value) )															//heap에 이미 value가 존재할경우 에러메시지를 띄웠습니다.
	{
		fprintf(fout, "insert error : %d is already in the heap\n",value);
		return;
	}
	int i = heap->Size;																	//i는 Elements 배열의 마지막 index가 됩니다.
	heap->Elements[heap->Size++] = value;												//Elements의 마지막 index에 value를 삽입하고 Size를 하나 올립니다. 
	fprintf(fout, "insert %d\n", value);												//삽입성공 메시지를 띄웁니다.
	while ( i != 0 )																	//percolate up을 진행합니다. root에 닿을 때 까지 진행합니다.
	{
		int parent = heap->Elements[(i - 1) / 2];										//삽입된 value의 부모입니다.
		int child = heap->Elements[i];													//삽입된 value입니다.
		if ( parent < child )															//만약 삽입된 value가 더 크다면 부모랑 바꿔주는 작업을 진행하고 index(i)를 부모로 바꿉니다.
		{
			heap->Elements[i] = parent;
			heap->Elements[(i - 1) / 2] = child;
			i = (i - 1) / 2;
		}																				//만약 child가 더 작다면 그대로 percolate up은 종료됩니다.
		else
			break;
	}
}

int Find(Heap* heap, int value){
	
	for ( int i = 0; i < heap->Size; i++ )												//배열의 맨 앞쪽부터 순차대로 탐색을 진행합니다 도중에 발견되면 1을 return하고 없으면 0을 return합니다.
	{
		if ( heap->Elements[i] == value )return 1;
	}
	return 0;
}

int DeleteMax(Heap* heap){
	if ( heap->Size == 0 )
	{
		fprintf(fout, "delete error : heap is empty\n");							//heap의 size가 0 즉 heap에 아무것도 들어있지 않은 경우 에러메시지를 띄웠습니다.
		return -INF;
	}
	int del_element = heap->Elements[0];											//삭제될 노드입니다.
	heap->Size--;																	//size를 하나 내리고 root위치에 맨 마지막 index에 있던 value를 올립니다.
	heap->Elements[0] = heap->Elements[heap->Size];
	int i = 0;	
	while ( i < heap->Size )													//percolate down의 과정입니다.
	{
		int left = i * 2 + 1;												//왼쪽자식입니다.
		int right = i * 2 + 2;												//오른쪽자식입니다.
		int max;
		if ( left < heap->Size && right < heap->Size )					//자식 둘다 존재하는경우입니다.
		{
			if ( heap->Elements[left] > heap->Elements[right] ) max = left;		//자식둘중에 누가더 큰지 확인하고 max로 설정합니다 이 max가 index로 가리키고있는 부모의 value보다 클경우 둘의 위치를 바꿉니다.
			else max = right;
			if ( heap->Elements[max] > heap->Elements[i] )
			{
				int swap = heap->Elements[max];
				heap->Elements[max] = heap->Elements[i];
				heap->Elements[i] = swap;
				i = max;													//index를 자식중에 더 컸던 자식으로 옮기고 loop를 진행합니다.
			}
			else
				break;													//자식보다 부모가 더 크므로 종료합니다.
		}
		else if ( left < heap->Size && right >= heap->Size )		//왼쪽 자식만 존재하는경우입니다.
		{
			max = left;
			if ( heap->Elements[max] > heap->Elements[i] )			//바꿀필요가 있는지 확인후 leaf에 도달했기 때문에 break합니다.
			{
				int swap = heap->Elements[max];
				heap->Elements[max] = heap->Elements[i];
				heap->Elements[i] = swap;
			}
			break;
		}
		else                                                       //자식이 존재하지 않는 경우입니다. 바로 break합니다.
			break;
	}
	return del_element;											//처음에 지정되었던 value를 return합니다.
}

void PrintHeap(Heap* heap){
	if(heap->Size == 0){													//empty한지 확인합니다.
		fprintf(fout, "print error : heap is empty\n");
		return;
	}
	for ( int i = 0; i < heap->Size; i++ )										//배열의 처음부터 끝까지 출력합니다 자동으로 level order가 진행됩니다.
	{
		if(i == heap->Size - 1)fprintf(fout, "%d", heap->Elements[i]);
		else fprintf(fout, "%d ", heap->Elements[i]);
	}
	fprintf(fout, "\n");	
}

int IsFull(Heap* heap){															//size가 capacity랑 같은경우 즉 꽉찬 경우 1을 return합니다.아니면 0을 return합니다.
	if ( heap->Size == heap->Capacity ) return 1;
	return 0;
}
