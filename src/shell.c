#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

char **get_input(char *);
int cd(char *);

int
main() {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;
    
    while (1) {
        input = readline("beersh$ ");
        command = get_input(input);
        
        if (!command[0]) {
            free(input);
            free(command);
            continue;
        }
        
        if (strcmp(command[0], "cd") == 0) {
            if (cd(command[1]) < 0) {
                perror(command[1]);
            }
            
            /* Skip the fork */
            continue;
        }
        
        child_pid = fork();
        if (child_pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        
        if (child_pid == 0) {
            if (execvp(command[0], command) < 0) {
                perror(command[0]);
                exit(1);	
            }
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }
        
        free(input);
        free(command);
    }
    
    return 0;
}

char 
**get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    int index = 0;
    char *p = input;
    
    while (*p != '\0') {
        /* skip leading spaces */
        while (*p == ' ') p++;
        if (*p == '\0') break;

        if (*p == '"') { /* detect and collect quote */
            command[index++] = p;
            p++;
            while (*p != '"' && *p != '\0') p++;
            if (*p == '"') *p++;
            if (*p == ' ') *p++ = '\0';
        } else { /* collect normal token */
            command[index++] = p;
            while (*p != ' ' && *p != '\0') p++;
            if (*p == ' ') *p++ = '\0';
        }
    }

    command[index] = NULL;
    return command;
}

int
cd(char *path) {
    return chdir(path);
}