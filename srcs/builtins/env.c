/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:58:50 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/22 12:30:09 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Displays all environment variables
 *
 * This function is a builtin command taking no argument which
 * displays all environment variables, separated by a newline.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
void	ft_env(t_data *data, t_cmd *cmd)
{
	int	i;

	if (ft_tablen(cmd->argv) > 1)
	{
		data->err_code = 1;
		ft_putstr_fd("minishell: env: too many arguments\n", 2);
		return ;
	}
	i = 0;
	while (data->env[i])
	{
		if (ft_count_char(data->env[i], '=') > 0)
		{
			ft_putstr_fd(data->env[i], 1);
			ft_putchar_fd('\n', 1);
		}
		i++;
	}
	data->err_code = 0;
}
