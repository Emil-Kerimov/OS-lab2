//
// Created by Эмиль on 11.10.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define DEFAULT_BUFFER_SIZE 127

void display_prompt() {
    struct passwd *pw = getpwuid(getuid());
    char *user = pw->pw_name;
    printf("[%s]$ ", user);
}

int main(int argc, char *argv[]) {
    int buffer_size = DEFAULT_BUFFER_SIZE;

    // Обробка опцій
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Usage: %s [-b buffer_size]\n", argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--buffer") == 0) {
            if (i + 1 < argc) {
                buffer_size = atoi(argv[++i]);
            } else {
                fprintf(stderr, "No buffer size provided\n");
                exit(1);
            }
        }
    }

    char *command = (char *)malloc(buffer_size * sizeof(char));

    while (1) {
        display_prompt();
        fgets(command, buffer_size, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "stop") == 0) {
            printf("Wait 3 seconds...\n");
            sleep(3);
            system("clear");
            break;
        }

        int status = system(command);
        if (status == -1) {
            perror("system");
        } else {
            printf("Exit code: %d\n", WEXITSTATUS(status));
        }
    }

    free(command);
    return 0;
}
