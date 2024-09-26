/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:59:30 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/22 14:40:51 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Deals with a cd error
 *
 * This function gets executed when an error happens in the
 * "chdir" method. It prints the right message and sets the
 * error code.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
static void	cd_error_case(t_data *data, char *path)
{
	data->err_code = 1;
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	if (errno == EACCES)
		ft_putstr_fd(": Permission denied", 2);
	else if (errno == ENOENT)
		ft_putstr_fd(": No such file or directory", 2);
	else if (errno == ENOTDIR)
		ft_putstr_fd(": Not a directory", 2);
	else
		ft_putstr_fd(": Something went wrong", 2);
	ft_putchar_fd('\n', 2);
}

/**
 * @brief Updates the PWD and OLDPWD env variables
 *
 * This function updates the PWD and OLDPWD env variables everytime
 * the current directory is changed with "cd". OLDPWD gets the previous PWD
 * and PWD gets the new working directory.
 *
 * @param data Struct object containing all the program data
 */
static void	update_pwd(t_data *data, char *path, int minus, int argc)
{
	char	*cwd;
	char	*old_wd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cd: error retrieving current directory: getcwd: \
cannot access parent directories");
		return ;
	}
	old_wd = find_value(data, "PWD");
	add_env_variable(data, "OLDPWD", old_wd, 0);
	add_env_variable(data, "PWD", cwd, 0);
	free(data->cwd);
	data->cwd = cwd;
	free(old_wd);
	data->err_code = 0;
	if (minus == 0 && argc > 1)
	{
		ft_putstr_fd(path, 1);
		ft_putchar_fd('\n', 1);
	}
}

/**
 * @brief Deals with "-" and "--" arguments
 *
 * This function checks if the given argument was either "-" or "--".
 * A single '-' should change directory to OLDPWD and a double '-' should
 * change directory to HOME. If they are not set, the message VAR not set
 * is printed.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @param path Pointer to the String containing the path to go to
 * @return int 0 if a pattern was found and the var was set, 2 if a
 * pattern was found but the var was not set and 1 if no pattern found
 */
static int	minus_case(t_data *data, char *arg, char **path)
{
	if (ft_strcmp(arg, "-") == 0)
	{
		*path = find_value(data, "OLDPWD");
		if (!*path || *path[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			data->err_code = 1;
			free(*path);
			return (2);
		}
		return (0);
	}
	else if (ft_strcmp(arg, "--") == 0)
	{
		*path = find_value(data, "HOME");
		if (!*path || *path[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			data->err_code = 1;
			free(*path);
			return (2);
		}
		return (0);
	}
	return (1);
}

/**
 * @brief Changes current directory
 *
 * This function is a builtin command which changes the current
 * directory to the requested one. In case of error, the corresponding
 * error code is set and message is printed.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
void	ft_cd(t_data *data, t_cmd *cmd)
{
	int		argc;
	int		minus;
	char	*path;

	path = NULL;
	argc = ft_tablen(cmd->argv);
	if (argc > 2)
	{
		data->err_code = 1;
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return ;
	}
	if (argc == 2)
		path = cmd->argv[1];
	minus = minus_case(data, cmd->argv[1], &path);
	if (argc == 1 && minus == 1)
		minus = minus_case(data, "--", &path);
	if (minus == 2)
		return ;
	if (chdir(path) < 0)
		cd_error_case(data, path);
	else
		update_pwd(data, path, minus, argc);
	if (minus == 0 || argc == 1)
		free(path);
}
