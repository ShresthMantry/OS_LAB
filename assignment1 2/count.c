#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int ch;
    int nonAlphabeticCount = 0;

    while ((ch = getchar()) != EOF) {
        if (!isalpha(ch)) {
            nonAlphabeticCount++;
            
        }
        putchar(ch);
    }

    fprintf(stderr, "Non-alphabetic character count: %d\n", nonAlphabeticCount);

    exit(0);
}

