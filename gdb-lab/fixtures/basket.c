#include <stdio.h>

struct item {
    const char *name;
    int price;              /* cents */
};

static struct item items[] = {
    { "notebook",  349 },
    { "pencil",     60 },
    { "mug",       899 },
    { "ruler",     125 },
    { "lamp",     2400 },
};

int main(void)
{
    int count = 5;
    int total = 0;
    int i;

    for (i = 0; i < count - 1; i++) {   /* bug: stops one early */
        total += items[i].price;
    }

    printf("items: %d, total: %d cents\n", count, total);
    return 0;
}
