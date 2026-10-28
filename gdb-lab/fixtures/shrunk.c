#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int seed = atoi(argv[1]);
    int answer = seed * 2 + 1;
    printf("answer = %d\n", answer);
    printf("done\n");
    return 0;
}
