#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int *handler_nums;

void exit_handler() {
    static int index = 0;
    sleep(1);
    printf("Handler %d \n", handler_nums[index++]);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <num>\n", argv[0]);
        return 1;
    }

    int handlers_count = atoi(argv[1]);

    if (handlers_count <= 0) {
        printf("must be a positive number\n");
        return 1;
    }

    handler_nums = malloc(handlers_count * sizeof(int));
    if (handler_nums == NULL) {
        fprintf(stderr, "Memory alloc error\n");
        return 1;
    }

    for (int i = 0; i < handlers_count; i++) {
        handler_nums[i] = i + 1;
        if (atexit(exit_handler)) {
            fprintf(stderr, "handler error %d\n", i + 1);
            return 1;
        }
    }

    printf("Success\n");

    return 0;
}
