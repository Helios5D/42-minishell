/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 12:44:15 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/15 10:45:46 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_chars(long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n = -n;
	}
	while (n > 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	fill_str(char *str_nb, long n, int *i)
{
	if (n < 0)
	{
		str_nb[*i] = '-';
		n = -n;
		*i = *i + 1;
	}
	if (n < 10)
	{
		str_nb[*i] = n + '0';
		*i = *i + 1;
		return ;
	}
	fill_str(str_nb, n / 10, i);
	str_nb[*i] = n % 10 + '0';
	*i = *i + 1;
}

char	*ft_itoa(int n)
{
	char	*str_nb;
	int		i;

	i = 0;
	str_nb = malloc((count_chars(n) + 1) * sizeof(char));
	if (!str_nb)
		return (NULL);
	fill_str(str_nb, n, &i);
	str_nb[i] = '\0';
	return (str_nb);
}
