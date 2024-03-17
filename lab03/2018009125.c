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
			if ( stack->top < 1 )							//+�� �Է¹޾��� �� stack�� �����ִ� ������ ������ 1�������� �� error_flag �� ISEMPTY�� ������ �Ͽ���.
				error_flag = 2;
			else                                            //�װ� �ƴ϶�� ���������� a�� top b�� �״��� ���Ҹ� �Է¹ް� ���ÿ� b+a�� �Է��Ͽ���. ���ÿ� 2���� ������ ��� �Ǿ����Ƿ� IsFull�� Ȯ���� �ʿ�� ����.
			{
				a = Pop(stack);
				b = Pop(stack);
				Push(stack, b + a);
			}
		}
		else if ( input_str[i] == '-' )						//-�� �Է¹޾��� �� ���������� stack�� �����ִ� ������ ������ 1�� ���������� Ȯ���ϰ� �����޼����� �����Ͽ���.
		{
			if ( stack->top < 1 )
				error_flag = 2;
			else
			{												//�װ� �ƴ϶�� a�� top b�� �� ���� ���Ҹ� �Է¹ް� b-a�� ���ÿ� �Է��Ͽ���.
				a = Pop(stack);
				b = Pop(stack);
				Push(stack, b - a);
			}
		}
		else if ( input_str[i] == '*' )
		{													//*�� �Է¹޾��� �� ���������� stack�� �����ִ� ������ ������ 1������������ Ȯ���ϰ� �����޼����� �����Ͽ���.
			if ( stack->top < 1 )
				error_flag = 2;
			else
			{												//�װ� �ƴ϶�� a�� top, b�� �״��� ���Ҹ� �Է¹ް� b*a�� ���ÿ� �Է��Ͽ���.
				a = Pop(stack);
				b = Pop(stack);
				Push(stack, b * a);
			}
		}
		else if ( input_str[i] == '/' )
		{													//���� �����ڵ�� �����ϴ�.
			if ( stack->top < 1 )
				error_flag = 2;
			else
			{
				a = Pop(stack);
				b = Pop(stack);
				if ( a == 0 )								//���Ⱑ �������ε� ���� ������ ���� a�� 0�̶�� error_flag�� DIVIDEZERO(3)���� �����Ͽ��� �װ� �ƴ϶�� ���������� PUSH�� �����Ͽ���.
					error_flag = 3;
				else
				{
					Push(stack, b / a);
				}
			}
		}
		else if ( input_str[i] == '%' )						// '/'�� �����ϴ�.
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
		else                                              //operand�� �Է¹޴� �ڵ��̴�. 110�ٿ��� ������ ���� á���� Ȯ���ϰ� ����á���� error_flag�� ISFULL(1)�� �����Ͽ���.
		{
			if ( IsFull(stack) )
				error_flag = 1;
			else
			{											//������ ���������� �ʴٸ� 114�ٿ��� ���ڸ� int�� �ٲٴ� �۾��� �����ϰ� push�Ѵ�. �̶� ����� ��ġ�� '0'���κ��� �Է¹������� �󸶳� �������ִ����� Ȯ���ϴ� ����� ����Ͽ���.
				a = input_str[i] - '0';
				Push(stack, a);
			}
		}
		if ( error_flag ) break;                       //error_flag�� 0�̸� �����̰� 0�̾ƴ϶�� error���߻��ߴٴ� ���̹Ƿ� �׋����� break�� ������ while���� ���������� �׸��� 126�ٺ���137�ٿ� �ش��ϴ� �ڵ带 ���� ������ ����ϰԵ��.					

		int t = Top(stack);							//Top�Լ��� ���� stack������ �ִ� key�� t�� �����Ѵ�. �ٷ� ���� �ٿ��� �̰��� ����ϰ� result�� ����μ� �����Ѵ�. ��� ����� ������ �� result�� �ִ� ���� ���� ����� ��µǰ� �ȴ�.
		fprintf(fout, "%d ", t);
		result = t;
		i++;										//input.txt�� ���� ���ڸ� �б� ���� i++�� �����Ѵ�.
	}

	if ( error_flag == ISFULL )																//���������� ����ϴ� �ڵ���̴�.
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
																							//���������� while���� ���������ԵǸ� error_flag�� 0�̾ else������ ���Եȴ�.
	else                                                                                    //141���� if���� ���� Ȥ�ö� ���ÿ� �����ִ� key�� 2�� �̻������� Ȯ���Ѵ� �̶� ���ÿ� key�� �ϳ��� �����ִٸ� stack->top �� 0 �̹Ƿ� stack->top+1�� �Ѱ��� stack�� �����ִ� key�� ������ ��Ÿ���� �ȴ�.
	{
		if ( stack->top + 1 > 1 )															//�����ִ� key�� 2���̻��̶�� error���� ���� stack->top+1�� ����ϴµ� �̰��� �����ִ� key�� ����.
		{
			fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top + 1);
		}
		else                                                                               //�ᱹ ������ ���������� �۵��Ǿ����� result�� ����ϰԵȴ�.
		{
			fprintf(fout, "\nevaluation result : %d\n", result);
		}
	}
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack *CreateStack(int max)																//157���� ������ stack�� �����ϰ� 158���� �����Ҵ����Ѵ�. �̋� �Ҵ��� �����ߴٸ� 161~163�� �����Ѵ�. 162���� key�� max�� �ش��ϴ� ����ŭ�� �޸𸮸� �Ҵ��Ͽ� �迭�μ� ���ǵ����Ͽ���. 
{
	Stack *sta;
	sta = (Stack *)malloc(sizeof(Stack));
	if ( sta != NULL )
	{
		sta->max_stack_size = max;
		sta->key = (int *)malloc(sizeof(int) * max);
		sta->top = -1;																	//�ʱ��� top�� index�� -1�̴�.
	}
	return sta;																			//���������� �����Ҵ�Ǿ��ٸ� ������ ������ �Էµ� stack�� ��ȯ������ �׷��� �ʴٸ� NULL�� ��ȯ�ϰԵȴ�.
}

