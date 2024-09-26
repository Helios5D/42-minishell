/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:48:22 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/15 16:53:19 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_size;
	size_t	s2_size;
	size_t	i;
	char	*str;

	i = 0;
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	str = malloc((s1_size + s2_size + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2 && s2[i - s1_size])
	{
		str[i] = s2[i - s1_size];
		i++;
	}
	str[i] = '\0';
	return (str);
}
