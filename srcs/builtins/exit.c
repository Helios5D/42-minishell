/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 18:15:32 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/22 15:13:16 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the given exit code is valid
 *
 * This function checks if the given exit code is a valid decimal
 * integer.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @return int Equal to 0 if code is invalid, 1 if it's valid
 */
static int	check_valid_exit_code(const char *nptr)
{
	long	nbr;
	int		indexes[2];
	int		sign;

	nbr = 0;
	indexes[0] = 0;
	indexes[1] = 0;
	sign = 1;
	if (nptr[indexes[0]] == '+' || nptr[indexes[0]] == '-')
		if (!nptr[indexes[0] + 1])
			return (0);
	if (nptr[indexes[0]] == '+' || nptr[indexes[0]] == '-')
		if (nptr[indexes[0]++] == '-')
			sign = -1;
	while (nptr[indexes[0]] == '0')
		indexes[0]++;
	while (nptr[indexes[0]] >= '0' && nptr[indexes[0]] <= '9')
	{
		nbr = nbr * 10 + nptr[indexes[0]++] - '0';
		indexes[1]++;
	}
	if (nptr[indexes[0]] || indexes[1] > 10 || nbr * sign > 2147483647
		|| nbr * sign < -2147483648)
		return (0);
	return (1);
}

/**
 * @brief Gets the exit code from the error code argument
 *
 * This function turns the String into a number, which is between 0 and
 * and 255 (if it's not, it gets it between 0 and 255 with a modulo)
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @return unsigned int The given error code
 */
unsigned int	get_exit_code(const char *nptr)
{
	int		nbr;
	int		i;
	int		sign;

	i = 0;
	nbr = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			sign = -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
		nbr = nbr * 10 + nptr[i++] - '0';
	return ((unsigned int)(nbr * sign % 256));
}

/**
 * @brief Prints "exit" and exits if necessary
 *
 * This function checks if the cmd object is NULL or if the command
 * is not in a subshell. In that case, it prints "exit\n" on stderr
 * (bash doesn't print "exit\n" if executed in a subshell). It then
 * just checks if cmd is NULL (in case of ctrl + D) and if that's
 * the case, it directly exits.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
static void	exit_first_checks(t_data *data, t_cmd *cmd)
{
	if (!cmd || cmd->pid == -2)
		ft_putstr_fd("exit\n", 2);
	if (!cmd)
		clean_and_exit(data);
}

/**
 * @brief Exits current interactive terminal session
 *
 * This function is a builtin command that exits the current
 * interactive terminal session, either with the current err_code or
 * with the given error code. The given code is checked to see
 * if it's valid.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
void	ft_exit(t_data *data, t_cmd *cmd)
{
	int	i;

	exit_first_checks(data, cmd);
	i = 1;
	if (ft_strcmp(cmd->argv[1], "--") == 0)
		i = 2;
	if (ft_tablen(cmd->argv) > i + 1 && check_valid_exit_code(cmd->argv[i]))
	{
		data->err_code = 1;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return ;
	}
	else if (cmd->argv[i])
	{
		if (check_valid_exit_code(cmd->argv[i]))
			data->err_code = get_exit_code(cmd->argv[i]);
		else
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->argv[i], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			data->err_code = 2;
		}
	}
	clean_and_exit(data);
}
