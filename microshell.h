#ifndef MICROSHELL_H
#define MICROSHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int     ft_strlen(char *str);
int     executeCmd(char **args, int nbr_pipes, char **envp);

#endif