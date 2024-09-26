/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tabdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:36:37 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/03 15:27:43 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_tabdup(char **tab)
{
	int		i;
	char	**new_tab;

	i = 0;
	new_tab = malloc((ft_tablen(tab) + 1) * sizeof(char *));
	if (!new_tab)
		return (NULL);
	while (tab[i])
	{
		new_tab[i] = ft_strdup(tab[i]);
		if (!new_tab[i])
			return (ft_free_tab((void **)new_tab, i), NULL);
		i++;
	}
	new_tab[i] = NULL;
	return (new_tab);
}
