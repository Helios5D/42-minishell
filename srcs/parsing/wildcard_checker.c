/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 17:56:56 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/21 13:42:48 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_wildcard_point(char *file_name, char *wildcard)
{
	int	is_point;

	is_point = 0;
	if (*wildcard == '.')
		is_point = 1;
	while (*file_name)
	{
		if (*file_name == '.' && !is_point)
			return (0);
		else if (*file_name != '.')
			break ;
		file_name++;
	}
	return (1);
}

static void	set_star(char **wildcard, char **star, char **s, char **file_name)
{
	while (**wildcard == '*')
		*wildcard += 1;
	*star = *wildcard;
	*s = *file_name;
}

static void	wildcard_increment(char **file_name, char **wildcard)
{
	*wildcard += 1;
	*file_name += 1;
}

/**
 * @brief This function checks if a file matches with a wildcard pattern
 *
 * This function iterates through a file_name and through a wildcard and
 * compares characters. If same characters are found, it increments both
 * wildcard and file_name. If one or more wildcard characters '*' are found,
 * it keeps the adress of the next character in star and saves the current
 * file_name character in s. If a conflict is found but star is already set,
 * file_name resets to the next character after the star was found (++s), and
 * wildcard resets to the character after the star.

 */
int	check_wildcard_match(char *file_name, char *wildcard)
{
	char	*star;
	char	*s;

	star = NULL;
	s = NULL;
	if (!is_wildcard_point(file_name, wildcard))
		return (0);
	while (*file_name)
	{
		if (*wildcard == *file_name)
			wildcard_increment(&file_name, &wildcard);
		else if (*wildcard == '*')
			set_star(&wildcard, &star, &s, &file_name);
		else if (star)
		{
			wildcard = star;
			file_name = ++s;
		}
		else
			return (0);
	}
	while (*wildcard == '*')
		wildcard += 1;
	return (*wildcard == '\0');
}
