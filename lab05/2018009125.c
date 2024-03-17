#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}





Tree insertNode(Tree root, int key)                   //ptr�� �޸𸮸� �Ҵ���� �� �ʱ�ȭ�� �����մϴ�.
{
	Tree ptr = (BST *)malloc(sizeof(BST));
	if ( ptr == NULL )
	{
		return NULL;
	}
	int i = 2;										//�ܼ��� �����Ϸ��� ��ġ�� parent�� �������� ���������� Ȯ���ϱ� ���� �����Դϴ�. ���ʿ��� ������ 0, �����ʿ��� ������ 1�̶�� ǥ���մϴ�. ó������ 2�� �ʱ�ȭ�߽��ϴ�.
	ptr->value = key;
	ptr->left = NULL;
	ptr->right = NULL;
	if ( root == NULL )								//���� root�� NULL�̶�� root�ڸ��� ptr�� �����մϴ�.
	{
		root = ptr;
		fprintf(fout,"insert %d\n",key);
		return root;
	}
	Tree cur = root;								//cur�� ���Ե� ��ġ�� ã�� �����̰� ccur�� cur�� parent�Դϴ�.
	Tree ccur = cur;
	while ( cur != NULL )							//cur�� NULL�� ���Ե� �ڸ��� ã�� ������ while���� �ݺ��մϴ�. 92�ٰ� 97�� if������ value�� ���ؼ� ������ value�� ������ �������� ũ�� ���������� ���� �߽��ϴ�.
	{
		if ( cur->value > ptr->value ){
			i = 0;									//�������� ���ٴ� ǥ��
			ccur = cur;
			cur = cur->left;
		}
		else if(cur->value < ptr->value){
			i = 1;									//���������� ���ٴ� ǥ��
			ccur = cur;
			cur = cur->right;
		}											
		else                                       //���� ���� ���ٸ� �̹� �����Ѵٴ� error message�� ���� root���� return �մϴ�.
		{
			fprintf(fout, "insertion error: %d is already in the tree\n", key);
			return root;
		}
	}
	if(i == 0){										//���Ե� ��ġ�� ã�� ���� i == 0�̶�� ���ʿ��� �Ա⶧���� parent�� left �� ccur->left �ڸ��� ptr�� �����մϴ�.
		ccur->left = ptr;
	}
	else{											//�����ʿ��� �Դٸ� ccur->right �� ptr�� �����մϴ�.
		ccur->right = ptr;
	}
	fprintf(fout,"insert %d\n",key);				//���� �ƴٰ� ����ϰ� root�� return �մϴ�.
	return root;
}
Tree deleteNode(Tree root, int key)				
{
	Tree _root = root;								//dir�� �����Դϴ� ���������� ���ʿ��� ������ 0 �����ʿ��� ������ 1 ������ �ڸ��� root��� 2�� ǥ���մϴ�.
	Tree cur = root;
	Tree ccur = cur;
	int dir = 2;
	while ( cur->value != key )						//main������ �̹� �����Ѵٴ� ���� Ȯ���ϰ� �����ϴ� �Լ��̱� ������ �ش� node�� ���ö� ���� while���� �����ϴ�.
	{
		if ( cur->value > key )						//insert�� �����մϴ�.
		{
			ccur = cur;
			cur = cur->left;
			dir = 0;
		}
		else
		{
			ccur = cur;
			cur = cur->right;
			dir = 1;
		}
	}												//������ ����� cur�� ã�Ұ� �� �ڽ��� �����մϴ�.
	if ( cur->left == NULL && cur->right == NULL )	//�ڽ��� ���ٸ� �׳� cur�� �����մϴ�.
	{
		if ( dir == 0 )
		{
			free(cur);
			ccur->left = NULL;
		}
		else if ( dir == 1 )
		{
			free(cur);
			ccur->right = NULL;
		}
		else
		{
			free(root);
			return NULL;
		}
	}
	else if ( cur->left == NULL )					//������ �ڽĸ� �����Ѵٸ� dir==0 �̸� �θ��� ccur�� left���ٰ� cur->right�� �����մϴ�. �׸��� cur�� ���۴ϴ�.dir==1�϶��� ���������� �����մϴ�. dir==2��� root�� �����ϴ� ���̹Ƿ� _root�� cur->right�� �����ع����ϴ�.
	{
		if ( dir == 0 )
		{
			ccur->left = cur->right;
			free(cur);
		}
		else if(dir == 1)
		{
			ccur->right = cur->right;
			free(cur);
		}
		else
		{
			_root = cur->right;
			free(cur);
		}
	}												//���� �ڽĸ� �����ҋ����� ������ �ڽİ� �Ȱ��� �ϵ� cur->left�� �θ�� �����մϴ�.
	else if ( cur->right == NULL )
	{
		if ( dir == 0 )								
		{	
			ccur->left = cur->left;
			free(cur);
		}
		else if(dir == 1)
		{
			ccur->right = cur->left;
			free(cur);
		}
		else
		{
			_root = cur->left;
			free(cur);
			
		}
	}
	else
	{											//�����Ϸ��� ����� �ڽ��� �Ѵ� ������ �� �Դϴ�.
		if ( cur->right->left == NULL )			//�����Ϸ��� ����� �������� �����ڽ��� ���ٸ� cur �ڸ��� cur->right�� ��ü�մϴ�.
		{
			if ( dir == 0 )
			{
				ccur->left = cur->right;
				cur->right->left = cur->left;
				free(cur);
			}
			else if(dir == 1)
			{
				ccur->right = cur->right;
				cur->right->left = cur->left;
				free(cur);
			}
			else                                //�����Ƿ��� node�� root�ϋ��� ��Ȳ�Դϴ�.
			{
				_root = cur->right;				//_root�� cur->rihgt�� �����ϰ� _root_left�� ���� root�� left�� �ٲ��ݴϴ�.
				_root->left = cur->left;		
				free(cur);						//cur�� root�Դϴ�.
			}
		}
		else                                     //�����Ϸ��� ����� �������� �����ڽ��� ������ ���Դϴ�. �̰�� cur->right���� ��� ���� �ڽ����� ���ϴ�. �̷��ԵǸ� cur�������� ū���� ���õǴµ� �̼��� cur�ڸ��� ��ü�մϴ�.
		{
			cur = cur->right;
			while ( cur->left->left != NULL ) //cur->left�� �����Ϸ��� ��� �ٷ� ���� ���� ū���� ����Ű�� ��尡 �˴ϴ�.
			{
				cur = cur->left;
			}
			Tree cccur = cur->left;				//cccur�� ������ ��� �ڸ��� �� ����Դϴ�. �����ɳ���� value�ٷ� ���� ���� ������ �ֽ��ϴ�.
			if ( dir == 0 )
			{
				Tree d = ccur->left;				//d�� delete�� ��带 ����ŵ�ϴ�.
				cur->left = cccur->right;			//cccur�� left�� ������ right�� ���������� �ֽ��ϴ�. ���� cur�� cccur�� parent�̹Ƿ� cccur�� right�� parent�� �������ݴϴ�.
				cccur->left = d->left;				//cccur�� d��ſ� �� ������� �մϴ�. cccur�� �ڽ��� d�� �ڽĳ��� �ٲߴϴ�.
				cccur->right = d->right;
				ccur->left = cccur;					//ccur�� d�� parent�̹Ƿ� cccur�� parent�� �������ݴϴ�.
				free(d);							//d�� �����մϴ�.
			}
			else if(dir == 1)					//dir==0�� �Ȱ��� parent, ccur�� right�� d�ΰ͸� �ٸ��ϴ�. 
			{
				Tree d = ccur->right;
				cur->left = cccur->right;
				cccur->left = d->left;
				cccur->right = d->right;
				ccur->right = cccur;			//ccur�� right�� d�����Ƿ� d��� cccur�� �ֽ��ϴ�.
				free(d);
			}
			else
			{									//���� root�� delete�� �ȴٸ� root�� parent�� ���⶧���� _root�� cccur�� �����ϰ� _root�� �ڽĳ�带 ��¥ root�� �ڽĳ��� ��ü�մϴ�.				
				cur->left = cccur->right;
				cccur->left = root->left;
				cccur->right = root->right;
				_root = cccur;
				free(root);
			}


		}
	}

	return _root;                               //�ֻ��� root�� return�մϴ�.
}
int findNode(Tree root, int key)				
{												//key���� ū value�� ���� node�� ������ �������� ���ϴ� key���� ���� value�� ���� node�� ������ ���������� ���ϴ�.
	Tree cur = root;
	while (cur != NULL )						//cur�� NULL�� �ɶ����� �ݺ��մϴ�.
	{
		if ( cur->value > key )
			cur = cur->left;
		else if ( cur->value < key )
			cur = cur->right;
		else
			return 1;							//cur->value == key�̸� tree�ȿ� key�� �����Ƿ� 1�� ��ȯ�մϴ�.
	}
	return 0;									//ã�����ϸ� 0�� ��ȯ�մϴ�.
}
void printInorder(Tree root)					//��͸� ����Ͽ����ϴ�. �� ���� ��带 ��δ� �����Ѵ��� �ڽ��� value�� ����ϰ� �״��� ������ ��带 ��δ� �����մϴ�.
{
	if ( root != NULL )
	{
		printInorder(root->left);
		fprintf(fout, "%d ", root->value);
		printInorder(root->right);
	}
	
}
void deleteTree(Tree root)					//���������� ��͸� ����Ͽ����ϴ�. ������ �ڽ� ��带 ��� �����ѵ� ������ free�մϴ�. �̷��� �Ǹ� tree�� �ϴܿ� �ִ� node�� ���� free�� �̷����ϴ�.
{
	if ( root != NULL )
	{
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
}
