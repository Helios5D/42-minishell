/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:07:45 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/05 09:07:59 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	char			*str;
	unsigned char	ch;

	str = (char *)s;
	ch = (unsigned char)c;
	i = 0;
	while (str[i] && str[i] != ch)
		i++;
	if (str[i] == ch)
		return (str + i);
	return (NULL);
}
