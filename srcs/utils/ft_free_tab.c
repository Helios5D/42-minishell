/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_tab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 14:50:01 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/19 11:38:49 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_tab(void **tab, int n)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	if (n != -1)
	{
		while (i < n)
		{
			free(tab[i]);
			tab[i++] = NULL;
		}
	}
	else
	{
		while (tab[i])
		{
			free(tab[i]);
			tab[i++] = NULL;
		}
	}
	free(tab);
	tab = NULL;
}
