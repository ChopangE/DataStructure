#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}

AVLTree Insert(ElementType X, AVLTree T)
{
	int l_h = -1;
	int ll_h = -1;
	int r_h = -1;
	int rr_h = -1;
	int count = 1;
	int dir = 0;
	Position *arr;
	Position T1;
	Position cur;
	Position ccur;
	T1 = (Position)malloc(sizeof(AVLNode));					//초기화
	T1->height = 0;
	T1->left = NULL;
	T1->right = NULL;
	T1->element = X;
	if ( T == NULL )									//빈노드일때
		return T1;
	cur = T;
	ccur = cur;
	while ( cur != NULL )					//삽입될 위치 찾기
	{
		if ( cur->element > X )
		{
			ccur = cur;
			cur = cur->left;
			dir = 0;
			count++;
		}
		else if ( cur->element < X )
		{
			ccur = cur;
			cur = cur->right;
			dir = 1;
			count++;
		}
		else                                 //cur->element = x 라면 이미 존재하는 노드이기 떄문에 에러메시지를 반환.
		{
			fprintf(fout, "insertion error : %d is already in the tree!\n", X);
			return T;
		}
	}
	if ( dir == 0 )													//dir=0 부모의 왼쪽에서 ,dir =1 부모의 오른쪽에서.
		ccur->left = T1;
	else
		ccur->right = T1;
	cur = T;
	arr = (Position *)malloc(sizeof(Position) * count);				//Position타입을 저장하는 배열을 설정 이때 78줄 while문에서 count된 노드수가 배열의 크기를 결정지어줌. arr은 Position* 즉 이중포인터.
	for ( int i = 0; i < count; i++ )								//root부터 차례대로 배열안으로 집어넣음.
	{
		arr[i] = cur;
		if ( cur->element > X )
			cur = cur->left;
		else
			cur = cur->right;
	}
	for ( int i = count - 1; i >= 0; i-- )						//집어넣은것을 역으로 꺼내어 '높이'를 정하는 과정을 진행.
	{
		if ( i != 0 )											//i=0이라면 이것은 root이기 때문에 부모 노드가 없으므로 실행안해도됨.
		{
			if ( arr[i - 1]->left == arr[i] ) dir = 0;
			else dir = 1;
		}															//높이를 새로 결정할때 NULL노드의 높이를 -1로 설정해서 비교함.
		if ( arr[i]->left == NULL )
			l_h = -1;
		else
			l_h = arr[i]->left->height;
		if ( arr[i]->right == NULL )
			r_h = -1;
		else
			r_h = arr[i]->right->height;
		if ( l_h > r_h )
		{
			if ( l_h - r_h > 1 )         //rotate가 필요한 순간.(자식간의 높이차이가 2이상난다.)
			{
				if ( arr[i]->left->left == NULL )						//왼쪽의 height가 높으므로 왼쪽노드의 자식들을 조사하여 이것이 singlerotate상황인지 아니면 doublerotate상황인지 확인함.
					ll_h = -1;
				else ll_h = arr[i]->left->left->height;
				if ( arr[i]->left->right == NULL )
					rr_h = -1;
				else rr_h = arr[i]->left->right->height;
				if ( ll_h > rr_h )
				{
					arr[i] = SingleRotateWithLeft(arr[i]);
					
				}
				else
				{
					arr[i] = DoubleRotateWithLeft(arr[i]);
					
				}
				if ( i != 0 )
				{
					if ( dir == 0 ) arr[i - 1]->left = arr[i];   //부모랑 연결.
					else arr[i - 1]->right = arr[i];
				}
			}
			else    //정상적인 상황(자식간의 높이가 1이하로 차이남)
			{
				arr[i]->height = ++l_h;
			}
		}															//위의 코드에서 좌우만 바뀌었다.
		else if ( l_h < r_h )
		{
			if ( r_h - l_h > 1 )					//rotate가 필요한 순간.
			{
				if ( arr[i]->right->right == NULL )
					rr_h = -1;
				else rr_h = arr[i]->right->right->height;
				if ( arr[i]->right->left == NULL )
					ll_h = -1;
				else ll_h = arr[i]->right->left->height;
				if ( rr_h > ll_h )
				{
					arr[i] = SingleRotateWithRight(arr[i]);
				}
				else
				{
					arr[i] = DoubleRotateWithRight(arr[i]);
				}
			
				if ( i != 0 )
				{
					if ( dir == 0 ) arr[i - 1]->left = arr[i];
					else arr[i - 1]->right = arr[i];
				}
			}
			else    //정상적인 상황(자식간의 높이가 1이하로 차이남.)
			{
				arr[i]->height = ++r_h;
			}

		}
		else
		{
			arr[i]->height = ++l_h;
		}
		
	}					//cur에 arr[0] 즉 root를 저장해놓은다음 free시행하고 cur을 반환함. free(arr)은 배열만 free되지 그 내용물도 같이 free되지 않음.
	cur = arr[0];
	free(arr);
	return cur;
}
AVLTree Delete(ElementType X, AVLTree T)
{
	int is_root=0;
	int dir = 0;
	int l_h = -1;
	int r_h = -1;
	int ll_h = -1;
	int rr_h = -1;
	int count = 1;
	int idx;
	int n;
	Position cur = T;
	Position ccur = cur;
	Position cccur;
	Position needed_del;
	Position *arr;
	while ( cur != NULL )
	{
		if ( cur->element < X )
		{
			ccur = cur;
			cur = cur->right;
			dir = 1;
			count++;
		}
		else if ( cur->element > X )
		{
			ccur = cur;
			cur = cur->left;
			dir = 0;
			count++;
		}
		else
			break;
	}
	if ( cur == NULL )										//X 가존재하지 않을때.
	{
		fprintf(fout, "deletion error : %d is not in the tree!\n", X);
		return T;
	}
	if ( count == 1 )is_root = 1;				//삭제할 노드가 root일 때
	idx = count - 1;							//삭제될 노드의 index표시.
	needed_del = cur;							//free될 노드를 표시
	if ( cur->left == NULL || cur->right == NULL )
	{
		if ( cur->left != NULL ) //cur->right == NULL
		{
			if ( is_root )
			{
				cur = cur->left;
				free(ccur);
				return cur;
			}
			if ( dir == 0 )ccur->left = cur->left;
			else ccur->right = cur->left;
		}
		else if ( cur->right != NULL ) //cur->left == NULL;
		{
			if ( is_root )
			{
				cur = cur->right;
				free(ccur);
				return cur;
			}
			if ( dir == 0 )ccur->left = cur->right;
			else ccur->right = cur->right;
		}
		else//자식노드 없음.
		{
			if ( is_root )
			{
				free(ccur);
				T = NULL;
				return T;
			}
			if ( dir == 0 )ccur->left = NULL;
			else ccur->right = NULL;
		}
		arr = (Position *)malloc(sizeof(Position) * (count-1));			//insert와 같은 개념으로 접근한다.
		cur = T;
		for ( int i = 0; i < count-1; i++ )
		{
			arr[i] = cur;
			if ( cur->element > X )
				cur = cur->left;
			else
				cur = cur->right;
		}//arr작성완료
		n = count - 1;									//배열의 가장 마지막 index표시.
	}
	else//자식 둘다있음.
	{
		
		cur = cur->right;
		cccur = cur;
		count++;
		while ( cur->left != NULL )
		{
			cccur = cur;
			cur = cur->left;
			count++;
		}
		if ( cur == cccur )  //오른쪽 노드의 왼쪽자식이 없을떄.
		{
			if ( is_root )
			{
				cur->left = T->left;
				
				T = cur;
			}
			else
			{
				if ( dir == 0 )
				{
					cur->left = ccur->left->left;;
					ccur->left = cur;
				}
				else
				{
					cur->left = ccur->right->left;
					ccur->right = cur;
				}
			}
		}
		else
		{
			cccur->left = cur->right;
			if ( is_root == 0 )				//삭제하려는 노드가 root가 아닐경우 실행
			{
				if ( dir == 0 )				//삭제하려는 노드가 부모의 왼쪽에서 왔을때
				{
					cur->left = ccur->left->left;
					cur->right = ccur->left->right;
					ccur->left = cur;
				}
				else                            //삭제하려는 노드가 부모의 오른쪽에서 왔을 때
				{
					cur->left = ccur->right->left;
					cur->right = ccur->right->right;
					ccur->right = cur;
				}
			}
			else  //삭제하려는 노드가 root일 때
			{
				cur->left = T->left;
				cur->right = T->right;
				T = cur;
			}
		}
		cur = T;
		arr = (Position *)malloc(sizeof(Position) * (count - 1));
		for ( int i = 0; i < idx + 1; i++ )								//우선 root부터 삭제될 노드까지를 배열에 담고 그이후로는 rightsubtree에서 가장 작은 노드를 찾을 때까지 거쳐간 모든 노드를 저장한다.
		{
			arr[i] = cur;
			if ( i == idx )break;
			if ( cur->element > X )
				cur = cur->left;
			else
				cur = cur->right;
		}
		cur = cur->right;
		for ( int i = idx + 1; i < count - 1; i++ )
		{
			arr[i] = cur;
			cur = cur->left;
		}
		n = count - 1;
	}
	for ( int i = n - 1; i >= 0; i-- )										//insert와 똑같이 끝에서부터 진행하며 height를 새로 설정해주는 단계. insert와 코드가 동일하다.
	{
		if ( i != 0 )
		{
			if ( arr[i - 1]->left == arr[i] ) dir = 0;
			else dir = 1;
		}
		if ( arr[i]->left == NULL )
			l_h = -1;
		else
			l_h = arr[i]->left->height;
		if ( arr[i]->right == NULL )
			r_h = -1;
		else
			r_h = arr[i]->right->height;
		if ( l_h > r_h )
		{
			if ( l_h - r_h > 1 )         //rotate가 필요한 순간.
			{
				if ( arr[i]->left->left == NULL )
					ll_h = -1;
				else ll_h = arr[i]->left->left->height;
				if ( arr[i]->left->right == NULL )
					rr_h = -1;
				else rr_h = arr[i]->left->right->height;
				if ( ll_h > rr_h )
				{
					arr[i] = SingleRotateWithLeft(arr[i]);

				}
				else
				{
					arr[i] = DoubleRotateWithLeft(arr[i]);

				}
				if ( i != 0 )
				{
					if ( dir == 0 ) arr[i - 1]->left = arr[i];   //부모랑 연결.
					else arr[i - 1]->right = arr[i];
				}
			}
			else    //정상적인 상황
			{
				arr[i]->height = ++l_h;
			}
		}
		else if ( l_h < r_h )
		{
			if ( r_h - l_h > 1 )
			{
				if ( arr[i]->right->right == NULL )
					rr_h = -1;
				else rr_h = arr[i]->right->right->height;
				if ( arr[i]->right->left == NULL )
					ll_h = -1;
				else ll_h = arr[i]->right->left->height;
				if ( rr_h > ll_h )
				{
					arr[i] = SingleRotateWithRight(arr[i]);
				}
				else
				{
					arr[i] = DoubleRotateWithRight(arr[i]);
				}

				if ( i != 0 )
				{
					if ( dir == 0 ) arr[i - 1]->left = arr[i];
					else arr[i - 1]->right = arr[i];
				}
			}
			else    //정상적인 상황
			{
				arr[i]->height = ++r_h;
			}

		}
		else
		{
			arr[i]->height = ++l_h;
		}
		
	}
	free(needed_del);
	T = arr[0];
	free(arr);
	return T;
}


 // 둘다 있을 때..

