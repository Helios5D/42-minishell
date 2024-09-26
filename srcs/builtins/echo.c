/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 10:24:55 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/21 12:17:32 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_arg(t_cmd *cmd, int *i)
{
	int	j;
	int	is_n_arg;

	is_n_arg = 0;
	while (cmd->argv[*i])
	{
		j = 1;
		if (cmd->argv[*i][0] != '-')
			return (is_n_arg);
		while (cmd->argv[*i][j])
			if (cmd->argv[*i][j++] != 'n')
				return (is_n_arg);
		is_n_arg = 1;
		(*i)++;
	}
	return (is_n_arg);
}

/**
 * @brief Displays the given Strings
 *
 * This function is a builtin command which displays the given
 * Strings, one after another and separated by spaces. If the -n
 * argument was given, no newline is displayed at the end.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
void	ft_echo(t_data *data, t_cmd *cmd)
{
	int		is_newline;
	int		i;

	is_newline = 1;
	i = 1;
	if (cmd->argv[1] && is_n_arg(cmd, &i))
		is_newline = 0;
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[++i])
			ft_putchar_fd(' ', 1);
	}
	if (is_newline)
		ft_putchar_fd('\n', 1);
	data->err_code = 0;
}
