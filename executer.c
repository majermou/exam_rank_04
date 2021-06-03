#include "microshell.h"

int get_NbrSpaces(char *str)
{
    int     i = 0, nbr = 0;

    while (str[i])
    {
        if (str[i] == ' ' && str[i + 1])
            nbr++;
        i++;
    }
    return (nbr);
}

char    *ft_strdup0(char **str)
{
    int     i = 0, lenght = 0;
    char     *ret = NULL;

    while (str[0][i] && str[0][i] != ' ')
    {
        lenght++;
        i++;
    }
    ret = malloc((lenght + 1) * sizeof(char));
    i = 0;
    while (str[0][i] && str[0][i] != ' ')
    {
        ret[i] = str[0][i];
        i++;
    }
    ret[i++] = '\0';
    str[0] += i;
    return (ret);
}

char    **ft_split(char *str)
{
    int     i = 0, nbr_space = 0;
    char    **ret = NULL;

    if (str[0] == ' ')
        str++;
    nbr_space = get_NbrSpaces(str) + 1;
    ret = malloc((nbr_space + 1) * sizeof(char *));
    while (i < nbr_space)
        ret[i++] = ft_strdup0(&str);
    ret[i] = NULL;
    return (ret);
}

int executeCmd(char **args, int nbr_pipes, char **envp)
{
    char    **param;
    pid_t   pid, *wpid;
    int     status, i, j, *fds;
    int     ret = EXIT_SUCCESS;

    if (!nbr_pipes)
    {
        param = ft_split(args[0]);
        if (!strcmp(param[0], "cd"))
        {
            if (param[2])
            {
                write(2, "error: cd: bad arguments\n", 25);
                exit(EXIT_FAILURE);
            }
            else if (chdir(param[1]) < 0)
            {
                write(2, "error: cd: cannot change directory to ", 38);
                write(2, param[1], ft_strlen(param[1]));
                write(2, "\n", 1);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            pid = fork();
            if (pid < 0)
            {
                write(2, "error: fatal\n", 13);
                exit(EXIT_FAILURE);
            }
            else if (!pid && (ret = execve(param[0], param, envp)) < 0)
            {
                write(2, "error: cannot execute ", 22);
                write(2, param[0], ft_strlen(param[0]));
                write(2, "\n", 1);
                exit(ret);
            }
            else if (pid > 0)
                waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                ret = WEXITSTATUS(status);
        }
        i = 0;
        while (param[i])
           free(param[i++]);
        free(param);
    }
    else
    {
        fds = malloc(nbr_pipes * 2 * sizeof(int));
        wpid = malloc((nbr_pipes + 1) * sizeof(pid_t));
        i = 0;
        while (args[i])
        {
            pipe(fds + 2 * i);
            param = ft_split(args[i]);
            pid = fork();
            if (pid < 0)
            {
                write(2, "error: fatal\n", 13);
                exit(EXIT_FAILURE);
            }
            else if (!pid)
            {
                if (i > 0)
                    dup2(fds[i * 2 - 2], 0);
                if (args[i + 1])
                    dup2(fds[i * 2 + 1], 1);
                if ((ret = execve(param[0], param, envp)) < 0)
                {
                    write(2, "error: cannot execute ", 22);
                    write(2, param[0], ft_strlen(param[0]));
                    write(2, "\n", 1);
                    exit(ret);
                }
            }
            else if (pid > 0)
            {
                if (i > 0)
                    close(fds[i * 2 - 2]);
                if (args[i + 1])
                    close(fds[i * 2 + 1]);
                wpid[i] = pid;
            }
            j = 0;
            while (param[j])
                free(param[j++]);
            free(param);
            i++;
        }
        i = 0;
        while (i < nbr_pipes + 1)
            waitpid(wpid[i++], &status, 0);
        if (WIFEXITED(status))
            ret = WEXITSTATUS(status);
        free(fds);
        free(wpid);
    }
    return (ret);
}