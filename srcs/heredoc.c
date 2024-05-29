/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburakow <mburakow@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:09:30 by ahamalai          #+#    #+#             */
/*   Updated: 2024/05/28 12:12:17 by mburakow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*make_breakchar(char *breakchar, int *i, int *j, int hdocs)
{
	int		k;
	char	*temp;

	while (breakchar[*i] != '\0')
	{
		if (breakchar[*i] == '<' && breakchar[*i + 1] == '<'
			&& breakchar[*i + 2] != '<')
		{
			*j += 1;
			if (*j == hdocs)
				break ;
		}
		*i += 1;
	}
	k = *i + 2;
	while (breakchar[k] != '\0' && breakchar[k] != '<')
		k++;
	if (breakchar[*i] != ' ')
		breakchar = ft_substr(breakchar, *i + 2, k - *i - 2);
	temp = ft_strjoin(breakchar, "\n");
	free(breakchar);
	return (temp);
}

void	ft_handler_heredoc(int signum)
{
	signum = 0;
	write(1, "\n", 1);
	exit(0);
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

void	*ft_heredoc(char *breakchar, int hdocs)
{
	char	*astr;
	int		i;
	int		j;
	char	*temp;
	int		pid;
	int		pipefd[2];

	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		close(pipefd[0]);
		ft_bzero(&astr, sizeof(char));
		i = 0;
		j = 0;
		temp = make_breakchar(breakchar, &i, &j, hdocs);
		free(breakchar);
		if (heredoc_loop(&astr, temp, j, hdocs) == 0)
		{
			free(temp);
			exit (0);
		}
		free(temp);
		if (astr[0] == '\0')
			ft_strjoin(astr, "\0");
		ft_putstr_fd(astr, pipefd[1]);
		close(pipefd[1]);
		exit (0);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, NULL, 0);
	free(breakchar);
	close (pipefd[1]);
	while (1)
	{
		temp = get_next_line(pipefd[0]);
		if (temp == NULL)
			break ;
		astr = ft_strjoin(astr, temp);
		free (temp);
	}
	free (temp);
	close (pipefd[0]);
	return (astr);
}
