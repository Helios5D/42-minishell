/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 16:32:47 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/22 13:11:50 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if an env variable key is valid
 *
 * This function takes a potential env variable key and checks if
 * it's a valid env variable key. A valid env variable key is not
 * empty, does not start with a number and contains only letters,
 * numbers and underscores
 *
 * @param key The key to check
 * @return int 1 if the key is valid, 0 otherwise
 */
int	check_valid_key_name(const char *key, int size)
{
	int	i;

	if (!key || !key[0] || (key[0] >= '0' && key[0] <= '9'))
		return (0);
	i = 0;
	while (key[i] && i < size)
	{
		if (key[i] != '_' && (key[i] < 'a' || key[i] > 'z')
			&& (key[i] < 'A' || key[i] > 'Z')
			&& (key[i] < '0' || key[i] > '9'))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Displays all environment variables in alphabetic order
 *
 * This function is called in case the export builtin is sent without a
 * parameter. It displays all the environment variables in alphabetic
 * order, preceded by "declare -x "
 *
 * @param data Struct object containing all the program data
 */
void	display_env_vars(t_data *data)
{
	int		i;
	int		j;
	char	**env_copy;

	env_copy = ft_copy_and_sort_tab(data->env);
	if (!env_copy)
		clean_and_exit(data);
	i = -1;
	while (env_copy[++i])
	{
		ft_putstr_fd("declare -x ", 1);
		j = 0;
		while (env_copy[i][j] && env_copy[i][j] != '=')
			ft_putchar_fd(env_copy[i][j++], 1);
		if (env_copy[i][j])
		{
			ft_putchar_fd(env_copy[i][j++], 1);
			ft_putchar_fd('"', 1);
			while (env_copy[i][j])
				ft_putchar_fd(env_copy[i][j++], 1);
			ft_putchar_fd('"', 1);
		}
		ft_putchar_fd('\n', 1);
	}
	ft_free_tab((void **)env_copy, -1);
}
