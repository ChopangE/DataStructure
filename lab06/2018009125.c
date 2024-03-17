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
	T1 = (Position)malloc(sizeof(AVLNode));					//�ʱ�ȭ
	T1->height = 0;
	T1->left = NULL;
	T1->right = NULL;
	T1->element = X;
	if ( T == NULL )									//�����϶�
		return T1;
	cur = T;
	ccur = cur;
	while ( cur != NULL )					//���Ե� ��ġ ã��
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
		else                                 //cur->element = x ��� �̹� �����ϴ� ����̱� ������ �����޽����� ��ȯ.
		{
			fprintf(fout, "insertion error : %d is already in the tree!\n", X);
			return T;
		}
	}
	if ( dir == 0 )													//dir=0 �θ��� ���ʿ��� ,dir =1 �θ��� �����ʿ���.
		ccur->left = T1;
	else
		ccur->right = T1;
	cur = T;
	arr = (Position *)malloc(sizeof(Position) * count);				//PositionŸ���� �����ϴ� �迭�� ���� �̶� 78�� while������ count�� ������ �迭�� ũ�⸦ ����������. arr�� Position* �� ����������.
	for ( int i = 0; i < count; i++ )								//root���� ���ʴ�� �迭������ �������.
	{
		arr[i] = cur;
		if ( cur->element > X )
			cur = cur->left;
		else
			cur = cur->right;
	}
	for ( int i = count - 1; i >= 0; i-- )						//����������� ������ ������ '����'�� ���ϴ� ������ ����.
	{
		if ( i != 0 )											//i=0�̶�� �̰��� root�̱� ������ �θ� ��尡 �����Ƿ� ������ص���.
		{
			if ( arr[i - 1]->left == arr[i] ) dir = 0;
			else dir = 1;
		}															//���̸� ���� �����Ҷ� NULL����� ���̸� -1�� �����ؼ� ����.
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
			if ( l_h - r_h > 1 )         //rotate�� �ʿ��� ����.(�ڽİ��� �������̰� 2�̻󳭴�.)
			{
				if ( arr[i]->left->left == NULL )						//������ height�� �����Ƿ� ���ʳ���� �ڽĵ��� �����Ͽ� �̰��� singlerotate��Ȳ���� �ƴϸ� doublerotate��Ȳ���� Ȯ����.
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
					if ( dir == 0 ) arr[i - 1]->left = arr[i];   //�θ�� ����.
					else arr[i - 1]->right = arr[i];
				}
			}
			else    //�������� ��Ȳ(�ڽİ��� ���̰� 1���Ϸ� ���̳�)
			{
				arr[i]->height = ++l_h;
			}
		}															//���� �ڵ忡�� �¿츸 �ٲ����.
		else if ( l_h < r_h )
		{
			if ( r_h - l_h > 1 )					//rotate�� �ʿ��� ����.
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
			else    //�������� ��Ȳ(�ڽİ��� ���̰� 1���Ϸ� ���̳�.)
			{
				arr[i]->height = ++r_h;
			}

		}
		else
		{
			arr[i]->height = ++l_h;
		}
		
	}					//cur�� arr[0] �� root�� �����س������� free�����ϰ� cur�� ��ȯ��. free(arr)�� �迭�� free���� �� ���빰�� ���� free���� ����.
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
	if ( cur == NULL )										//X ���������� ������.
	{
		fprintf(fout, "deletion error : %d is not in the tree!\n", X);
		return T;
	}
	if ( count == 1 )is_root = 1;				//������ ��尡 root�� ��
	idx = count - 1;							//������ ����� indexǥ��.
	needed_del = cur;							//free�� ��带 ǥ��
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
		else//�ڽĳ�� ����.
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
		arr = (Position *)malloc(sizeof(Position) * (count-1));			//insert�� ���� �������� �����Ѵ�.
		cur = T;
		for ( int i = 0; i < count-1; i++ )
		{
			arr[i] = cur;
			if ( cur->element > X )
				cur = cur->left;
			else
				cur = cur->right;
		}//arr�ۼ��Ϸ�
		n = count - 1;									//�迭�� ���� ������ indexǥ��.
	}
	else//�ڽ� �Ѵ�����.
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
		if ( cur == cccur )  //������ ����� �����ڽ��� ������.
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
			if ( is_root == 0 )				//�����Ϸ��� ��尡 root�� �ƴҰ�� ����
			{
				if ( dir == 0 )				//�����Ϸ��� ��尡 �θ��� ���ʿ��� ������
				{
					cur->left = ccur->left->left;
					cur->right = ccur->left->right;
					ccur->left = cur;
				}
				else                            //�����Ϸ��� ��尡 �θ��� �����ʿ��� ���� ��
				{
					cur->left = ccur->right->left;
					cur->right = ccur->right->right;
					ccur->right = cur;
				}
			}
			else  //�����Ϸ��� ��尡 root�� ��
			{
				cur->left = T->left;
				cur->right = T->right;
				T = cur;
			}
		}
		cur = T;
		arr = (Position *)malloc(sizeof(Position) * (count - 1));
		for ( int i = 0; i < idx + 1; i++ )								//�켱 root���� ������ �������� �迭�� ��� �����ķδ� rightsubtree���� ���� ���� ��带 ã�� ������ ���İ� ��� ��带 �����Ѵ�.
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
	for ( int i = n - 1; i >= 0; i-- )										//insert�� �Ȱ��� ���������� �����ϸ� height�� ���� �������ִ� �ܰ�. insert�� �ڵ尡 �����ϴ�.
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
			if ( l_h - r_h > 1 )         //rotate�� �ʿ��� ����.
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
					if ( dir == 0 ) arr[i - 1]->left = arr[i];   //�θ�� ����.
					else arr[i - 1]->right = arr[i];
				}
			}
			else    //�������� ��Ȳ
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
			else    //�������� ��Ȳ
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


 // �Ѵ� ���� ��..

