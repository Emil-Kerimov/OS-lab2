//
// Created by Эмиль on 14.10.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

#define DEFAULT_FILENAME "output"
#define DEFAULT_NUM_PROCESSES 1

void child_process(int child_num, int num_randoms) {
    char *file_name;
    file_name = getenv("FILE_NAME");
    char filename_buffer[256];

    if (file_name == NULL) {
        snprintf(filename_buffer, sizeof(filename_buffer), "%s_%d.txt", DEFAULT_FILENAME, child_num);
    } else {
        snprintf(filename_buffer, sizeof(filename_buffer), "%s_%d.txt", file_name, child_num);
    }

    FILE *fp = fopen(filename_buffer, "w");
    if (fp == NULL) {
        perror("Cant open the file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL) + child_num);
    for (int i = 0; i < num_randoms; i++) {
        float random_num = (float)rand() / RAND_MAX;
        fprintf(fp, "%f\n", random_num);
    }

    fclose(fp);
    printf("child procces %d: created file %s\n", child_num, filename_buffer);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    int opt;
    char *file_name = NULL;
    int num_processes = DEFAULT_NUM_PROCESSES;
    int num_randoms = 10;

    struct option long_options[] = {
        {"file", required_argument, 0, 'f'},
        {"number", required_argument, 0, 'n'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "f:n:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'f':
                file_name = optarg;
                setenv("FILE_NAME", file_name, 1);
                break;
            case 'n':
                num_processes = atoi(optarg);
                break;
            default:
                fprintf(stderr, "usages: %s [-f file_name] [-n num_processes]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    printf("Filename: %s\n", file_name ? file_name : DEFAULT_FILENAME);
    printf("number of child processes: %d\n", num_processes);

    for (int i = 1; i <= num_processes; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            child_process(i, num_randoms);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    printf("All child process are terminated\n");
    return 0;
}
