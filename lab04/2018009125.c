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
	int from = 0;											//해당 노드에 어떻게 접근하였는지를 알려주는 변수 입니다. 변수값으론 FROMTHREAD와 FROMEPARENT를 받게 됩니다.
	int k = 1;												//짝수 idx를 처리할때 쓰이는 변수입니다. 나누는 수로 사용됩니다.
	ThreadedPtr ptr = NULL;									//새로운 노드 ptr을 만듭니다. 그리고 초기화(46~50line)까지진행합니다.
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
	if ( idx == 1 )											//idx가 1 즉 맨처음 insert가 되었을 때의 상황입니다. root의 오른쪽 child로 ptr을 설정하였고 ptr의 left와 rightchild는 root이고 thread는 둘다 1입니다.
	{
		root->right_thread = 0;
		root->right_child = ptr;
		ptr->left_child = root;
		ptr->right_child = root;
	}														//idx가 2 즉 두번째로 insert가 시행되었을 때의 상황입니다. ptr의 left child를 root로 설정하였고 이전 노드의 leftchild로 ptr을 설정하였습니다.
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
	{                                                       //idx가 3이상일 떄 입니다. 첫번째와 두번째insert를 제외하곤 모두 이 코드를 따라가게 됩니다.
		if ( root == NULL ) return 0;
		ThreadedPtr cur = root->right_child;				//root가 NULL이면 return 0하도록 해서 main을 끝내도록 하였습니다.
		if ( idx % 2 == 0 )									// idx가 짝수, 즉 노드의 왼쪽으로 삽입이 되는 경우입니다.
		{
			int n = idx;									//오른쪽 노드 삽입은 쉬웠는데 왼쪽노드삽입 즉 idx가 짝수일 떄가 무지 힘들었습니다. 그래서 어쩔수 없이 원시적인 방법으로 진행하였습니다.
			while ( n != 1 )								// 수학적인작업입니다. idx가 예를들어 18이라면 k가 8이 되도록 즉 18보다 작지만 2의 거듭제곱에서 두번째로 큰 8을 k로 설정하게 됩니다.
			{												//왜 두번째로 큰 거듭제곱인가 결론은 첫번째 분기점을 결정짓기 위해서 였습니다. 18은 16과 같은 depth에 있습니다. 즉 최상위 노드로부터 4번 내려오게 된것인데 거슬러 3번 올라가게 되면 첫번째 분기점에서 어디로 향했는지 알수가 있게되는 것입니다. 
				n = n / 2;									//74에서 79는 진짜 단순히 idx보다 작은, 두번째로 큰 2의 거듭제곱을 구하기 위한 과정입니다.
				k = k * 2;									
			}
			k = k / 2;										
			while ( k != 1 && k != 0)						//80에서 91까지 while문에서 loop를 반복할때마다 k는 2로 나누어지게 됩니다. 즉, 분기점이 한칸씩 내려오는 것을 의미합니다. ex)idx가 18일때 k는 8이었고 이는 첫번째 분기를 정하게 된다. 첫번째 분기가 정해지고는 k는 2로나누어져 두번째 분기를 결정하는 수가 됩니다.
			{												//이 짓을 k가 1이 될때까지합니다. k가 1이라면 모든 분기를 결정 지은 상태이고 18의 부모 노드를 cur이 가리키고 있는 상태입니다. 이때 우리는 idx가 짝수이므로 무조건 왼쪽에 노드를 삽입하게 합니다. 
				if ( (idx / k) % 2 == 0 )					//82줄의 코드는 idx를 k로 나눈값이 2로 나누어 떨어지는지 확인하였습니다. 2로 나누어떨어진다면 짝수인 왼쪽분기를 택하게 하였고 아니라면 홀수인 오른쪽 분기를 택하게 하였습니다.
				{
					cur = cur->left_child;
				}
				else
				{
					cur = cur->right_child;
				}
				k = k / 2;
			}												// 92줄 부터 97줄까지는 마지막 노드 왼쪽에 새로운 노드를 삽입하게 되는 과정입니다. 짝수삽입이 마땅히 떠오르지 않아 어쩔수없이 이러한 방식을 사용하였습니다.. 혹시 정답이 따로 존재한다면 chosw1999@hanyang.ac.kr로 보내주신다면 정말 감사드리겠습니다.
			ptr->right_child = cur;
			ptr->right_thread = 1;
			ptr->left_child = cur->left_child;
			ptr->left_thread = 1;
			cur->left_child = ptr;
			cur->left_thread = 0;
		}
		else //홀수
		{													
			while ( !cur->left_thread )
			{
				cur = cur->left_child; //cur은 가장 왼쪽 leaf를 가르키게 됩니다.
			}
			from = FROMPARENT;         // 왼쪽 leaf는 마지막에 Parent로 부터 왔으므로 from 을 FROMPARENT로 설정해줍니다.
			while ( cur != root )
			{								// cur이 root를 가리킬때 까지 계속 진행됩니다. 이 과정은 사실 inorder를 따라 가게 됩니다. 그래서 printinorder와 거의 동일한 코드가 사용되었습니다.
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
/*  108부터 135까지의 주석입니다. 우선 from을 살핍니다 from 이 FROMPARENT라면 cur가 leaf라는 뜻이므로 이곳은 삽입할 자리가 아닙니다. 따라서 cur를 cur의 rightchild를 따라가게 합니다. 그리고from을 FROMTHREAD로 바꿉니다.
	from이 FROMPARENT라면 적어도 자식노드를 1개이상 가지고 있는 node입니다. 이때cur 오른쪽 child를 살펴봅니다.만약 child가 없다면 이곳이 바로 삽입될 자리입니다. 그 이유는 왼쪽에서부터 포인터가 올라왔으므로 왼쪽에는 자식이 있다는 뜻인데 오른쪽에는 없다면 완전이진트리의 정의에 어긋나게 되므로 그자리에 삽입을 해주어야되기떄문임니다.
	그게 아니고 만약 cur의 오른쪽 child가 존재한다면 우선 한칸만 오른쪽으로 내려갑니다. 그리고 처음 맨왼쪽 leaf노드를 찾았을 떄와 마찬가지로 계속왼쪽으로 달리고 right_thread를 통해 차근차근 올라오는 과정을 겪습니다. idx는 홀수이므로 무조건 node의 오른쪽에 삽입이 되어야하고 무조건 어떤 node는 비대칭적인 child를 가지고 있습니다.
	그점을 이용해 right_insert를 진행해보았습니다. 그리고 이 과정은 inorder와 동일합니다. 
	삽입될 위치를 찾앗다면 ptr입장에선 왼쪽에 parent가 있기때문에 left_child를 parent로 설정해줍니다. 그런다음 right_child를 parent의 right_child로 설정합니다. parent의 right_child는 방금 삽입한 노드가 되게 됩니다.
	상대적으로 홀수 삽입은 쉬웠습니다. 그런데 짝수 삽입에서 사용된 방법을 홀수 삽입에서도 사용할 수 있습니다. 하지만 홀수 삽입과정은 제가 정답이라고 생각하기에 짝수코드로 홀짝을 통합하지않고 코드를 나누게 되었습니다. 

*/
	

// printinorder는 insert에서 홀수 삽입과 동일합니다. 단지  지목받았을때 본인의 data를 출력하는 코드만 삽입하게 되었습니다. 
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
//DeleteTree코드도 솔직히 insert , inorder와 똑같습니다. 단지 다른점은 pptr을 설정하여 ptr이 지나간 자리를 free하는 것이 다릅니다.
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
