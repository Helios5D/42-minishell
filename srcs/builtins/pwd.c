/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 17:29:31 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/19 17:32:22 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Prints the current working directory
 *
 * This function is a builtin command which prints the full path of
 * the current working directory. The path is dynamically allocated by
 * the getcwd function, so we free it at the end.
 */
void	ft_pwd(t_data *data)
{
	char	*cwd;

	(void)data;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd: cannot access parent directories");
		return ;
	}
	ft_putstr_fd(cwd, 1);
	ft_putchar_fd('\n', 1);
	free(cwd);
}
