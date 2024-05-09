#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <signal.h>
#include <string.h>
void sig_handler(int signo)
{
}

void redirect_output_creat_file(char *filename) // for >
{
    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file == -1)
    {
        printf("Error in open()\n");
        exit(1);
    }
    if (dup2(file, STDOUT_FILENO) == -1)
    {
        printf("Error in dup2()\n");
        exit(1);
    }
    close(file);
}

void redirect_output_append_file(char *filename) // for >>
{
    int file = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (file == -1)
    {
        printf("Error in open()\n");
        exit(1);
    }
    if (dup2(file, STDOUT_FILENO) == -1)
    {
        printf("Error in dup2()\n");
        exit(1);
    }
    close(file);
}

void execute_piped_commands(char **argv, int i) // for |
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        printf("Error in pipe()\n");
        exit(1);
    }
    int pidPipe = fork();
    if (pidPipe == -1)
    {
        printf("Error in forkPipe\n");
        exit(1);
    }
    if (pidPipe == 0)
    {
        // child process
        signal(SIGINT, sig_handler);
        close(fd[0]);

        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            printf("Error in dup2 (read)\n");
            exit(1);
        }
        close(fd[1]);
        char *command[10];
        int j;
        for (j = 0; strcmp(argv[j], "|"); j++)
        {
            command[j] = argv[j];
        }
        command[j] = NULL;

        if (execvp(command[0], command) == -1)
        {
            printf("Error in execvp()\n");
            exit(1);
        }
    }
    else
    {
        // parent process
        int index = 0, k = i + 1;
        while (argv[k])
        {
            argv[index++] = argv[k++];
        }
        argv[index] = NULL;
        close(fd[1]);
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            printf("Error in dup2_parent\n");
            exit(1);
        }
        close(fd[0]);
        wait(NULL);
    }
}

int main()
{
    char input[1024];
    char *token;
    char *argv[10];

    signal(SIGINT, sig_handler);

    while (1)
    {
        printf("\033[1;32mstshell\033[0m$ ");

        fgets(input, 1024, stdin);
        input[strlen(input) - 1] = '\0'; // replace \n with \0

        if (strcmp(input, "exit") == 0)
        {
            return 0;
        }

        char *token = strtok(input, " ");
        int argc = 0;
        while (token != NULL)
        {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
        argv[argc] = NULL;

        int pid = fork();
        if (pid == -1)
        {
            printf("Error in forkPipe\n");
            exit(1);
        }
        if (pid == 0)
        {
            signal(SIGINT, sig_handler);
            for (int i = 0; argv[i] != NULL; i++)
            {
                if (strcmp(argv[i], ">") == 0)
                {
                    redirect_output_creat_file(argv[i + 1]);
                    argv[i] = NULL;
                    break;
                }

                if (strcmp(argv[i], ">>") == 0)
                {
                    redirect_output_append_file(argv[i + 1]);
                    argv[i] = NULL;
                    break;
                }

                if (strcmp(argv[i], "|") == 0)
                {
                    execute_piped_commands(argv, i);
                    i = -1;
                }
            }

            if (execvp(argv[0], argv) == -1)
            {
                printf("Error in execvp()\n");
                exit(1);
            }
        }
        wait(NULL);
    }
    return 0;
}
