/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburakow <mburakow@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 11:20:14 by mburakow          #+#    #+#             */
/*   Updated: 2024/05/20 13:38:36 by mburakow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	trim_quote_alloc_hdoc_rdir(t_shell *sh)
{
	int		i;

	i = 0;
	i = 0;
	while (sh->cmd[i] != NULL)
	{
		sh->cmd[i] = trim_string(sh->cmd[i]);
		i++;
	}
	sh->cmd = ft_remove_quotes(sh->cmd);
	sh->hdocs = ft_calloc((i + 1), sizeof(int));
	sh->redirs = ft_calloc((i + 1), sizeof(int));
	if (sh->hdocs == NULL || sh->redirs  == NULL)
		errexit("hdocs or redirs malloc error", NULL, NULL, sh);
	sh->hdocs[i] = -1;
	sh->redirs[i] = -1;
}

static void	get_heredocs(t_shell *sh)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (sh->cmd[i] != NULL)
	{
		j = 0;
		while (sh->cmd[i][j] != '\0')
		{
			if (sh->cmd[i][j] == '<' && sh->cmd[i][j + 1] == '<'
				&& sh->cmd[i][j + 2] != '<' && sh->cmd[i][j + 2] != '\0')
			{
				sh->hdocs[i]++;
				temp = ft_heredoc(sh->cmd[i], sh->hdocs[i]);
			}
			j++;
		}
		if (sh->hdocs[i] > 0)
			sh->cmd[i] = temp;
		i++;
	}
}

// Right now this creates both pipes if there are more than one command.
// How pipes should work is create only pfd for multiple, efd for single builtin.
void	create_pipes(t_shell *sh)
{
	int 	cmdcount;
	int		i;

	cmdcount = 1;
	sh->hdoc = FALSE;
	i = 0;
	while (sh->cmdarr[cmdcount] != NULL)
		cmdcount++;
	while (sh->hdocs[i] != -1)
	{
		if (sh->hdocs[i] > 0)
			sh->hdoc = TRUE;
		i++;
	}
	if (cmdcount > 1 || sh->hdoc)
	{
		dprintf(2, "Hdoc found.\n");
		if (pipe(sh->pfd) == -1 || pipe(sh->efd) == -1) //  || pipe(sh.sfd) == -1)
			errexit("error :", "pipe initialization", NULL, sh);
	}
	dprintf(2, "Pipes created.\n");
	sh->cmdcount = cmdcount;
}

static t_cmdn	*create_node(t_cmdn *current, t_shell *sh, int index)
{
	int	len;

	len = 0;
	while (sh->cmdarr[len] != NULL)
		len++;
	sh->cmdarr[index] = trim_rdirspace(sh->cmdarr[index]);
	sh->cmd = ft_split_time_space(sh->cmdarr[index], ' ');
	if (!(sh->cmd))
		errexit("error: ", "root malloc", NULL, sh);
	trim_quote_alloc_hdoc_rdir(sh);
	get_heredocs(sh);
	create_pipes(sh);
	get_redirects(sh);
	dprintf(2, "Redirs gotten.\n");
	if (index < len - 2)
	{
		current->left = init_cmd_node(COMMAND, sh, FALSE);
		current->right = init_cmd_node(PIPELINE, sh, FALSE);
		current = current->right;
	}
	else if (index == len - 2)
		current->left = init_cmd_node(COMMAND, sh, FALSE);
	else
		current->right = init_cmd_node(COMMAND, sh, TRUE);
	return (current);
}

// Creates root node, splits the input along pipes and
// creates the nodes in a tree structure.
void	parse_input(t_shell *sh)
{
	t_cmdn	*current;
	int		i;

	sh->root = init_cmd_node(PIPELINE, sh, FALSE);
	if (!(sh->root))
		errexit("error: ", "root malloc", NULL, sh);
	sh->cmdarr = ft_split(sh->input, "|");
	// dprintf(2, "Exited with sh->cmdarr[0]: %s", sh->cmdarr[0]);
	if (!(sh->cmdarr))
		errexit("error: ", "cmdarr malloc", NULL, sh);
	free(sh->input);
	sh->input = NULL;
	current = sh->root;
	i = 0;
	while (sh->cmdarr[i] != NULL)
	{
		current = create_node(current, sh, i);
		i++;
	}
}
