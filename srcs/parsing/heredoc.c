/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:32:12 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/21 13:41:06 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_heredoc_line(t_data *data, t_file *file, char *input_line)
{
	char	*input_line_nl;
	char	*temp_file_content;

	input_line_nl = ft_strjoin(input_line, "\n");
	free(input_line);
	if (!input_line_nl)
		clean_and_exit(data);
	if (!file->content)
		file->content = ft_strjoin("", input_line_nl);
	else
	{
		temp_file_content = ft_strjoin(file->content, input_line_nl);
		free(file->content);
		file->content = temp_file_content;
	}
	free(input_line_nl);
	if (!file->content)
		clean_and_exit(data);
}

/**
 * @brief This function replaces a heredoc file with user input
 *
 * This function uses the readline function to ask for user input until
 * the heredoc delimiter is found. It adds each line to the file content
 * by calling the add_heredoc_line function.
 *
 * @param data Pointer to the data structure
 * @param file Pointer to a file structure which is a heredoc
 * @return
 */
static int	get_heredoc(t_data *data, t_file *file)
{
	char	*input_line;
	char	*del;

	file->is_heredoc = 1;
	del = file->path;
	while (1)
	{
		input_line = readline("> ");
		if (g_signal_received == 1)
		{
			rl_done = 0;
			return (0);
		}
		if (!input_line)
		{
			ft_putstr_fd("minishell: warning: here-document \
delimited by end-of-file (wanted `", 2);
			return (ft_putstr_fd(del, 2), ft_putstr_fd("')\n", 2), 1);
		}
		if (ft_strcmp(input_line, del) == 0)
			break ;
		add_heredoc_line(data, file, input_line);
	}
	return (free(input_line), 0);
}

/**
 * @brief This function replaces heredocs in a command list with user input
 *
 * This function iterates through a command list and upon finding a file
 * with a heredoc redirection, calls the get_heredoc function which reads
 * on the standard input and waits for the user to enter the heredoc content.
 *
 * @param data Pointer to the data structure
 * @param cmds Pointer to the first element of a list of commands
 * @return
 */
void	handle_heredocs(t_data *data, t_cmd *cmds)
{
	t_file	*temp_file;
	t_redir	prev_redir;

	while (cmds)
	{
		prev_redir = cmds->redir;
		temp_file = cmds->next_file;
		while (temp_file)
		{
			if (prev_redir == HEREDOC)
				get_heredoc(data, temp_file);
			prev_redir = temp_file->redir;
			temp_file = temp_file->next_file;
		}
		cmds = cmds->next_cmd;
	}
}
