#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>


char current_dir[256];


int buildin_chdir(char **args)
{
    if (args[1] == NULL || chdir(args[1]) < 0) {
        return 0;
    }

    return 1;
}


int shell_execute(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("shell");
        }
        exit(1);
    } else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return status;
}


char **shell_split_line(char *line)
{
    char **list = malloc(sizeof(char *));
    int position = 0;

    list[0] = NULL;
    char *token = strtok(line, " ");
    while (token != NULL) {
        char *tok = malloc(strlen(token) + 1);
        strcpy(tok, token);
        list[position++] = tok;
        list = realloc(list, sizeof(char *) * position + 1);
        list[position] = NULL;
        token = strtok(NULL, " ");
    }

    return list;
}


void free_args(char **args)
{
    for (char **p = args; *p != NULL; p++) {
        free(*p);
    }

    free(args);
}


char *shell_read_line(void)
{
    char *line = malloc(256);
    int position = 0;

    while (1) {

        int c = getchar();
        if (c == '\n') {
            line[position] = '\0';
            break;
        }

        line[position++] = c;
    }

    return line;
}


void print_prompt(void)
{
    printf("%s > ", current_dir);
}


int equal_string(const char *s1, const char *s2)
{
    return strcmp(s1, s2) == 0; 
}


void shell_loop(void)
{
    int status = 1;

    while (status) {
        
        print_prompt();

        char *line = shell_read_line();
        char **args = shell_split_line(line);

        if (equal_string(args[0], "exit")) {
            status = 0;
            continue;
        }

        if (equal_string(args[0], "cd")) {
            buildin_chdir(args);
            continue;
        }

        shell_execute(args);

        free_args(args);
        free(line);
    }
}


void initialize()
{
    getcwd(current_dir, sizeof(current_dir));
}


int main(int argc, char *argv[])
{
    initialize();
    shell_loop();

    return 0;
}
