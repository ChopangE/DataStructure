
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
Position FindPrevious(ElementType X, List L); //FindPrevious �Լ� ���� ������ Delete �Լ����� ���ǹǷ� �̸� �����س��ҽ��ϴ�.
List MakeEmpty(List L)							
{
	L = (List)malloc(sizeof(struct Node));		//�μ��� ���� List type L������ struct Node�� �ش��ϴ� �޸𸮰��� �����޾ҽ��ϴ�. 
	L->element = -1;							//�μ��� ���� L�� header�̱� ������ element�� -1�� �ӷ¹޾ҽ��ϴ�.
	L->next = NULL;								//�ʱ⼳���̹Ƿ� header�� next�� NULL�� �����߽��ϴ�.
	return L;									//struct Node ��ĭ�� �ش��ϴ� �޸𸮸� �������� L�� return �Ͽ� header = MakeEmpty(header)�� ���� header�� �̰��� �����մϴ�.
}

int IsEmpty(List L)
{

	return L->next == NULL;						//header������ NULL�̶�� header�ۿ� �����Ƿ� header->next �� Ȯ���մϴ�.
}

int IsLast(Position P, List L)					//�μ� P�� List�� ������������ Ȯ���ϴ� �Լ� �̹Ƿ� P������ NULL������ Ȯ���մϴ�.
{

	return P->next == NULL;						
}

void Insert(ElementType X, List L, Position P)
{
	Position p1 = (Position)malloc(sizeof(struct Node));      //�켱 ���ο� Node �ϳ��� ����ϴ�.   
	p1->element = X;										//�װ��� element ������ X�� �ֽ��ϴ�.
	p1->next = P->next;										//�켱 p1�� next������ ���� p�� next���� �����մϴ�,
	P->next = p1;											//�״���  p�� next���� p1���� �����Ͽ� p1�� insert�� �������˴ϴ�.
}

void PrintList(List L)
{
	if ( IsEmpty(L) )
	{
		fprintf(fout, "empty list!\n");						//List�� ������� IsEmpty(L)�� ���� Ȯ���� �� �׷��ٸ� empty list�� ����մϴ�.
	}
	else
	{
		Position P;											
		P = L->next;										//List�� ������� �ʴ´ٸ� P�� List ù��° ���� �����մϴ�.
		while ( P != NULL )									//P�� ������� �ʴ´ٸ� P�� ������ �ִ� element�� ����ϰ� P�� ���� node�� �ű�ϴ�. while���� ���� �������� �ݺ��մϴ�.
		{
			fprintf(fout, "key:%d ", P->element);
			P = P->next;
		}

		fprintf(fout, "\n");								//���� fprintf�� �ٹٲ��� �����Ƿ� ���⼭ �ٹٲ��� ���ݴϴ�.
	}
}


void Delete(ElementType X, List L)
{
	Position ptr, ptr2;
	ptr = FindPrevious(X, L);								//FindPrevios �Լ��� ���� ptr�� element�� x�� ������ �ִ� ��� �ٷ� ������ �ű�ϴ�. x�� ������ �ִ� ��尡 ���ٸ� ������ ��带 �Է¹޽��ϴ�.
	if ( ptr->next == NULL )								//ptr�� ������ ��� �� , �ش��ϴ� X�� ������ Ȯ���ϸ� ���� �޼����� ���ϴ�.
	{
		fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", X,X); 
	}
	else                                                    //�װ� �ƴ϶�� ptr2�� x�� ������ �ִ� ��带 �Է��մϴ�. �׸��� �� ���� ����� ptr�� next�� ptr2�� next�� �ٲߴϴ� �̷��� �Ǹ� ptr2�� ptr���� ������ ����Ƿ� ����Ʈ���� �����˴ϴ�. 
	{
		ptr2 = ptr->next;			
		ptr->next = ptr2->next;
		free(ptr2);									//ptr2�� �ش��ϴ� ���� ���������Ƿ� �޸𸮸� ��ȯ�մϴ�.
	}
}

Position Find(ElementType X, List L)
{
	Position P;
	P = L;												//P�� header�� �����մϴ�.
	while ( P != NULL && P->element != X )				//x���� ������ �ִ� ��带 ã�� ������ list���� ������ Ž���ϸ� NULL�� �����ų� �ش��ϴ� ��带 ã���� while��������˴ϴ�. �ش��ϴ� ��尡 ������ NULL�� ��ȯ�մϴ�.
	{
		P = P->next;
	}
	return P;
}

Position FindPrevious(ElementType X, List L)
{
	Position P;
	P = L;
	while ( P->next != NULL && P->next->element != X )   //ã�� ����� �ٷ� �� ��带 ã�� ���̹Ƿ� next�� �̿��� Ž���մϴ�. Ʋ�� find�� �����մϴ�. �ش��ϴ� ��尡 ������ �� �Լ��� ������ ��带 ��ȯ�մϴ�.
	{
		P = P->next;
	}
	return P;
}

void DeleteList(List L)							
{
	Position P, PP;										//��带 �ΰ� ����մϴ�. �� ������ �޸𸮹�ȯ�� ���ؼ��Դϴ�.
	P = L;												//106���� 110���� List�� ����ִ����� �˻��ϴ� �����Դϴ�. ����ִٸ� header�� ��ȯ���ݴϴ�.
	if ( P->next == NULL )
	{
		free(P);
	}													//�װԾƴ϶�� P�� list�� ù��° ���, PP�� P��� �ٷ����� ���� ����ϴ�. �̰���� 115�ٺ��� 120�ٱ��� ��� ����˴ϴ�.P�� ������ ���� ���Ҷ����� PP�� �׻� �� �������� ��� ����� �޸𸮸� ��ȯ��Ű�� �����մϴ�.
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
		free(PP);										//P�� NULL�̸� �� PP�� ������ ��尡 �ɶ� while���� ����Ǵµ� �׷� ��ó ������������ ������ ��尡 ���������Ƿ� �װͱ��� �������ݴϴ�,
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
// main�� ������ ���� ��İ� ������ ����������� �ƴ϶� ���� �ּ��� ���� �ʾƵ� �� �� ���Ƽ� �ּ��� ���� �ʾҽ��ϴ�.