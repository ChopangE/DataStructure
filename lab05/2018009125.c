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





Tree insertNode(Tree root, int key)                   //ptr에 메모리를 할당받은 뒤 초기화를 진행합니다.
{
	Tree ptr = (BST *)malloc(sizeof(BST));
	if ( ptr == NULL )
	{
		return NULL;
	}
	int i = 2;										//단순히 삽입하려는 위치가 parent의 왼쪽인지 오른쪽인지 확인하기 위한 변수입니다. 왼쪽에서 왔으면 0, 오른쪽에서 왔으면 1이라고 표시합니다. 처음에는 2로 초기화했습니다.
	ptr->value = key;
	ptr->left = NULL;
	ptr->right = NULL;
	if ( root == NULL )								//만약 root가 NULL이라면 root자리에 ptr을 삽입합니다.
	{
		root = ptr;
		fprintf(fout,"insert %d\n",key);
		return root;
	}
	Tree cur = root;								//cur은 삽입될 위치를 찾는 변수이고 ccur은 cur의 parent입니다.
	Tree ccur = cur;
	while ( cur != NULL )							//cur이 NULL즉 삽입될 자리를 찾을 때까지 while문을 반복합니다. 92줄과 97줄 if문에서 value를 비교해서 삽입할 value가 작으면 왼쪽으로 크면 오른쪽으로 가게 했습니다.
	{
		if ( cur->value > ptr->value ){
			i = 0;									//왼쪽으로 간다는 표시
			ccur = cur;
			cur = cur->left;
		}
		else if(cur->value < ptr->value){
			i = 1;									//오른쪽으로 간다는 표시
			ccur = cur;
			cur = cur->right;
		}											
		else                                       //만약 둘이 같다면 이미 존재한다는 error message를 띄우고 root값을 return 합니다.
		{
			fprintf(fout, "insertion error: %d is already in the tree\n", key);
			return root;
		}
	}
	if(i == 0){										//삽입될 위치를 찾은 다음 i == 0이라면 왼쪽에서 왔기때문에 parent의 left 즉 ccur->left 자리에 ptr을 삽입합니다.
		ccur->left = ptr;
	}
	else{											//오른쪽에서 왔다면 ccur->right 에 ptr을 삽입합니다.
		ccur->right = ptr;
	}
	fprintf(fout,"insert %d\n",key);				//삽입 됐다고 출력하고 root를 return 합니다.
	return root;
}
Tree deleteNode(Tree root, int key)				
{
	Tree _root = root;								//dir은 방향입니다 마찬가지로 왼쪽에서 왔으면 0 오른쪽에서 왔으면 1 삭제될 자리가 root라면 2를 표시합니다.
	Tree cur = root;
	Tree ccur = cur;
	int dir = 2;
	while ( cur->value != key )						//main문에서 이미 존재한다는 것을 확인하고 시행하는 함수이기 떄문에 해당 node가 나올때 까지 while문을 돌립니다.
	{
		if ( cur->value > key )						//insert와 유사합니다.
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
	}												//삭제될 노드인 cur을 찾았고 그 자식을 조사합니다.
	if ( cur->left == NULL && cur->right == NULL )	//자식이 없다면 그냥 cur만 삭제합니다.
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
	else if ( cur->left == NULL )					//오른쪽 자식만 존재한다면 dir==0 이면 부모인 ccur의 left에다가 cur->right를 삽입합니다. 그리고 cur을 없앱니다.dir==1일때도 마찬가지로 진행합니다. dir==2라면 root를 삭제하는 것이므로 _root를 cur->right로 설정해버립니다.
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
	}												//왼쪽 자식만 존재할떄에도 오른쪽 자식과 똑같이 하되 cur->left를 부모와 연결합니다.
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
	{											//삭제하려는 노드의 자식이 둘다 존재할 떄 입니다.
		if ( cur->right->left == NULL )			//삭제하려는 노드의 오른쪽의 왼쪽자식이 없다면 cur 자리를 cur->right로 대체합니다.
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
			else                                //삭제되려는 node가 root일떄의 상황입니다.
			{
				_root = cur->right;				//_root를 cur->rihgt로 설정하고 _root_left를 원래 root의 left로 바꿔줍니다.
				_root->left = cur->left;		
				free(cur);						//cur은 root입니다.
			}
		}
		else                                     //삭제하려는 노드의 오른쪽의 왼쪽자식이 존재할 떄입니다. 이경우 cur->right에서 계속 왼쪽 자식으로 갑니다. 이렇게되면 cur다음으로 큰수가 선택되는데 이수를 cur자리로 대체합니다.
		{
			cur = cur->right;
			while ( cur->left->left != NULL ) //cur->left는 삭제하려는 노드 바로 다음 으로 큰수를 가리키는 노드가 됩니다.
			{
				cur = cur->left;
			}
			Tree cccur = cur->left;				//cccur은 삭제된 노드 자리에 들어갈 노드입니다. 삭제될노드의 value바로 다음 수를 가지고 있습니다.
			if ( dir == 0 )
			{
				Tree d = ccur->left;				//d는 delete될 노드를 가리킵니다.
				cur->left = cccur->right;			//cccur의 left는 없지만 right는 남아있을수 있습니다. 따라서 cur은 cccur의 parent이므로 cccur의 right와 parent를 연결해줍니다.
				cccur->left = d->left;				//cccur이 d대신에 들어갈 빌드업을 합니다. cccur의 자식을 d의 자식노드로 바꿉니다.
				cccur->right = d->right;
				ccur->left = cccur;					//ccur은 d의 parent이므로 cccur과 parent도 연결해줍니다.
				free(d);							//d를 삭제합니다.
			}
			else if(dir == 1)					//dir==0과 똑같고 parent, ccur의 right가 d인것만 다릅니다. 
			{
				Tree d = ccur->right;
				cur->left = cccur->right;
				cccur->left = d->left;
				cccur->right = d->right;
				ccur->right = cccur;			//ccur의 right가 d였으므로 d대신 cccur을 넣습니다.
				free(d);
			}
			else
			{									//만약 root가 delete가 된다면 root의 parent는 없기때문에 _root를 cccur로 설정하고 _root의 자식노드를 진짜 root의 자식노드로 교체합니다.				
				cur->left = cccur->right;
				cccur->left = root->left;
				cccur->right = root->right;
				_root = cccur;
				free(root);
			}


		}
	}

	return _root;                               //최상위 root를 return합니다.
}
int findNode(Tree root, int key)				
{												//key보다 큰 value를 가진 node를 만나면 왼쪽으로 갑니다 key보다 작은 value를 가진 node를 만나면 오른쪽으로 갑니다.
	Tree cur = root;
	while (cur != NULL )						//cur이 NULL이 될때까지 반복합니다.
	{
		if ( cur->value > key )
			cur = cur->left;
		else if ( cur->value < key )
			cur = cur->right;
		else
			return 1;							//cur->value == key이면 tree안에 key가 있으므로 1을 반환합니다.
	}
	return 0;									//찾지못하면 0을 반환합니다.
}
void printInorder(Tree root)					//재귀를 사용하였습니다. 맨 왼쪽 노드를 모두다 조사한다음 자신의 value를 출력하고 그다음 오른쪽 노드를 모두다 조사합니다.
{
	if ( root != NULL )
	{
		printInorder(root->left);
		fprintf(fout, "%d ", root->value);
		printInorder(root->right);
	}
	
}
void deleteTree(Tree root)					//마찬가지로 재귀를 사용하였습니다. 본인의 자식 노드를 모두 조사한뒤 본인을 free합니다. 이렇게 되면 tree의 하단에 있는 node들 부터 free가 이뤄집니다.
{
	if ( root != NULL )
	{
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
}
