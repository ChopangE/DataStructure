#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0  
#define FROMTHREAD 1  

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread
}ThreadedTree;

ThreadedPtr CreateTree(){
	ThreadedPtr tree = NULL;
	tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if(tree==NULL){
		fprintf(fout, "Out of Space!\n");
		return NULL;
	}
	tree->data = -1;
	tree->left_thread = 1;
	tree->left_child = tree;
	tree->right_thread = 1;
	tree->right_child = tree;
	return tree;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
	int from = 0;											//�ش� ��忡 ��� �����Ͽ������� �˷��ִ� ���� �Դϴ�. ���������� FROMTHREAD�� FROMEPARENT�� �ް� �˴ϴ�.
	int k = 1;												//¦�� idx�� ó���Ҷ� ���̴� �����Դϴ�. ������ ���� ���˴ϴ�.
	ThreadedPtr ptr = NULL;									//���ο� ��� ptr�� ����ϴ�. �׸��� �ʱ�ȭ(46~50line)���������մϴ�.
	ptr = (ThreadedPtr)malloc(sizeof(ThreadedTree));
	if ( ptr == NULL )
	{
		fprintf(fout, "Out of Space!\n");
		return 0;
	}
	ptr->data = data;
	ptr->left_thread = 1;
	ptr->left_child = ptr;
	ptr->right_thread = 1;
	ptr->right_child = ptr;
	if ( idx == 1 )											//idx�� 1 �� ��ó�� insert�� �Ǿ��� ���� ��Ȳ�Դϴ�. root�� ������ child�� ptr�� �����Ͽ��� ptr�� left�� rightchild�� root�̰� thread�� �Ѵ� 1�Դϴ�.
	{
		root->right_thread = 0;
		root->right_child = ptr;
		ptr->left_child = root;
		ptr->right_child = root;
	}														//idx�� 2 �� �ι�°�� insert�� ����Ǿ��� ���� ��Ȳ�Դϴ�. ptr�� left child�� root�� �����Ͽ��� ���� ����� leftchild�� ptr�� �����Ͽ����ϴ�.
	else if ( idx == 2 )
	{
		ptr->left_child = root;
		ptr->left_thread = 1;
		ptr->right_child = root->right_child;
		ptr->right_thread = 1;
		root->right_child->left_child = ptr;
		root->right_child->left_thread = 0;
	}
	else
	{                                                       //idx�� 3�̻��� �� �Դϴ�. ù��°�� �ι�°insert�� �����ϰ� ��� �� �ڵ带 ���󰡰� �˴ϴ�.
		if ( root == NULL ) return 0;
		ThreadedPtr cur = root->right_child;				//root�� NULL�̸� return 0�ϵ��� �ؼ� main�� �������� �Ͽ����ϴ�.
		if ( idx % 2 == 0 )									// idx�� ¦��, �� ����� �������� ������ �Ǵ� ����Դϴ�.
		{
			int n = idx;									//������ ��� ������ �����µ� ���ʳ����� �� idx�� ¦���� ���� ���� ��������ϴ�. �׷��� ��¿�� ���� �������� ������� �����Ͽ����ϴ�.
			while ( n != 1 )								// ���������۾��Դϴ�. idx�� ������� 18�̶�� k�� 8�� �ǵ��� �� 18���� ������ 2�� �ŵ��������� �ι�°�� ū 8�� k�� �����ϰ� �˴ϴ�.
			{												//�� �ι�°�� ū �ŵ������ΰ� ����� ù��° �б����� �������� ���ؼ� �����ϴ�. 18�� 16�� ���� depth�� �ֽ��ϴ�. �� �ֻ��� ���κ��� 4�� �������� �Ȱ��ε� �Ž��� 3�� �ö󰡰� �Ǹ� ù��° �б������� ���� ���ߴ��� �˼��� �ְԵǴ� ���Դϴ�. 
				n = n / 2;									//74���� 79�� ��¥ �ܼ��� idx���� ����, �ι�°�� ū 2�� �ŵ������� ���ϱ� ���� �����Դϴ�.
				k = k * 2;									
			}
			k = k / 2;										
			while ( k != 1 && k != 0)						//80���� 91���� while������ loop�� �ݺ��Ҷ����� k�� 2�� ���������� �˴ϴ�. ��, �б����� ��ĭ�� �������� ���� �ǹ��մϴ�. ex)idx�� 18�϶� k�� 8�̾��� �̴� ù��° �б⸦ ���ϰ� �ȴ�. ù��° �бⰡ ��������� k�� 2�γ������� �ι�° �б⸦ �����ϴ� ���� �˴ϴ�.
			{												//�� ���� k�� 1�� �ɶ������մϴ�. k�� 1�̶�� ��� �б⸦ ���� ���� �����̰� 18�� �θ� ��带 cur�� ����Ű�� �ִ� �����Դϴ�. �̶� �츮�� idx�� ¦���̹Ƿ� ������ ���ʿ� ��带 �����ϰ� �մϴ�. 
				if ( (idx / k) % 2 == 0 )					//82���� �ڵ�� idx�� k�� �������� 2�� ������ ���������� Ȯ���Ͽ����ϴ�. 2�� ����������ٸ� ¦���� ���ʺб⸦ ���ϰ� �Ͽ��� �ƴ϶�� Ȧ���� ������ �б⸦ ���ϰ� �Ͽ����ϴ�.
				{
					cur = cur->left_child;
				}
				else
				{
					cur = cur->right_child;
				}
				k = k / 2;
			}												// 92�� ���� 97�ٱ����� ������ ��� ���ʿ� ���ο� ��带 �����ϰ� �Ǵ� �����Դϴ�. ¦�������� ������ �������� �ʾ� ��¿������ �̷��� ����� ����Ͽ����ϴ�.. Ȥ�� ������ ���� �����Ѵٸ� chosw1999@hanyang.ac.kr�� �����ֽŴٸ� ���� ����帮�ڽ��ϴ�.
			ptr->right_child = cur;
			ptr->right_thread = 1;
			ptr->left_child = cur->left_child;
			ptr->left_thread = 1;
			cur->left_child = ptr;
			cur->left_thread = 0;
		}
		else //Ȧ��
		{													
			while ( !cur->left_thread )
			{
				cur = cur->left_child; //cur�� ���� ���� leaf�� ����Ű�� �˴ϴ�.
			}
			from = FROMPARENT;         // ���� leaf�� �������� Parent�� ���� �����Ƿ� from �� FROMPARENT�� �������ݴϴ�.
			while ( cur != root )
			{								// cur�� root�� ����ų�� ���� ��� ����˴ϴ�. �� ������ ��� inorder�� ���� ���� �˴ϴ�. �׷��� printinorder�� ���� ������ �ڵ尡 ���Ǿ����ϴ�.
				if ( from == FROMTHREAD )									
				{
					if ( !cur->right_thread )
					{
						cur = cur->right_child;
						while ( !cur->left_thread )cur = cur->left_child;
						from = FROMPARENT;
					}
					else
					{
						ptr->left_child = cur;
						ptr->left_thread = 1;
						ptr->right_child = cur->right_child;
						ptr->right_thread = 1;
						cur->right_child = ptr;
						cur->right_thread = 0;
						break;
					}
				}
				else
				{
					cur = cur->right_child;
					from = FROMTHREAD;
				}
			}
		}
	}
}
/*  108���� 135������ �ּ��Դϴ�. �켱 from�� ���̴ϴ� from �� FROMPARENT��� cur�� leaf��� ���̹Ƿ� �̰��� ������ �ڸ��� �ƴմϴ�. ���� cur�� cur�� rightchild�� ���󰡰� �մϴ�. �׸���from�� FROMTHREAD�� �ٲߴϴ�.
	from�� FROMPARENT��� ��� �ڽĳ�带 1���̻� ������ �ִ� node�Դϴ�. �̶�cur ������ child�� ���캾�ϴ�.���� child�� ���ٸ� �̰��� �ٷ� ���Ե� �ڸ��Դϴ�. �� ������ ���ʿ������� �����Ͱ� �ö�����Ƿ� ���ʿ��� �ڽ��� �ִٴ� ���ε� �����ʿ��� ���ٸ� ��������Ʈ���� ���ǿ� ��߳��� �ǹǷ� ���ڸ��� ������ ���־�ߵǱ⋚���Ӵϴ�.
	�װ� �ƴϰ� ���� cur�� ������ child�� �����Ѵٸ� �켱 ��ĭ�� ���������� �������ϴ�. �׸��� ó�� �ǿ��� leaf��带 ã���� ���� ���������� ��ӿ������� �޸��� right_thread�� ���� �������� �ö���� ������ �޽��ϴ�. idx�� Ȧ���̹Ƿ� ������ node�� �����ʿ� ������ �Ǿ���ϰ� ������ � node�� ���Ī���� child�� ������ �ֽ��ϴ�.
	������ �̿��� right_insert�� �����غ��ҽ��ϴ�. �׸��� �� ������ inorder�� �����մϴ�. 
	���Ե� ��ġ�� ã�Ѵٸ� ptr���忡�� ���ʿ� parent�� �ֱ⶧���� left_child�� parent�� �������ݴϴ�. �׷����� right_child�� parent�� right_child�� �����մϴ�. parent�� right_child�� ��� ������ ��尡 �ǰ� �˴ϴ�.
	��������� Ȧ�� ������ �������ϴ�. �׷��� ¦�� ���Կ��� ���� ����� Ȧ�� ���Կ����� ����� �� �ֽ��ϴ�. ������ Ȧ�� ���԰����� ���� �����̶�� �����ϱ⿡ ¦���ڵ�� Ȧ¦�� ���������ʰ� �ڵ带 ������ �Ǿ����ϴ�. 

*/
	