Position SingleRotateWithLeft(Position node)
{
	Position cur = node;
	Position Lcur = cur->left;
	cur->left = Lcur->right;							//���̸� ���ϱ����� ���� ���踦 Ȯ�����Ѵ�.
	Lcur->right = cur;
	if ( cur->left == NULL || cur->right == NULL )		//���̸� �������ִ� �ܰ�. Lcur�� ���� ��ȭ�� ���� cur�� ���̸� ��ȭ�� ������ �����Ƿ� cur�� Ȯ���� �Ѵ�.
	{
		if ( cur->left != NULL )cur->height = 1;
		else if ( cur->right != NULL )cur->height = 1;
		else cur->height = 0;
	}
	else if ( cur->right->height > cur->left->height )
		cur->height = cur->right->height + 1;
	else
		cur->height = cur->left->height + 1;
	return Lcur;										//�� subtree���� ���� ����⿡ �ְ� �Ǵ� Lcur�� return�Ѵ�. 

}
Position SingleRotateWithRight(Position node)			//���� �Լ��� ���⸸ �ٸ��� �Ȱ���.
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
Position DoubleRotateWithLeft(Position node)					//singlerotate�� �ι� �����ϴ� �������� �����Ͽ���.
{
	Position cur;
	int l_h = -1;
	int r_h = -1;
	cur = SingleRotateWithRight(node->left);					//���� node�� ���ʺκ��� right->left rotate�� �����Ѵ�. �� ���� cur�� ���̸� ���������ְ�,
	if ( cur->left == NULL ) l_h = -1;		
	else l_h = cur->left->height;
	if ( cur->right == NULL ) r_h = -1;
	else r_h = cur->right->height;
	if ( l_h > r_h ) cur->height = l_h + 1;
	else cur->height = r_h + 1;
	node->left = cur; 
	cur = SingleRotateWithLeft(node);							//cur�� �θ��� node�� left->right�� ������. 
	return cur;													//���������� subtree�� ����⿡ �ְԵǴ� cur�� return�Ѵ�.
}
Position DoubleRotateWithRight(Position node)					//���⸸ �ݴ��̴�.
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
void PrintInorder(AVLTree T)								//���� �� �Լ��� bst�� �����ϰ� �����Ͽ���. ��� �Լ��� ����Ͽ� fprintf�� free�� �����Ͽ���.
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
