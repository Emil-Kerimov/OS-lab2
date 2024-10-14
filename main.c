#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command>\n", argv[0]);
        return 1;
    }

    char command[256];
    snprintf(command, sizeof(command), "%s", argv[1]);
    int status = system(command);

    if (status == -1) {
        perror("system");
    } else {
        printf("Exit code: %d\n", WEXITSTATUS(status));
    }

    return 0;
}
