#ifndef ARRSTACK
#define ARRSTACK
#define MAX_STACK_SIZE 1000
typedef struct ArrStack
{
	int stack[MAX_STACK_SIZE];
	int top;
} ArrStack;

int stackEmpty(ArrStack *stk);
int initStack(ArrStack *stk);
int push(ArrStack *stk, int num);
int pop(ArrStack *stk, int *status);
int top(ArrStack *stk, int *status);
#endif
