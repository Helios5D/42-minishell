/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:58:48 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/03 14:16:41 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(t_data *data, t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->argv[i])
		del_env_variable(data, cmd->argv[i++]);
	data->err_code = 0;
}
