/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburakow <mburakow@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:23:00 by mburakow          #+#    #+#             */
/*   Updated: 2024/04/10 13:31:06 by mburakow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

static void	exec_cmd(t_cmdn *node, int pfd[2])
{
	char	*path;
	char	**path_array;
	char	*cmdp;

	if (dup2(pfd[0], STDIN_FILENO) == -1)
		exit (1);
	path = getenv("PATH");
	path_array = ft_split(path, ":");
	cmdp = get_exec_path(path_array, node->cargs[0]);
	if (cmdp == NULL)
		exit (127);
	printf("#%s#\n", cmdp);
	if (!node->cargs[0] || !*node->cargs || !cmdp
		|| execve(cmdp, node->cargs, node->envp) == -1)
	{
		printf("Execve error.\n");
		// free_args(&path);
		exit (127);
	}
	close(pfd[0]);
	close(pfd[1]);
}

static int	exec_node(t_cmdn *node, int *pfd, t_dynint *commands)
{
	int	pid;
	// int status;

	if (node == NULL)
		return (0);
	exec_node(node->left, pfd, commands);
	if (node->ntype == COMMAND)
	{
		pid = fork();
		if (pid == -1)
		{
			wait_for(commands);
			exit (1);
		}
		else if (pid == 0)
			exec_cmd(node, pfd);
		else
			add_to_dynamic_int_array(commands, pid);
	}	
	exec_node(node->right, pfd, commands);
	return (0);
}

int	run_cmds(t_cmdn *root)
{
	int			pfd[2];
	t_dynint	*commands;
	
	if (root == NULL)
		return (0);
	if (pipe(pfd) == -1)
		exit (1);
	commands = create_dynamic_int_array();
	exec_node(root, pfd, commands);
	wait_for(commands);
	close(pfd[0]);
	close(pfd[1]);
	return (0);
}

/*
void	print_cmdn(t_cmdn *node)
{
	int	i;

	if (node == NULL)
		return ;
	print_cmdn(node->left);
	i = 0;
	while (node->cargs && node->cargs[i] != '\0')
	{
		if (i != 0)
			printf("\t");
		printf("%s\n", node->cargs[i]);
		i++;
	}
	print_cmdn(node->right);
}

static	add_to_commands(int pid, int **commands)
{
	static int	i = 0;

	if (commands == NULL)
	{
		commands = ft_calloc(5, sizeof(int *));
		commands[0] = pid;
	}
	else  
	{

	}

}

static int	exec_node(t_cmdn *node, int *pfd, int **commands)
{
	int	pid;
	int status;

	if (node == NULL)
		return (0);
	exec_node(node->left, pfd, commands);
	if (node->ntype == COMMAND)
	{
		pid = fork();
		if (pid == -1)
		{
			wait_for(commands);
			exit (1);
		}
		else if (pid == 0)
			exec_cmd(node, pfd);
		else
			add_to_commands(pid, commands);
	}	
	exec_node(node->right, pfd, commands);
	return (0);
}

*/