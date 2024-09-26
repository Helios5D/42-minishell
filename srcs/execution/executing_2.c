/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executing_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 16:34:15 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/21 15:15:41 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Executes a builtin command, or returns 0
 *
 * This function tries to execute the builtin command that matches the
 * command that was sent. If no builtin matches the command, it returns
 * 0 so the calling function knows it was not a builtin.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @return int 1 if it's a builtin, 0 otherwise
 */
static int	exec_builtin(t_data *data, t_cmd *cmd)
{
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		ft_cd(data, cmd);
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		ft_echo(data, cmd);
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		ft_env(data, cmd);
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ft_exit(data, cmd);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		ft_export(data, cmd);
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		ft_pwd(data);
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		ft_unset(data, cmd);
	else if (!cmd->argv[0])
		return (1);
	else
		return (0);
	return (1);
}

/**
 * @brief Closes all unnecessary fds
 *
 * This function gets executed everytime a new process is created, after
 * the STDIN and STDOUT of this process were replaced with the right fds.
 * As a child process is a copy of the parent process, all the open fds
 * in the main process stay open in the child processes, so this function
 * is called to avoid any unpredictable issues with fds.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
static void	close_all_fds(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmd_line;
	while (cmd)
	{
		close(cmd->std[0]);
		close(cmd->std[1]);
		cmd = cmd->next_cmd;
	}
	close(data->main_stdin);
	close(data->main_stdout);
}

/**
 * @brief Checks if a command path is valid
 *
 * This function checks if a command path is valid. It checks if
 * the path points to an existing directory, in which case it would
 * be invalid. It also checks if it can be executed, otherwise, it's
 * invalid. In case of invalidity, we print the right message and set
 * the corresponding error code, partly thanks to the global errno.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @return int Equal to 1 if it's valid, 0 otherwise
 */
static int	is_cmd_path_valid(t_data *data, t_cmd *cmd)
{
	struct stat	statbuf;

	if (stat(cmd->argv[0], &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->argv[0], 2),
			ft_putstr_fd(": Is a directory\n", 2), data->err_code = 126, 0);
	if (access(cmd->argv[0], X_OK) < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		if (errno == EACCES)
			return (data->err_code = 126,
				ft_putstr_fd(": Permission denied\n", 2), 0);
		else if (errno == ENOENT)
			return (data->err_code = 127,
				ft_putstr_fd(": No such file or directory\n", 2), 0);
		else if (errno == ENOTDIR)
			return (data->err_code = 126,
				ft_putstr_fd(": Not a directory\n", 2), 0);
		else
			return (data->err_code = 127,
				ft_putstr_fd(": Something went wrong\n", 2), 0);
	}
	return (1);
}

/**
 * @brief Checks if a command is valid
 *
 * This function checks if a command is valid. It first checks if it's
 * a path. If it is, it checks if the path is valid. Otherwise, it checks
 * if it's one of our builtins. If it's not, it sets the right error code
 * and prints the right message, because it's considered invalid, since
 * the parsing already replaced existing commands with their path, if
 * present in the PATH env variable.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @return int Equal to 1 if it's valid, 0 otherwise
 */
static int	is_cmd_valid(t_data *data, t_cmd *cmd)
{
	int	i;
	int	is_path;

	i = 0;
	is_path = 0;
	if (!cmd->argv || !cmd->argv[0])
		return (data->err_code = 0, 0);
	while (cmd->argv[0][i])
		if (cmd->argv[0][i++] == '/')
			is_path = 1;
	if (is_path && !is_cmd_path_valid(data, cmd))
		return (0);
	else if (!is_path && ft_strcmp(cmd->argv[0], "cd")
		&& ft_strcmp(cmd->argv[0], "env") && ft_strcmp(cmd->argv[0], "exit")
		&& ft_strcmp(cmd->argv[0], "export") && ft_strcmp(cmd->argv[0], "pwd")
		&& ft_strcmp(cmd->argv[0], "unset") && ft_strcmp(cmd->argv[0], "echo"))
		return (data->err_code = 127, ft_putstr_fd(cmd->argv[0], 2),
			ft_putstr_fd(": command not found\n", 2), 0);
	return (1);
}

/**
 * @brief Checks if a command is valid and executes it
 *
 * This function checks if a command is valid and executes it.
 * It first replaces STDIN and STDOUT of the current process
 * with the corresponding fds and then checks if there's only a
 * builtin. If that's the case, it launches its execution in the main
 * process. Otherwise, it creates a subprocess for every command.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
void	check_and_exec_cmd(t_data *data, t_cmd *cmd)
{
	if (!is_cmd_valid(data, cmd))
		return ;
	dup2(cmd->std[0], STDIN_FILENO);
	dup2(cmd->std[1], STDOUT_FILENO);
	if ((cmd->std[0] == data->cmd_line->std[0]
			&& cmd->std[1] == data->cmd_line->std[1] && !cmd->next_cmd
			&& !exec_builtin(data, cmd)) || cmd->next_cmd
		|| cmd->std[0] != data->cmd_line->std[0]
		|| cmd->std[1] != data->cmd_line->std[1])
	{
		cmd->pid = fork();
		if (cmd->pid == -1)
			clean_and_exit(data);
		else if (cmd->pid == 0)
		{
			close_all_fds(data);
			if (!exec_builtin(data, cmd))
				execve(cmd->argv[0], cmd->argv, data->env);
			clean_and_exit(data);
		}
	}
}
