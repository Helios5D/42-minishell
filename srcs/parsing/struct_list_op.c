/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_list_op.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 15:36:43 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/21 16:46:38 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*ft_cmd_new(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = NULL;
	new->pid = -2;
	new->is_var = 0;
	new->redir = NO_REDIR;
	new->next_file = NULL;
	new->next_cmd = NULL;
	return (new);
}

void	ft_cmd_add(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*head_cmd;

	if (cmd == NULL || new == NULL)
		return ;
	if (*cmd == NULL)
		*cmd = new;
	else
	{
		head_cmd = (*cmd);
		while ((*cmd)->next_cmd != NULL)
			(*cmd) = (*cmd)->next_cmd;
		(*cmd)->next_cmd = new;
		(*cmd) = head_cmd;
	}
}

t_file	*ft_file_new(void)
{
	t_file	*new;

	new = malloc(sizeof(t_file));
	if (!new)
		return (NULL);
	new->path = NULL;
	new->content = NULL;
	new->is_heredoc = 0;
	new->redir = NO_REDIR;
	new->next_file = NULL;
	return (new);
}

void	ft_file_add(t_file **file, t_file *new)
{
	t_file	*head_file;

	if (file == NULL || new == NULL)
		return ;
	if (*file == NULL)
		*file = new;
	else
	{
		head_file = (*file);
		while ((*file)->next_file != NULL)
			(*file) = (*file)->next_file;
		(*file)->next_file = new;
		(*file) = head_file;
	}
}
