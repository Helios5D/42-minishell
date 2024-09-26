/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:12:03 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/19 11:41:39 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Frees a list of tokens.
 *
 * This function iterates through a list of tokens and frees each element.
 *
 * @param tokens A pointer to the first element of a list of tokens
 * @return
 */
void	free_tokens(t_token *tokens)
{
	t_token	*temp_token;

	while (tokens)
	{
		temp_token = tokens;
		tokens = tokens->next;
		free(temp_token->content);
		free(temp_token);
	}
}

/**
 * @brief Frees a list of files.
 *
 * This function iterates through a list of files and frees each element.
 *
 * @param tokens A pointer to the first element of a list of files
 * @return
 */
void	free_files(t_file *files)
{
	t_file	*temp_file;

	while (files)
	{
		temp_file = files;
		files = files->next_file;
		if (temp_file->path)
		{
			free(temp_file->path);
			temp_file->path = NULL;
		}
		if (temp_file->content)
		{
			free(temp_file->content);
			temp_file->content = NULL;
		}
		free(temp_file);
		temp_file = NULL;
	}
}

/**
 * @brief Frees a list of commands.
 *
 * This function iterates through a list of commands and frees each element.
 *
 * @param tokens A pointer to the first element of a list of commands
 * @return
 */
void	free_commands(t_cmd **cmd_line)
{
	t_cmd	*cmds;
	t_cmd	*temp_cmd;

	cmds = *cmd_line;
	while (cmds)
	{
		temp_cmd = cmds;
		cmds = cmds->next_cmd;
		free_files(temp_cmd->next_file);
		ft_free_tab((void **)temp_cmd->argv, -1);
		free(temp_cmd);
		temp_cmd = NULL;
	}
	*cmd_line = NULL;
}
