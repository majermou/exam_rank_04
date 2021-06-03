#include "microshell.h"

int     ft_strlen(char *str)
{
    int     i = 0;

    while (str && str[i])
        i++;
    return (i);
}

void    ft_strjoin(char **cmd, char *args)
{
    int     lenght = ft_strlen(cmd[0]) + ft_strlen(args) + 2;
    char    *ret = malloc(lenght * sizeof(char));
    int     i = 0, j = 0;

    if (cmd[0])
    {
        while (cmd[0][j])
            ret[i++] = cmd[0][j++];
        ret[i++] = ' ';
        free(cmd[0]);
    }
    j = 0;
    while (args[j])
        ret[i++] = args[j++];
    ret[i] = '\0';
    cmd[0] = ret;
}

int check_errors(char *cmd)
{
    int     i = 0;

    while (cmd[i])
    {
        if ((cmd[i] == ';' && cmd[i + 1] == ';') ||
            (cmd[i] == ';' && cmd[i + 1] == ' ' && cmd[i + 2] == ';'))
            return (1);
        i++;
    }
    return (0);
}

int get_NbrCmds(char *cmd)
{
    int     i = 0;
    int     nbr = 0;

    while (cmd[i])
    {
        if (cmd[i] == ';')
            nbr++;
        i++;
    }
    return (nbr);
}

int get_NbrPipes(char *cmd)
{
    int     i = 0;
    int     nbr = 0;

    while (cmd[i] && cmd[i] != ';')
    {
        if (cmd[i] == '|')
            nbr++;
        i++;
    }
    return (nbr);
}

char    *ft_strdup(char **str)
{
    int     lenght = 0, i = 0;
    char    *ret = NULL;

    while (str[0][i] && str[0][i] != ';' && str[0][i] != '|')
    {
        lenght++;
        i++;
    }
    ret = malloc((lenght + 1) * sizeof(char));
    i = 0;
    while (str[0][i] && str[0][i] != ';' && str[0][i] != '|')
    {
        ret[i] = str[0][i];
        i++;
    }
    ret[i++] = '\0';
    str[0] += i;
    return (ret);
}

int main(int argc, char **argv, char **envp)
{
    char    *cmd = NULL, *tmp = NULL, **args;
    int     i = 1, nbr_pipes = 0, ret, nbr_cmds = 0;

    if (argc > 1)
    {
        while (argv[i])
            ft_strjoin(&cmd, argv[i++]);
        if (check_errors(cmd))
        {
            write(1, "error: fatal\n", 13);
            return (1);
        }
        tmp = cmd;
        nbr_cmds = get_NbrCmds(cmd) + 1;
        while (nbr_cmds-- > 0)
        {
            nbr_pipes = get_NbrPipes(cmd) + 1;
            args = malloc((nbr_pipes + 2) * sizeof(char *));
            i = 0;
            while (i < nbr_pipes)
                args[i++] = ft_strdup(&cmd);
            args[i] = NULL;
            if (args[0][0])
            {
                ret = executeCmd(args, --nbr_pipes, envp);
            }
            i = 0;
            while (args[i])
                free(args[i++]);
            free(args);
        }
        free(tmp);
    }
    return (ret);
}