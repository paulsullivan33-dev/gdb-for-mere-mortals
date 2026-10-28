#include <stdio.h>

void countdown(int n)
{
    if (n == 0) {
        printf("liftoff\n");
        return;
    }
    printf("%d...\n", n);
    countdown(n - 1);
}

int main(void)
{
    countdown(3);
    return 0;
}
