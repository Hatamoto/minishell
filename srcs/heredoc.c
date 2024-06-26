/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamalai <ahamalai@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:09:30 by ahamalai          #+#    #+#             */
/*   Updated: 2024/05/29 12:46:04 by ahamalai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_breakchar(char *breakchar, int *j, int hdocs)
{
	int		k;
	char	*temp;
	int		i;

	i = 0;
	while (breakchar[i] != '\0')
	{
		if (breakchar[i] == '<' && breakchar[i + 1] == '<'
			&& breakchar[i + 2] != '<')
		{
			*j += 1;
			if (*j == hdocs)
				break ;
		}
		i += 1;
	}
	k = i + 2;
	while (breakchar[k] != '\0' && breakchar[k] != '<')
		k++;
	if (breakchar[i] != ' ')
		breakchar = ft_substr(breakchar, i + 2, k - i - 2);
	temp = ft_strjoin(breakchar, "\n");
	free(breakchar);
	return (temp);
}

int	heredoc_loop(char **astr, char *breakchar, int j, int hdocs)
{
	char			*buf;

	buf = NULL;
	signal(SIGINT, ft_handler_heredoc);
	while (1)
	{
		write(1, "> ", 2);
		buf = get_next_line(0);
		if (buf == NULL)
			return (0);
		if (!ft_strncmp(breakchar, buf, ft_strlen(breakchar)))
			break ;
		if (j == hdocs)
			*astr = ft_strjoin(*astr, buf);
		free(buf);
	}
	free(buf);
	return (1);
}

void	heredoc_child(int pipefd[2], char *breakchar, t_shell *sh, int hdocs)
{
	char	*astr;
	char	*temp;
	int		bkquote;
	int		j;

	j = 0;
	astr = ft_strdup("");
	close(pipefd[0]);
	temp = make_breakchar(breakchar, &j, hdocs);
	bkquote = test_quote_level(temp);
	if (bkquote != -1)
		remove_breakchar_quotes(temp);
	if (heredoc_loop(&astr, temp, j, hdocs) == 0)
	{
		free(temp);
		exit (0);
	}
	free_breakchar_temp(breakchar, temp);
	if (astr[0] == '\0')
		ft_strjoin(astr, "\0");
	if (bkquote == -1)
		astr = replace_envp_tags(astr, sh);
	ft_putstr_fd(astr, pipefd[1]);
	close(pipefd[1]);
	exit (0);
}

char	*heredoc_main(char *breakchar, int pipefd[2])
{
	char	*astr;
	char	*temp;
	char	*temp2;

	astr = ft_strdup("");
	free(breakchar);
	close (pipefd[1]);
	while (1)
	{
		temp = get_next_line(pipefd[0]);
		if (temp == NULL)
			break ;
		temp2 = astr;
		astr = ft_strjoin(temp2, temp);
		free (temp2);
		free (temp);
	}
	free (temp);
	close (pipefd[0]);
	return (astr);
}

void	*ft_heredoc(char *breakchar, int hdocs, t_shell *sh)
{
	int		pid;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		errexit("error :", "pipe initialization", NULL, sh);
	pid = fork();
	if (pid == -1)
		errexit("Error:", "fork failure", NULL, sh);
	if (pid == 0)
		heredoc_child(pipefd, breakchar, sh, hdocs);
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, NULL, 0);
		return (heredoc_main(breakchar, pipefd));
	}
	return (NULL);
}