// printinorder�� insert���� Ȧ�� ���԰� �����մϴ�. ����  ����޾����� ������ data�� ����ϴ� �ڵ常 �����ϰ� �Ǿ����ϴ�. 
void printInorder(ThreadedPtr root){
	fprintf(fout, "inorder traversal : ");
	int from = 0;
	ThreadedPtr ptr = root->right_child;
	while ( !ptr->left_thread )
	{
		ptr = ptr->left_child;
	}
	from = FROMPARENT;
	while ( ptr != root )
	{
		if ( from == FROMTHREAD )
		{
			fprintf(fout, "%d ", ptr->data);
			if ( !ptr->right_thread )
			{
				ptr = ptr->right_child;
				while ( !ptr->left_thread ) ptr = ptr->left_child;
				from = FROMPARENT;
			}
			else
			{
				ptr = ptr->right_child;
				from = FROMTHREAD;
			}
		}
		else
		{
			fprintf(fout, "%d ", ptr->data);
			ptr = ptr->right_child;
			from = FROMTHREAD;
		}
	}



}
//DeleteTree�ڵ嵵 ������ insert , inorder�� �Ȱ����ϴ�. ���� �ٸ����� pptr�� �����Ͽ� ptr�� ������ �ڸ��� free�ϴ� ���� �ٸ��ϴ�.
void DeleteTree(ThreadedPtr root){
	int from = 0;
	ThreadedPtr ptr = root->right_child;
	ThreadedPtr pptr = root;
	while ( !ptr->left_thread )
	{
		ptr = ptr->left_child;
	}
	from = FROMPARENT;
	pptr = ptr;
	while ( ptr->right_child != root )
	{
		if ( from == FROMTHREAD )
		{
			if ( !ptr->right_thread )
			{
				pptr = ptr;
				ptr = ptr->right_child;
				free(pptr);
				while ( !ptr->left_thread ) 
				{

					ptr = ptr->left_child;
				}
				from = FROMPARENT;
			}
			else
			{
				pptr = ptr;
				ptr = ptr->right_child;
				free(pptr);
				from = FROMTHREAD;
			}
		}
		else
		{
			pptr = ptr;
			ptr = ptr->right_child;
			free(pptr);
			from = FROMTHREAD;
		}
	}
	free(ptr);
	free(root);
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		
		if(Insert(root, root_idx, data, idx) == 0){
			return 0;
		}
	}

	printInorder(root);
	DeleteTree(root);
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}
