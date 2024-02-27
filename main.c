#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10
#define MAX_COMMAND_LENGTH 100

char *allocate_memory_for_file(size_t size)
{
	char *file_data = malloc(size);
	if (file_data == NULL)
	{
		perror("Error: Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	return (file_data);
}
void deallocate_memory_for_file(char *file_data)
{
	free(file_data);
}
void free_memory(char *lineptr)
{
	free(lineptr);
}
void handle_exit()
{
        printf("Exiting shell...\n");
        exit(EXIT_SUCCESS);
}

/**
 * main - Entry point for the shell program
 *
 * Return: Always 0.
 */
int main(void)
{
    char command[MAX_COMMAND_LENGTH];
    pid_t pid;
    char *args[MAX_ARGS + 1];
    int status;

    while (1)
    {
        printf("(Tshell) $ ");
        fgets(command, sizeof(command), stdin);

        /* Remove trailing newline character */
        command[strcspn(command, "\n")] = '\0';

        /* Check if command is "exit" */
        if (strncmp(command, "exit ", 5) == 0)
        {
            printf("Calling handle_exit_arg function...\n");
            handle_exit_arg(command);
            continue;
        }
        else if (strcmp(command, "exit") == 0)
        {
            exit(EXIT_SUCCESS);
        }

        /* Tokenize the command using the custom tokenizer */
        tokenize(command, args);

        /* Fork a child process */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            /* Child process */
            /* Execute command */
            execvp(args[0], args);
            /* If execvp returns, it must have failed */
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            /* Parent process */
            /* Wait for child to complete */
            wait(&status);
            if (WIFEXITED(status))
            {
                printf("Exit status: %d\n", WEXITSTATUS(status));
            }
        }
    }

    return EXIT_SUCCESS;
}
