/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburakow <mburakow@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 16:07:48 by mburakow          #+#    #+#             */
/*   Updated: 2024/05/15 11:13:38 by mburakow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

 void	*db_malloc(size_t size)
 {
	return (malloc(size));
 }

char	*get_exec_path(char **path, char *cmd)
{
	char	*slashpath;
	char	*execpath;

	execpath = NULL;
	slashpath = NULL;
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (*path)
	{
		if ((*path)[0] == '/')
        {
            execpath = ft_strdup(cmd); // cmd is the absolute path
            if (access(execpath, X_OK) == 0)
                return (execpath);
        }
		else
		{
		slashpath = ft_strjoin(*path, "/");
		execpath = ft_strjoin(slashpath, cmd);
		if (access(execpath, X_OK) != -1)
			return (execpath);
		}
		if (execpath)
			free(execpath);
		execpath = NULL;
		if (slashpath)
			free(slashpath);
		slashpath = NULL;
		path++;
	}
	return (NULL);
}

int	wait_for(t_intvec *commands)
{
	int	status;
	int	nc;

	nc = 0;

	while (commands->array[nc + 1])
	{
		// ft_putstr_fd("Waited for :", 2);
		// ft_putnbr_fd(waitpid(commands->array[nc], &status, 0), 2);
		// ft_putchar_fd('\n', 2);
		//dprintf(2, "PID: %d Status: %d\n", commands->array[nc], WEXITSTATUS(status));
		dprintf(2, "Waiting for PID: %d\n", commands->array[nc]);
		waitpid(commands->array[nc], &status, 0);
		nc++;
	}
	dprintf(2, "Waiting for PID: %d\n", commands->array[nc]);
	waitpid(commands->array[nc], &status, 0);
	//dprintf(2, "PID: %d Status: %d\n", commands->array[nc], WEXITSTATUS(status));
	return (WEXITSTATUS(status));
}

void	print_cmdn(t_cmdn *node)
{
	int	i;

	if (node == NULL)
		return ;
	print_cmdn(node->left);
	i = 0;
	if (node->ntype == COMMAND)
		ft_putendl_fd("COMMAND:", 2);
	if (node->ntype == PIPELINE)
		ft_putendl_fd("PIPELINE:", 2);
	while (node->cargs && node->cargs[i] != 0)
	{
		if (i != 0)
			ft_putchar_fd('\t', 2);
		ft_putendl_fd(node->cargs[i], 2);
		i++;
	}
	ft_putnbr_fd(node->last, 2);
	ft_putchar_fd('\n', 2);
	print_cmdn(node->right);
}

char	**ft_remove_quotes(char **cmd)
{
	int	i;
	int	j;

	i = -1;
	while (cmd[++i] != NULL)
	{
		j = 0;
		while (cmd[i][j] != '\0')
		{
			if (cmd[i][j] == '\"')
			{
				while (cmd[i][j] != '\0')
				{
					cmd[i][j] = cmd[i][j + 1];
					j++;
				}
				j = 0;
			}
			else
				j++;
		}
	}
	return (cmd);
}

// At the moment accounts only for space characters,
//	are other characters necessary?
char	*trim_string(char *str)
{
	int		i;
	char	*end;
	char 	*curs;

	i = 0;
	while ((unsigned char)str[i] == 32)
		i++;
	curs = &str[i];
	i = 0;
	if (*curs == '\0')
	{
		str[i] = *curs;
		return (str);
	}
	while (*curs != '\0')
	{
		str[i++] = *curs;
		curs++;
	}
	end = str + ft_strlen(str) - 1;
	while (end > str && (unsigned char)*end == 32)
		end--;
	end[1] = '\0';
	return (str);
}