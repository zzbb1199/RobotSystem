//
// Created by Raven on 2018/7/26.
//

#ifndef __STACK_H__
#define __STACK_H__

#define Status int
#define ElemType  int
/**
 * 栈结构--顺序栈实现
 */
//----------------顺序栈定义------------
#define INIT_SIZE 100       //初始化空间大小
#define INCREMENT 10       //内存空间增量
typedef struct {
	ElemType *base;         //栈底指针
	ElemType *top;          //栈首指针+1 （指向已分配内存但未赋值的内存空间）
	int stack_size;         //当前栈最大存储空间
}Sqstack;

//----------------顺序栈实现------------
/**
 * 对栈s进行相应的初始化
 * @param s
 * @return OK
 *         ERROR
 */
Status init_stack(Sqstack *s);

/**
 * 判定栈s是否为空
 * @param s
 * @return
 */
Status is_empty(Sqstack *s);

/**
 * 取栈s的栈顶元素，以e作为返回值
 * @param s
 * @param e
 * @return
 */
Status get_top(Sqstack *s, ElemType *e);

/**
 * 将e压如栈s
 * @param s
 * @param e
 * @return OK
 *          ERROR
 */
Status push(Sqstack *s, ElemType *e);

/**
 * 栈s栈顶元素弹出，以e作为返回值
 * @param s
 * @param e
 * @return
 */
Status pop(Sqstack *s, ElemType *e);
/**
 * 将栈s置为空栈
 * @param s
 * @return
 */
Status clear_stack(Sqstack *s);
/**
 * 销毁栈s
 * @param s
 * @return
 */
Status destory_stack(Sqstack *s);

#endif

