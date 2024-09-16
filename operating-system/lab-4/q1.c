#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    FILE *fp;
    char path[1035];

    fp = popen("./input", "r");
    if (fp == NULL) {
        perror("popen");
        return EXIT_FAILURE;
    }

    while (fgets(path, sizeof(path) - 1, fp) != NULL) {
        printf("Output from 'input' program: %s", path);
    }

    if (pclose(fp) == -1) {
        perror("pclose");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

