/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamalai <ahamalai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 11:06:04 by mburakow          #+#    #+#             */
/*   Updated: 2024/05/07 14:29:34 by ahamalai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	init_shell_struct(t_shell *sh)
{
	sh->ms_envp = NULL;
	sh->input = NULL;
	sh->root = NULL;
	sh->cmdarr = NULL;
	sh->cmd = NULL;
	sh->hdocs = NULL;
	sh->redirs = NULL;
	// Should this be -1 to check for actual update?
	sh->status = 0;
}

void	free_args(char **args)
{
	int	i;

	if (!args || !*args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

void	free_cmdn(t_cmdn *node)
{
	if (node == NULL)
		return ;
	free_cmdn(node->left);
	free_args(node->cargs);
	node->cargs = NULL;
	free(node);
	free_cmdn(node->right);
	node = NULL;
}

void	free_new_prompt(t_shell *sh)
{
	free(sh->input);
	free_cmdn(sh->root);
	free_args(sh->cmdarr);
	close(sh->pfd[0]);
	close(sh->pfd[1]);
	close(sh->efd[0]);
	close(sh->efd[1]);
	free_args(sh->cmd);
	free(sh->hdocs);
	sh->input = NULL;
	sh->root = NULL;
	sh->cmdarr = NULL;
	sh->cmd = NULL;
	sh->hdocs = NULL;
}