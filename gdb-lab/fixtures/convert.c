#include <stdio.h>
#include <stdlib.h>

/* Print a Celsius temperature in Fahrenheit.
   Usage: convert 100  ->  should print 212 */
int to_fahrenheit(int celsius)
{
    int fahr = (9 / 5) * celsius + 32;  /* bug: 9 / 5 is 1 in integer maths */
    return fahr;
}

int main(int argc, char **argv)
{
    int celsius;
    if (argc < 2) {
        printf("usage: convert CELSIUS\n");
        return 1;
    }
    celsius = atoi(argv[1]);
    printf("%d C = %d F\n", celsius, to_fahrenheit(celsius));
    return 0;
}
