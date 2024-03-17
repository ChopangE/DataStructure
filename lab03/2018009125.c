#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack
{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack *CreateStack(int max);
void Push(Stack *S, int X);
int Pop(Stack *S);
int Top(Stack *S);

void DeleteStack(Stack *S);
int IsEmpty(Stack *S);
int IsFull(Stack *S);

void main(int argc, char *argv[])
{
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Stack *stack;
	char input_str[101];
	int max = 20, i = 0, a, b, result, error_flag = 0;

	fgets(input_str, 101, fin);
	stack = CreateStack(max);

	fprintf(fout, "top numbers : ");
	while ( input_str[i] != '#' )
	{
		if ( input_str[i] == '+' )									
		{
			if ( stack->top < 1 )							//+를 입력받았을 때 stack에 남아있는 원소의 개수가 1개이하일 때 error_flag 가 ISEMPTY가 나오게 하였다.
				error_flag = 2;
			else                                            //그게 아니라면 정상적으로 a에 top b에 그다음 원소를 입력받고 스택에 b+a를 입력하였다. 스택에 2개의 공간이 비게 되었으므로 IsFull로 확인할 필요는 없다.
			{
				a = Pop(stack);
				b = Pop(stack);
				Push(stack, b + a);
			}
		}
		else if ( input_str[i] == '-' )						//-를 입력받았을 때 마찬가지로 stack에 남아있는 원소의 개수가 1개 이하인지를 확인하고 오류메세지를 설정하였다.
		{
			if ( stack->top < 1 )
				error_flag = 2;
			else
			{												//그게 아니라면 a에 top b에 그 다음 원소를 입력받고 b-a를 스택에 입력하였다.
				a = Pop(stack);
				b = Pop(stack);
				Push(stack, b - a);
			}
		}
		else if ( input_str[i] == '*' )
		{													//*를 입력받았을 때 마찬가지로 stack에 남아있는 원소의 개수가 1개이하인지를 확인하고 오류메세지를 설정하였다.
			if ( stack->top < 1 )
				error_flag = 2;
			else
			{												//그게 아니라면 a에 top, b에 그다음 원소를 입력받고 b*a를 스택에 입력하였다.
				a = Pop(stack);
				b = Pop(stack);
				Push(stack, b * a);
			}
		}
		else if ( input_str[i] == '/' )
		{													//위의 연산자들과 동일하다.
			if ( stack->top < 1 )
				error_flag = 2;
			else
			{
				a = Pop(stack);
				b = Pop(stack);
				if ( a == 0 )								//여기가 차이점인데 만약 나누는 수인 a가 0이라면 error_flag를 DIVIDEZERO(3)으로 설정하였고 그게 아니라면 정상적으로 PUSH를 진행하였다.
					error_flag = 3;
				else
				{
					Push(stack, b / a);
				}
			}
		}
		else if ( input_str[i] == '%' )						// '/'와 동일하다.
		{
			if ( stack->top < 1 )
				error_flag = 2;
			else
			{
				a = Pop(stack);
				b = Pop(stack);
				if ( a == 0 )
					error_flag = 3;							
				else
				{
					Push(stack, b % a);
				}
			}
		}
		else                                              //operand를 입력받는 코드이다. 110줄에서 스택이 가득 찼는지 확인하고 가득찼으면 error_flag를 ISFULL(1)로 설정하였다.
		{
			if ( IsFull(stack) )
				error_flag = 1;
			else
			{											//스택이 가득차있지 않다면 114줄에서 문자를 int로 바꾸는 작업을 진행하고 push한다. 이때 상대적 위치로 '0'으로부터 입력받은수가 얼마나 떨어져있는지를 확인하는 방법을 사용하였다.
				a = input_str[i] - '0';
				Push(stack, a);
			}
		}
		if ( error_flag ) break;                       //error_flag가 0이면 정상이고 0이아니라면 error가발생했다는 뜻이므로 그떄에는 break를 진행해 while문을 빠져나간다 그리고 126줄부터137줄에 해당하는 코드를 통해 오류를 출력하게딘다.					

		int t = Top(stack);							//Top함수를 통해 stack맨위에 있는 key를 t에 저장한다. 바로 다음 줄에서 이것을 출력하고 result에 결과로서 저장한다. 모든 계산이 끝났을 때 result에 있는 수가 최종 결과로 출력되게 된다.
		fprintf(fout, "%d ", t);
		result = t;
		i++;										//input.txt의 다음 문자를 읽기 위해 i++를 시행한다.
	}

	if ( error_flag == ISFULL )																//오류내용을 출력하는 코드들이다.
	{
		fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
	}
	else if ( error_flag == ISEMPTY )
	{
		fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
	}
	else if ( error_flag == DIVIDEZERO )
	{
		fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
	}
																							//정상적으로 while문을 빠져나오게되면 error_flag가 0이어서 else문으로 오게된다.
	else                                                                                    //141줄의 if문을 통해 혹시라도 스택에 남아있는 key가 2개 이상인지를 확인한다 이때 스택에 key가 하나만 남아있다면 stack->top 이 0 이므로 stack->top+1을 한것이 stack에 남아있는 key의 개수를 나타내게 된다.
	{
		if ( stack->top + 1 > 1 )															//남아있는 key가 2개이상이라면 error문을 띄우고 stack->top+1을 출력하는데 이것이 남아있는 key의 수다.
		{
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top + 1);
		}
		else                                                                               //결국 모든것이 정상적으로 작동되었으면 result를 출력하게된다.
		{
			fprintf(fout, "\nevaluation result : %d\n", result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack *CreateStack(int max)																//157에서 임의의 stack을 생성하고 158에서 동적할당을한다. 이떄 할당이 성공했다면 161~163을 실행한다. 162에선 key에 max에 해당하는 수만큼의 메모리를 할당하여 배열로서 사용되도록하였다. 
{
	Stack *sta;
	sta = (Stack *)malloc(sizeof(Stack));
	if ( sta != NULL )
	{
		sta->max_stack_size = max;
		sta->key = (int *)malloc(sizeof(int) * max);
		sta->top = -1;																	//초기의 top의 index는 -1이다.
	}
	return sta;																			//정상적으로 동적할당되었다면 적절한 값들이 입력된 stack을 반환하지만 그렇지 않다면 NULL을 반환하게된다.
}

void DeleteStack(Stack *S)																//free(S->key)를 통해 key에 할당된 메모리를 반환하고 이어서 stack에 할당되었던 메모리도 반환한다.
{
	free(S->key);
	free(S);
}

void Push(Stack *S, int X)															//먼저 IsFull을 통해 스택이 가득찼는지 확인하고 가득찼으면 즉시 함수를 종료시킨다. 근데 사실 main문에서 이미 이 검사를 하기떄문에 실제로 이코드가 작동될 일은 없다.
{
	if ( IsFull(S) )															
	{
		return;
	}
	else
		S->key[++S->top] = X;														//stack이 가득차지 않았다면 현재 S->top에 +1을 한뒤 그곳에 X를 PUSH한다.
}

int Pop(Stack *S)																	//IsEmpty를 통해 스택이 비었는지를 먼저 확인하고 비었으면 -1를 반환한다. 사실 main에서 이미 검사를 하기때문에 실제로 이코드가 작동될 일은 없다.
{
	if ( IsEmpty(S) )
	{
		return  -1;
	}
	return S->key[S->top--];													//S->top에 해당하는 키를 반환하고 S->top을 -1한다.
}

int Top(Stack *S)																//S->top에 해당하는 key를 반환한다.
{
	return S->key[S->top];
}

int IsFull(Stack *S)															//S->top +1 이 스택에 들어있는 key의 수 이므로 이것이 max_stack_size와 같다면 stack이 가득 차있는 것이다.
{
	return S->top+1 == S->max_stack_size;
}

int IsEmpty(Stack *S)															//stack이 모두 비어있을땐 S->top 이 -1이므로 이것을 확인한다.
{
	return S->top == -1;
}
