#include <stdio.h>
#include "add.h"


static int add_test(int a,int b)
{
    return a+b;
}

int main(void)
{
    printf("%d\n",add_test(1,2));
    return 0;
}
