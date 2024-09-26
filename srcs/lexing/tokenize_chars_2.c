/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_chars_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:45:54 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/22 16:07:53 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenize_simple_quote(t_token **tokens,
	t_token *current_token)
{
	current_token->to_conc = 1;
	if (current_token->type == WORD && !current_token->is_in_quote)
		add_token_copy(tokens, current_token);
	if (!current_token->is_in_quote)
	{
		current_token->is_in_quote = 1;
		current_token->type = WORD;
	}
	else if (current_token->is_in_quote == 1)
	{
		add_token_copy(tokens, current_token);
		current_token->type = NONE;
		current_token->is_in_quote = 0;
	}
	else if (current_token->is_in_quote == 2)
	{
		if (current_token->is_var)
		{
			add_token_copy(tokens, current_token);
			current_token->type = WORD;
			current_token->to_conc = 1;
		}
		if (!write_in_token(current_token, "'", 1))
			current_token->is_err = 1;
	}
}

void	tokenize_double_quote(t_token **tokens,
	t_token *current_token)
{
	current_token->to_conc = 1;
	if (current_token->type == WORD && !current_token->is_in_quote)
	{
		add_token_copy(tokens, current_token);
	}
	if (!current_token->is_in_quote)
	{
		current_token->is_in_quote = 2;
		current_token->type = WORD;
	}
	else if (current_token->is_in_quote == 2)
	{
		add_token_copy(tokens, current_token);
		current_token->type = NONE;
		current_token->is_in_quote = 0;
	}
	else if (current_token->is_in_quote == 1)
	{
		if (!write_in_token(current_token, "\"", 1))
			current_token->is_err = 1;
	}
}

void	tokenize_env_variable(t_token **tokens,
	t_token *current_token, char *str)
{
	if (current_token->type != NONE && current_token->is_in_quote != 1
		&& current_token->content)
	{
		if (current_token->type == WORD)
		{
			current_token->to_conc = 1;
			add_token_copy(tokens, current_token);
		}
		current_token->to_conc = 1;
	}
	current_token->type = WORD;
	if (!write_in_token(current_token, "$", 1))
		current_token->is_err = 1;
	if (*(str + 1) && *(str + 1) != ' ' && *(str + 1) != '"'
		&& *(str + 1) != '\'' && *(str + 1) != '$')
		current_token->is_var = 1;
}

void	tokenize_space(t_token **tokens, t_token *current_token, char *str)
{
	if (!current_token->is_in_quote)
	{
		if (current_token->type != NONE)
		{
			current_token->is_space_after = 1;
			add_token_copy(tokens, current_token);
			current_token->is_space_after = 0;
			current_token->type = NONE;
		}
		current_token->to_conc = 0;
	}
	else if (current_token->is_in_quote == 2 && current_token->is_var)
	{
		current_token->to_conc = 1;
		add_token_copy(tokens, current_token);
		current_token->type = WORD;
		if (!write_in_token(current_token, str, 1))
			current_token->is_err = 1;
	}
	else
	{
		current_token->type = WORD;
		if (!write_in_token(current_token, str, 1))
			current_token->is_err = 1;
	}
}

void	tokenize_other(t_token **tokens, t_token *current_token, char *str)
{
	if (current_token->is_var && !check_valid_key_name(str, 1)
		&& !(*str >= '0' && *str <= '9')
		&& !(*str == '?' && current_token->content))
	{
		current_token->to_conc = 1;
		add_token_copy(tokens, current_token);
		current_token->type = WORD;
		current_token->to_conc = 1;
	}
	if (current_token->type == NONE)
		current_token->type = WORD;
	if (!write_in_token(current_token, str, 1))
		current_token->is_err = 1;
}
