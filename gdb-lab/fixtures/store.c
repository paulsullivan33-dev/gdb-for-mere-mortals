#include <stdio.h>

#define SLOTS 3

static int values[SLOTS];
static int claimed[SLOTS];

int *find_free_slot(void)
{
    int i;
    for (i = 0; i < SLOTS; i++) {
        if (!claimed[i]) {
            claimed[i] = 1;
            return &values[i];
        }
    }
    return NULL;            /* no slot left */
}

void store_value(int v)
{
    int *slot = find_free_slot();
    *slot = v;              /* crashes when slot is NULL */
}

int main(void)
{
    store_value(10);
    store_value(20);
    store_value(30);
    store_value(40);        /* one too many */
    printf("stored everything\n");
    return 0;
}
