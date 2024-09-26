/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_chars_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:43:17 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/05 14:33:07 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize_redir_output_append(t_token **tokens,
	t_token *current_token, int *i)
{
	if (current_token->type != NONE)
		add_token_copy(tokens, current_token);
	current_token->type = REDIRECT;
	current_token->to_conc = 0;
	if (!write_in_token(current_token, ">>", 2))
		current_token->is_err = 1;
	add_token_copy(tokens, current_token);
	current_token->type = NONE;
	*i += 1;
}

void	tokenize_redir_output(t_token **tokens, t_token *current_token)
{
	if (current_token->type != NONE)
		add_token_copy(tokens, current_token);
	current_token->type = REDIRECT;
	current_token->to_conc = 0;
	if (!write_in_token(current_token, ">", 1))
		current_token->is_err = 1;
	add_token_copy(tokens, current_token);
	current_token->type = NONE;
}

void	tokenize_redir_input(t_token **tokens, t_token *current_token)
{
	if (current_token->type != NONE)
		add_token_copy(tokens, current_token);
	current_token->type = REDIRECT;
	current_token->to_conc = 0;
	if (!write_in_token(current_token, "<", 1))
		current_token->is_err = 1;
	add_token_copy(tokens, current_token);
	current_token->type = NONE;
}

void	tokenize_heredoc(t_token **tokens, t_token *current_token, int *i)
{
	if (current_token->type != NONE)
		add_token_copy(tokens, current_token);
	current_token->type = REDIRECT;
	current_token->to_conc = 0;
	if (!write_in_token(current_token, "<<", 2))
		current_token->is_err = 1;
	add_token_copy(tokens, current_token);
	current_token->type = NONE;
	*i += 1;
}

void	tokenize_pipe(t_token **tokens, t_token *current_token)
{
	if (current_token->type != NONE)
		add_token_copy(tokens, current_token);
	current_token->type = PIPE;
	current_token->to_conc = 0;
	if (!write_in_token(current_token, "|", 1))
		current_token->is_err = 1;
	add_token_copy(tokens, current_token);
	current_token->type = NONE;
}