Position SingleRotateWithLeft(Position node)
{
	Position cur = node;
	Position Lcur = cur->left;
	cur->left = Lcur->right;							//높이를 정하기전에 먼저 관계를 확실히한다.
	Lcur->right = cur;
	if ( cur->left == NULL || cur->right == NULL )		//높이를 결정해주는 단계. Lcur은 높이 변화가 없고 cur의 높이만 변화가 있을수 있으므로 cur만 확인을 한다.
	{
		if ( cur->left != NULL )cur->height = 1;
		else if ( cur->right != NULL )cur->height = 1;
		else cur->height = 0;
	}
	else if ( cur->right->height > cur->left->height )
		cur->height = cur->right->height + 1;
	else
		cur->height = cur->left->height + 1;
	return Lcur;										//이 subtree에서 가장 꼭대기에 있게 되는 Lcur을 return한다. 

}
Position SingleRotateWithRight(Position node)			//위의 함수와 방향만 다르고 똑같다.
{
	Position cur = node;
	Position Rcur = cur->right;
	cur->right = Rcur->left;
	Rcur->left = cur;
	if ( cur->left == NULL || cur->right == NULL )
	{
		if ( cur->left != NULL )cur->height = 1;
		else if ( cur->right != NULL )cur->height = 1;
		else cur->height = 0;
	}
	else if ( cur->right->height > cur->left->height )
		cur->height = cur->right->height + 1;
	else 	
		cur->height = cur->left->height + 1;
	return Rcur;
	
}
Position DoubleRotateWithLeft(Position node)					//singlerotate를 두번 실행하는 방향으로 설계하였다.
{
	Position cur;
	int l_h = -1;
	int r_h = -1;
	cur = SingleRotateWithRight(node->left);					//먼저 node의 왼쪽부분을 right->left rotate를 실행한다. 그 다음 cur의 높이를 재조정해주고,
	if ( cur->left == NULL ) l_h = -1;		
	else l_h = cur->left->height;
	if ( cur->right == NULL ) r_h = -1;
	else r_h = cur->right->height;
	if ( l_h > r_h ) cur->height = l_h + 1;
	else cur->height = r_h + 1;
	node->left = cur; 
	cur = SingleRotateWithLeft(node);							//cur의 부모인 node를 left->right로 돌린다. 
	return cur;													//마찬가지로 subtree의 꼭대기에 있게되는 cur을 return한다.
}
Position DoubleRotateWithRight(Position node)					//방향만 반대이다.
{
	Position cur;
	int l_h = -1;
	int r_h = -1;
	cur = SingleRotateWithLeft(node->right);
	if ( cur->left == NULL ) l_h = -1;
	else l_h = cur->left->height;
	if ( cur->right == NULL ) r_h = -1;
	else r_h = cur->right->height;
	if ( l_h > r_h ) cur->height = l_h + 1;
	else cur->height = r_h + 1;
	node->right = cur;
	cur = SingleRotateWithRight(node);
	return cur;
}
void PrintInorder(AVLTree T)								//밑의 두 함수는 bst와 동일하게 설계하였다. 재귀 함수를 사용하여 fprintf와 free를 진행하였따.
{
	if ( T != NULL )
	{
		PrintInorder(T->left);
		fprintf(fout, "%d(%d) ", T->element, T->height);
		PrintInorder(T->right);
	}
}
void DeleteTree(AVLTree T)
{
	if ( T != NULL )
	{
		DeleteTree(T->left);
		DeleteTree(T->right);
		free(T);
	}
}
