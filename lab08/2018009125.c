#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
    int order;
    int size;           /* number of children */
    BNodePtr *child;    /* children pointers */
    int *key;           /* keys */
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);

void Insert(BNodePtr *root, int key);
BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int pos);
BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos);
BNodePtr Createnode(int order);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);
void free_node(BNodePtr node);



int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while(!feof(fin)){
        fscanf(fin, "%c", &cv);
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(&root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 0)
                    fprintf(fout, "print error : tree is empty!");
                else
                    PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }
   
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

/* 새로운 트리를 생성하는 함수 */
BNodePtr CreateTree(int order){
    BNodePtr root;
    root = (BNodePtr)malloc(sizeof(BNode));
    root->order = order;
    root->size = 0;
    root->child = (BNodePtr *)malloc(sizeof(BNodePtr) * (root->order + 1));
    root->key = (int *)malloc(sizeof(int) * (root->order));
    root->is_leaf = 1;
    return root;
}

/* insert key */
void Insert(BNodePtr *root, int key){ // 루트를 함수 내에서 변경시키기 위해 이중 포인터로 선언
    *root = Insert_sub(*root, *root, key, 0); // 리턴값이 있는 재귀함수 활용, 처음에 root 노드는 현재 노드이자 parent이므로 같이 parent, node 똑같이 매개변수 전달
}

BNodePtr Insert_sub(BNodePtr parent, BNodePtr node, int key, int p_pos){
    int pos;
    int i;
    if ( parent == node && node->size == 0 )                //처음으로 insert가 될때
    {
        node->key[0] = key;
        node->size++;
        return node;
    }
    for ( pos = 0; pos < node->size; pos++ )
    {
        if ( key < node->key[pos] ) break;
    }
    if ( node->is_leaf == 0 )
    {
        node->child[pos] = Insert_sub(node, node->child[pos], key, pos);
        if ( node->size >= node->order )
        {
            node = split_node(parent, node, p_pos);
        }
    }
    else
    {
        if ( pos < node->size )
        {
            for ( i = node->size; i > pos; i-- )    //오른쪽으로한칸씩밀기
            {
                node->key[i] = node->key[i - 1];
            }
        }
        node->key[pos] = key;                           //옳은 자리에 삽입
        node->size++;                                   //새로삽입했기때문에 size+1
        if ( node->size >= node->order )                //만약 siez와 order가 같다면 full이므로 split을 진행
        {
            node = split_node(parent, node, p_pos);
        }
    }
    return node;
   
}

BNodePtr split_node(BNodePtr parent, BNodePtr node, int pos){
    int mid = (node->order - 1) / 2;
    int i;
    BNodePtr right_child;
    BNodePtr new_root;
    right_child = Createnode(node->order);              
    if ( node->is_leaf )   right_child->is_leaf = 1;                //원래 node가 leaf면 right_child도 leaf이다.
    else right_child->is_leaf = 0;
    right_child->size = 0;                                  //처음 생성됐기 떄문에 size = 0
    for ( i = mid + 1; i < node->size; i++ )                //mid+1부터 마지막key까지 rihgt_child로 이동시킨다.
    {
        right_child->key[i - (mid + 1)] = node->key[i];
        right_child->size++;
    }
    node->size = mid;                                       //node(left_child)의 사이즈는 mid가 된다.
    if ( node->is_leaf == 0 )                                       //만약 현재 node가 leaf가 아니었다면 child가 존재하므로 그것도 right_child로 옮긴다.
    {
        for ( i = mid + 1; i < node->order+1; i++ )
        {
            right_child->child[i - (mid + 1)] = node->child[i];
        }
    }
    if ( parent == node )                                   //split의 해당노드가 root라면  새로운 root를 생성한다.
    {
        new_root = Createnode(node->order);                 //초기화과정
        new_root->is_leaf = 0;
        new_root->key[0] = node->key[mid];
        new_root->size = 1;
        new_root->child[0] = node;
        new_root->child[1] = right_child;
        return new_root;
    }
    else                                                    //split의 해당노드가 root가 아닐때.
    {
        if ( pos < parent->size )                               //pos가 parent->size보다 작다면 오른쪽으로 한칸씩민다.
        {
            for ( i = parent->size; i > pos; i-- )
            {
                parent->key[i] = parent->key[i - 1];
                parent->child[i + 1] = parent->child[i];
            }
        }
        parent->key[pos] = node->key[mid];
        parent->size++;
        parent->child[pos + 1] = right_child;
    }
    return node;
}

/* 노드 하나를 동적할당하는 함수, size, is_leaf는 노드마다 다르므로 초기화하지 않고 공통되는 부분만 초기화 */
BNodePtr Createnode(int order){ 
    BNodePtr new;
    new = (BNodePtr)malloc(sizeof(BNode));
    new->order = order;
    new->child = (BNodePtr *)malloc(sizeof(BNodePtr) * (new->order + 1));
    new->key = (int *)malloc(sizeof(int) * (new->order));
    return new;
}

/* find node that has key */
int Find(BNodePtr root, int key){
    BNodePtr ptr = root;
    int i;
    for ( i = 0; i < ptr->size; i++ )
    {
        if ( ptr->key[i] == key ) return 1;
        if ( key < ptr->key[i] )break;
    }
    if ( ptr->is_leaf )
    {
        return 0;
    }
    return Find(ptr->child[i], key);
}

/* print Tree, inorder traversal */
void PrintTree(BNodePtr root){
    int i;
    BNodePtr ptr = root;
    if ( ptr->is_leaf == 0 )
    {
        for ( i = 0; i < ptr->size+1; i++ )
        {
            PrintTree(ptr->child[i]);
            if ( i < ptr->size )
            {
                fprintf(fout, "%d ", ptr->key[i]);
            }
        }
    }
    else
    {
        for ( i = 0; i < ptr->size; i++ )
        {
            fprintf(fout, "%d ", ptr->key[i]);
        }
    }
}

/* depth first search -> free memory */
void DeleteTree(BNodePtr root){
    int i;
    BNodePtr ptr = root;
    if ( ptr->is_leaf == 0 )
    {
        for ( i = 0; i < ptr->size + 1; i++ )
        {
            DeleteTree(ptr->child[i]);
        }
    }
    free_node(ptr);    
}

/* 노드의 메모리를 해제해주는 helper function */
void free_node(BNodePtr node){
    free(node->child);
    free(node->key);
    free(node);
}
