
#include<stdio.h>
#include<stdlib.h>
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;
FILE *fin;
FILE *fout;
struct Node
{
	ElementType element;
	Position next;
};
Position FindPrevious(ElementType X, List L); //FindPrevious 함수 정의 이전에 Delete 함수에서 사용되므로 미리 선언해놓았습니다.
List MakeEmpty(List L)							
{
	L = (List)malloc(sizeof(struct Node));		//인수로 받은 List type L변수에 struct Node에 해당하는 메모리값을 배정받았습니다. 
	L->element = -1;							//인수로 받은 L은 header이기 때문에 element로 -1을 임력받았습니다.
	L->next = NULL;								//초기설정이므로 header의 next는 NULL로 설젇했습니다.
	return L;									//struct Node 한칸에 해당하는 메모리를 배정받은 L을 return 하여 header = MakeEmpty(header)를 통해 header에 이값을 저장합니다.
}

int IsEmpty(List L)
{

	return L->next == NULL;						//header다음에 NULL이라면 header밖에 없으므로 header->next 를 확인합니다.
}

int IsLast(Position P, List L)					//인수 P가 List의 마지막인지를 확인하는 함수 이므로 P다음이 NULL인지를 확인합니다.
{

	return P->next == NULL;						
}

void Insert(ElementType X, List L, Position P)
{
	Position p1 = (Position)malloc(sizeof(struct Node));      //우선 새로운 Node 하나를 만듭니다.   
	p1->element = X;										//그곳의 element 값으로 X를 넣습니다.
	p1->next = P->next;										//우선 p1의 next값으로 원래 p의 next값을 설정합니다,
	P->next = p1;											//그다음  p의 next값을 p1으로 설정하여 p1의 insert가 마무리됩니다.
}

void PrintList(List L)
{
	if ( IsEmpty(L) )
	{
		fprintf(fout, "empty list!\n");						//List가 비었는지 IsEmpty(L)을 통해 확인한 후 그렇다면 empty list를 출력합니다.
	}
	else
	{
		Position P;											
		P = L->next;										//List가 비어있지 않는다면 P를 List 첫번째 노드로 설정합니다.
		while ( P != NULL )									//P가 비어있지 않는다면 P가 가지고 있는 element를 출력하고 P를 다음 node로 옮깁니다. while문을 통해 끝노드까지 반복합니다.
		{
			fprintf(fout, "key:%d ", P->element);
			P = P->next;
		}

		fprintf(fout, "\n");								//위의 fprintf에 줄바꿈이 없으므로 여기서 줄바꿈을 해줍니다.
	}
}


void Delete(ElementType X, List L)
{
	Position ptr, ptr2;
	ptr = FindPrevious(X, L);								//FindPrevios 함수를 통해 ptr을 element로 x를 가지고 있는 노드 바로 앞으로 옮깁니다. x를 가지고 있는 노드가 없다면 마지막 노드를 입력받습니다.
	if ( ptr->next == NULL )								//ptr이 마지막 노드 즉 , 해당하는 X가 없음을 확인하면 오류 메세지를 띄웁니다.
	{
		fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", X,X); 
	}
	else                                                    //그게 아니라면 ptr2에 x를 가지고 있는 노드를 입력합니다. 그리고 그 앞의 노드인 ptr의 next를 ptr2의 next로 바꿉니다 이렇게 되면 ptr2와 ptr간의 연결이 끊기므로 리스트에서 삭제됩니다. 
	{
		ptr2 = ptr->next;			
		ptr->next = ptr2->next;
		free(ptr2);									//ptr2에 해당하는 노드는 삭제됐으므로 메모리를 반환합니다.
	}
}

Position Find(ElementType X, List L)
{
	Position P;
	P = L;												//P를 header로 설정합니다.
	while ( P != NULL && P->element != X )				//x값을 가지고 있는 노드를 찾을 때까지 list안의 노드들을 탐색하며 NULL을 만나거나 해당하는 노드를 찾으면 while문이종료됩니다. 해당하는 노드가 없으면 NULL을 반환합니다.
	{
		P = P->next;
	}
	return P;
}

Position FindPrevious(ElementType X, List L)
{
	Position P;
	P = L;
	while ( P->next != NULL && P->next->element != X )   //찾는 노드의 바로 전 노드를 찾는 것이므로 next를 이용해 탐색합니다. 틀은 find와 동일합니다. 해당하는 노드가 없으면 이 함수는 마지막 노드를 반환합니다.
	{
		P = P->next;
	}
	return P;
}

void DeleteList(List L)							
{
	Position P, PP;										//노드를 두개 사용합니다. 그 이유는 메모리반환을 위해서입니다.
	P = L;												//106부터 110줄은 List가 비어있는지를 검사하는 과정입니다. 비어있다면 header만 반환해줍니다.
	if ( P->next == NULL )
	{
		free(P);
	}													//그게아니라면 P를 list의 첫번째 노드, PP를 P노드 바로직전 노드로 잡습니다. 이관계는 115줄부터 120줄까지 계속 적용됩니다.P가 마지막 노드로 향할때까지 PP는 항상 그 직전에서 모든 노드의 메모리를 반환시키며 삭제합니다.
	else                                               
	{
		PP = L;
		P = L->next;
		while ( P != NULL )
		{
			free(PP);
			PP = P;
			P = P->next;
		}
		free(PP);										//P가 NULL이면 즉 PP는 마지막 노드가 될때 while문이 종료되는데 그럼 미처 삭제하지못한 마지막 노드가 남아있으므로 그것까지 삭제해줍니다,
	}
}

int main(int argc, char **argv)
{
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char x;
	Position header = NULL, tmp = NULL;
	header = MakeEmpty(header);
	while ( fscanf(fin, "%c", &x) != EOF )
	{
		if ( x == 'i' )
		{
			int a, b; fscanf(fin, "%d%d", &a, &b);
			tmp = Find(a, header);
			if ( tmp != NULL )
			{
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if ( tmp == NULL )
			{
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
		}
		else if ( x == 'd' )
		{
			int a; fscanf(fin, "%d", &a);
			Delete(a, header);
		}
		else if ( x == 'f' )
		{
			int a; fscanf(fin, "%d", &a);
			tmp = FindPrevious(a, header);
			if ( IsLast(tmp, header) ) fprintf(fout, "finding(%d) failed: element %d is not in the list\n",a,a);
			else
			{
				if ( tmp->element > 0 ) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if ( x == 'p' ) PrintList(header);
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	return 0;
}
// main의 내용은 과제 양식과 같으며 제가만든것이 아니라 따로 주석을 달지 않아도 될 것 같아서 주석을 달지 않았습니다.