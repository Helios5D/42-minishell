/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_addr_to_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:20:53 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/14 10:31:14 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	make_the_recursive(char *str, unsigned long long addr, int *i)
{
	if (addr < 10)
	{
		str[*i] = addr + '0';
		(*i)++;
		return ;
	}
	make_the_recursive(str, addr / 10, i);
	str[*i] = addr % 10 + '0';
	(*i)++;
}

void	ft_addr_to_str(char *str, unsigned long long addr)
{
	int	i;

	i = 0;
	make_the_recursive(str, addr, &i);
	str[i] = '\0';
}
