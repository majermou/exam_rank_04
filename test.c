#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    // int     i = 250;

    // printf("./microshell ");
    // while (i--)
    //     printf("/bin/ls -la |");
    // printf("/bin/ls -la");

    char *s = malloc(sizeof(char) * 8);
    s = "hi";
    printf("%s\n", s);
    while (1)
        ;
}