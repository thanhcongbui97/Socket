#include <stdio.h>
#include <string.h>

#define MAX 1024

int main()
{
    char bits[MAX];
    puts("Enter bit sequence: ");
    gets(bits);
    int value = (int) bits[2] ;
    printf("The third bit in sequence is : %c", bits[2]);
    printf("\nValue is : %d\n", value);
    return 0;
}