/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mburakow <mburakow@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:08:05 by ahamalai          #+#    #+#             */
/*   Updated: 2024/05/26 14:54:25 by mburakow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		ei;
	char	*s;

	i = 0;
	ei = 0;
	s = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (s == NULL)
		return (0);
	if (!s1)
		s1 = ft_strdup("");
	while (s1[i])
	{
		s[i] = s1[i];
		i++;
	}
	while (s2[ei])
	{
		s[i] = s2[ei];
		i++;
		ei++;
	}
	s[i] = '\0';
	return (s);
}
