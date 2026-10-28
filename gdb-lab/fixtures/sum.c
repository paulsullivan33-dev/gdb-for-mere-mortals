#include <stdio.h>

int add(int a, int b)
{
    int result = a + b;
    return result;
}

int main(void)
{
    int total = add(2, 3);
    printf("total = %d\n", total);
    return 0;
}
