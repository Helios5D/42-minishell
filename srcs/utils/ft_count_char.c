/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 17:06:37 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/02 17:36:32 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts the occurences of a given char in a String
 *
 * This function counts the number of occurences of a given char in
 * the given String and returns it
 *
 * @param str String we want to count the char in
 * @param c Char that is counted in the String
 * @return int The number of occurences of the given char
 */
int	ft_count_char(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
		if (str[i++] == c)
			count++;
	return (count);
}
