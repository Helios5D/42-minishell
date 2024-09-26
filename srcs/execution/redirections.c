/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 11:04:29 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/14 11:10:47 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Redirects the output of a command to a file
 *
 * This function opens and, if needed, creates the file the command
 * is redirected to. If the file exists, its content is entirely
 * erased. It then replaces STDOUT with the file fd.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @param file Struct object containing data about the file
 * @return int Equal to 1 if the file was valid, 0 otherwise
 */
int	redirect_output(t_data *data, t_cmd *cmd, t_file *file)
{
	file->fd = open(file->path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file->fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file->path, 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else if (errno == EISDIR)
			ft_putstr_fd(": Is a directory\n", 2);
		data->err_code = 1;
		return (0);
	}
	if (cmd->std[1] != data->main_stdout)
		close(cmd->std[1]);
	cmd->std[1] = file->fd;
	return (1);
}

/**
 * @brief Redirects the output of a command to a file (but will append it)
 *
 * This function opens and, if needed, creates the file the command
 * is redirected to. If the file exists, its content is not erased.
 * It then replaces STDOUT with the file fd.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @param file Struct object containing data about the file
 * @return int Equal to 1 if the file was valid, 0 otherwise
 */
int	redirect_output_append(t_data *data, t_cmd *cmd, t_file *file)
{
	file->fd = open(file->path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (file->fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file->path, 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else if (errno == EISDIR)
			ft_putstr_fd(": Is a directory\n", 2);
		data->err_code = 1;
		return (0);
	}
	if (cmd->std[1] != data->main_stdout)
		close(cmd->std[1]);
	cmd->std[1] = file->fd;
	return (1);
}

/**
 * @brief Redirects the input of a command to a file
 *
 * This function opens the file and replaces the command STDIN with
 * the file fd.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @param file Struct object containing data about the file
 * @return int Equal to 1 if the file was valid, 0 otherwise
 */
int	redirect_input(t_data *data, t_cmd *cmd, t_file *file)
{
	file->fd = open(file->path, O_RDONLY);
	if (file->fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file->path, 2);
		if (errno == EACCES)
			ft_putstr_fd(": Permission denied\n", 2);
		else if (errno == ENOENT)
			ft_putstr_fd(": No such file or directory\n", 2);
		data->err_code = 1;
		return (0);
	}
	if (cmd->std[0] != data->main_stdin)
		close(cmd->std[0]);
	cmd->std[0] = file->fd;
	return (1);
}

/**
 * @brief Creates a random file path
 *
 * This function first transforms the address of an int (which is pretty
 * much random) to a String. It then joins "/tmp/" with this address to
 * get a random temporary file path, allocated on the heap, which is then
 * returned.
 *
 * @param data Struct object containing all the program data
 * @return String Containing the random temporary file path
 */
char	*get_random_file_path(t_data *data)
{
	int		a;
	char	random_address[20];
	char	*random_file_path;

	ft_addr_to_str(random_address, (unsigned long long)&a);
	random_file_path = ft_strjoin("/tmp/", random_address);
	if (!random_file_path)
		clean_and_exit(data);
	return (random_file_path);
}

/**
 * @brief Redirects the input of a command to a file
 *
 * This function creates a temporary file, writes the content of the
 * heredoc inside it, closes the file to reset the reading position to
 * the start, opens it again in read-only, unlinks it, so that it gets
 * deleted once the fd is closed and then replaces the command STDIN
 * with this file fd.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 * @param file Struct object containing data about the file (and the heredoc)
 * @return int Equal to 0 if something went wrong, 1 otherwise
 */
int	redirect_input_heredoc(t_data *data, t_cmd *cmd, t_file *file)
{
	char	*temp_file_path;

	temp_file_path = get_random_file_path(data);
	file->fd = open(temp_file_path, O_WRONLY | O_CREAT, 0600);
	if (file->fd < 0)
		return (0);
	write(file->fd, file->content, ft_strlen(file->content));
	close(file->fd);
	file->fd = open(temp_file_path, O_RDONLY);
	if (file->fd < 0)
		return (0);
	if (unlink(temp_file_path) < 0)
		return (0);
	free(temp_file_path);
	if (cmd->std[0] != data->main_stdin)
		close(cmd->std[0]);
	cmd->std[0] = file->fd;
	return (1);
}