void DeleteStack(Stack *S)																//free(S->key)�� ���� key�� �Ҵ�� �޸𸮸� ��ȯ�ϰ� �̾ stack�� �Ҵ�Ǿ��� �޸𸮵� ��ȯ�Ѵ�.
{
	free(S->key);
	free(S);
}

void Push(Stack *S, int X)															//���� IsFull�� ���� ������ ����á���� Ȯ���ϰ� ����á���� ��� �Լ��� �����Ų��. �ٵ� ��� main������ �̹� �� �˻縦 �ϱ⋚���� ������ ���ڵ尡 �۵��� ���� ����.
{
	if ( IsFull(S) )															
	{
		return;
	}
	else
		S->key[++S->top] = X;														//stack�� �������� �ʾҴٸ� ���� S->top�� +1�� �ѵ� �װ��� X�� PUSH�Ѵ�.
}

int Pop(Stack *S)																	//IsEmpty�� ���� ������ ��������� ���� Ȯ���ϰ� ������� -1�� ��ȯ�Ѵ�. ��� main���� �̹� �˻縦 �ϱ⶧���� ������ ���ڵ尡 �۵��� ���� ����.
{
	if ( IsEmpty(S) )
	{
		return  -1;
	}
	return S->key[S->top--];													//S->top�� �ش��ϴ� Ű�� ��ȯ�ϰ� S->top�� -1�Ѵ�.
}

int Top(Stack *S)																//S->top�� �ش��ϴ� key�� ��ȯ�Ѵ�.
{
	return S->key[S->top];
}

int IsFull(Stack *S)															//S->top +1 �� ���ÿ� ����ִ� key�� �� �̹Ƿ� �̰��� max_stack_size�� ���ٸ� stack�� ���� ���ִ� ���̴�.
{
	return S->top+1 == S->max_stack_size;
}

int IsEmpty(Stack *S)															//stack�� ��� ��������� S->top �� -1�̹Ƿ� �̰��� Ȯ���Ѵ�.
{
	return S->top == -1;
}
