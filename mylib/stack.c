//
// Created by Raven on 2018/7/26.
//


#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
//----------------顺序栈实现------------
/**
 * 对栈s进行相应的初始化
 * @param s
 * @return OK
 *         ERROR
 */
Status init_stack(Sqstack *s)
{
	s->base = (ElemType *)malloc(INIT_SIZE * sizeof(ElemType));
	if (!s->base)
	{
		exit(0);
	}
	s->top = s->base;
	s->stack_size = INIT_SIZE;
	return 0;
}
/**
 * 判定栈s是否为空
 * @param s
 * @return
 */
Status is_empty(Sqstack *s)
{
	if (s->base == s->top)
	{
		return 1;
	}
	return 0;
}
int stack_length(Sqstack *s)
{
	return s->top - s->base;
}
/**
 * 取栈s的栈顶元素，以e作为返回值
 * @param s
 * @param e
 * @return
 */
Status get_top(Sqstack *s, ElemType *e)
{
	if (s->top == s->base)
	{
		printf("stack get top error\n");
		exit(-1);
	}
	*e = *(s->top - 1);
	return 0;
}
/**
 * 将e压如栈s
 * @param s
 * @param e
 * @return OK
 *          ERROR
 */
Status push(Sqstack *s, ElemType *e)
{
	if (s->top - s->base >= s->stack_size) //如果当前空间已经不足
	{
		ElemType *newbase = (ElemType *)realloc(s->base,
												(s->stack_size + INCREMENT) * sizeof(ElemType));
		if (!newbase) return -1;
		s->base = newbase;               //realloc 返回的地址可能与参数中的地址相同，也可能不同，所以需要重新指向
		s->top = s->base + s->stack_size;   //原因同上
		s->stack_size += INCREMENT;
	}
	*(s->top++) = *e;
	return 0;
}

/**
 * 栈s栈顶元素弹出，以e作为返回值
 * @param s
 * @param e
 * @return
 */
Status pop(Sqstack *s, ElemType *e)
{
	if (s->base == s->top) exit(-1);
	*e = *(--(s->top));
	return 0;
}
/**
 * 将栈s置为空栈
 * @param s
 * @return
 */
Status clear_stack(Sqstack *s)
{
	s->top = s->base;
	return 0;
}
/**
 * 销毁栈s
 * @param s
 * @return
 */
Status destory_stack(Sqstack *s)
{
	free(s->base);
	s->base = NULL;
	s->top = NULL;
	return 0;
}

//int main(void){
//    Sqstack stack;
//    init_stack(&stack);
//    push(&stack,10);
//    push(&stack,11);
//    push(&stack,12);
//    push(&stack,13);
//    push(&stack,14);
//    while(!is_empty(&stack)){
//        int  e;
//        pop(&stack,&e);
////      cout<<"length:"<<stackLength(stack)<<" "<<e<<" "<<endl;
//        printf("length = %d, elem = %d\n", stack_length(&stack), e);
//    }
//    destory_stack(&stack);
//    return 0;
//}
