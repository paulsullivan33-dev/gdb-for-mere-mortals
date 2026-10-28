#include <stdio.h>
#include <unistd.h>

int main(void)
{
    int tick = 0;
    while (tick < 60) {
        printf("tick %d\n", tick);
        fflush(stdout);
        tick++;
        sleep(1);
    }
    return 0;
}
