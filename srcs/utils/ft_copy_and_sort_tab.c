/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_copy_and_sort_tab.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 10:15:36 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/03 11:06:23 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_copy_and_sort_tab(char **tab)
{
	int		i;
	int		j;
	char	**tab_cpy;
	char	*tmp;

	tab_cpy = ft_tabdup(tab);
	if (!tab_cpy)
		return (NULL);
	j = 1;
	while (tab_cpy[j])
	{
		i = 0;
		while (tab_cpy[i + j])
		{
			if (ft_strcmp(tab_cpy[i], tab_cpy[i + 1]) > 0)
			{
				tmp = tab_cpy[i];
				tab_cpy[i] = tab_cpy[i + 1];
				tab_cpy[i + 1] = tmp;
			}
			i++;
		}
		j++;
	}
	return (tab_cpy);
}
