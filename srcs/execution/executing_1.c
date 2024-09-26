/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 10:30:06 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/20 16:17:10 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates fd communication pipes between commands
 *
 * This function uses the pipe function to create communication pipes
 * between 2 fds, so one command STDOUT is linked to another command
 * STDIN. If it's the first command, its STDIN stays the same and
 * if it's the last command, its STDOUT stays the same
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the commands
 */
static void	set_stds(t_data *data, t_cmd *cmd)
{
	int	fd[2];

	cmd->std[0] = data->main_stdin;
	while (cmd)
	{
		if (cmd->next_cmd)
		{
			if (pipe(fd) == -1)
				clean_and_exit(data);
			cmd->std[1] = fd[1];
			cmd->next_cmd->std[0] = fd[0];
		}
		else
			cmd->std[1] = data->main_stdout;
		cmd = cmd->next_cmd;
	}
}

/**
 * @brief Prepares the execution of a command
 *
 * This function prepares the execution of a command (a command is
 * what's between 2 pipes). It goes through every redirection (there
 * could be none) and redirects the input or the output of the command
 * as requested in the command line. It then launches the execution
 * of the command.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
static void	exec_cmd(t_data *data, t_cmd *cmd)
{
	t_file	*file;
	t_redir	*redir;
	int		is_valid;

	file = cmd->next_file;
	redir = &cmd->redir;
	while (file)
	{
		if (*redir == REDIRECT_OUTPUT)
			is_valid = redirect_output(data, cmd, file);
		else if (*redir == REDIRECT_OUTPUT_APPEND)
			is_valid = redirect_output_append(data, cmd, file);
		else if (*redir == REDIRECT_INPUT)
			is_valid = redirect_input(data, cmd, file);
		else if (*redir == HEREDOC)
			is_valid = redirect_input_heredoc(data, cmd, file);
		if (!is_valid)
			return ;
		redir = &file->redir;
		file = file->next_file;
	}
	check_and_exec_cmd(data, cmd);
}

void	wait_for_children(t_data *data)
{
	int		exit_status;
	t_cmd	*cmd;

	cmd = data->cmd_line;
	while (cmd)
	{
		if (cmd->pid > 0)
			if (waitpid(cmd->pid, &exit_status, 0) > 0
				&& WIFEXITED(exit_status))
				data->err_code = WEXITSTATUS(exit_status);
		if (cmd->pid <= 0)
			if (waitpid(-1, &exit_status, 0) > 0 && WIFEXITED(exit_status))
				data->err_code = WEXITSTATUS(exit_status);
		cmd = cmd->next_cmd;
	}
}

/**
 * @brief Executes all the commands of a line and waits until they finish
 *
 * This function first creates pipes between commands and sets them on every
 * cmd object so each cmd can communicate between each other. It then launches
 * the execution of each command at the same time, closing the fds it just used
 * if they can be closed and setting STDIN and STDOUT of the main process back
 * to the originals in case they were changed by a builtin. It then waits for
 * every command to finish, updating the error code with the exit status of
 * the processes.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command line
 */
void	exec_line(t_data *data, t_cmd *cmd)
{
	if (!cmd)
		return ;
	set_stds(data, cmd);
	show_sigquit_symbol(data);
	while (cmd)
	{
		exec_cmd(data, cmd);
		if (cmd->std[0] != data->main_stdin)
			close(cmd->std[0]);
		if (cmd->std[1] != data->main_stdout)
			close(cmd->std[1]);
		dup2(data->main_stdin, STDIN_FILENO);
		dup2(data->main_stdout, STDOUT_FILENO);
		cmd = cmd->next_cmd;
	}
	wait_for_children(data);
	hide_sigquit_symbol(data);
	if (g_signal_received == 2)
		data->err_code = 131;
}
