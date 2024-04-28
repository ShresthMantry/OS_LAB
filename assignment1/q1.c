#include <stdio.h>

int main(int argc, char *argv[]) {
    int count = 0;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            count=count+1;
            printf("%s ", argv[i]);
        }
    }

    if (count >= 1) {
        printf("\n");
    }

    return 0;
} 





