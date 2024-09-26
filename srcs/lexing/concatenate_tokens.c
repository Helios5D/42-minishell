/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concatenate_tokens.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 08:27:52 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/22 09:57:37 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	join_token_content(t_token **new_token, t_token **tokens)
{
	char	*temp_content;

	if (!(*new_token)->content)
		(*new_token)->content = ft_strjoin("", (*tokens)->content);
	else
	{
		temp_content = (*new_token)->content;
		(*new_token)->content = ft_strjoin(temp_content, (*tokens)->content);
		free(temp_content);
	}
}

static t_token	*create_conc_token(t_token **tokens)
{
	t_token	*new_token;

	new_token = init_token();
	if (!new_token)
		return (NULL);
	if ((*tokens) && (*tokens)->to_conc && (*tokens)->is_in_quote)
		new_token->is_in_quote = 1;
	while ((*tokens) && (*tokens)->to_conc)
	{
		if ((*tokens)->is_var)
			new_token->is_var = 1;
		join_token_content(&new_token, tokens);
		if (!new_token->content)
			return (NULL);
		if ((*tokens)->is_space_after)
		{
			(*tokens) = (*tokens)->next;
			break ;
		}
		(*tokens) = (*tokens)->next;
	}
	return (new_token->next = (*tokens), new_token);
}

static void	free_conc_tokens(t_token *tokens)
{
	t_token	*temp_token;

	while (tokens && tokens->to_conc)
	{
		temp_token = tokens;
		tokens = tokens->next;
		free(temp_token->content);
		if (temp_token->is_space_after)
		{
			free(temp_token);
			break ;
		}
		free(temp_token);
	}
}

static int	conc_tokens(t_token **tokens,
	t_token **temp_token, t_token **prev_token, t_token **head_token)
{
	(*temp_token) = create_conc_token((tokens));
	if (!(*temp_token))
		return (0);
	if ((*prev_token))
	{
		free_conc_tokens((*prev_token)->next);
		(*prev_token)->next = (*temp_token);
	}
	else
	{
		free_conc_tokens((*head_token));
		(*head_token) = (*temp_token);
	}
	(*tokens) = (*temp_token);
	return (1);
}

/**
 * @brief Concatenates all tokens which need to be concatenated
 *
 * This function iterates through a list of tokens and concatenates
 * every token which has a to_conc member equal to one.
 *
 * @param tokens Pointer to the first element of a list of tokens
 * @return
 */
int	conc_tokens_list(t_token **tokens)
{
	t_token	*temp_token;
	t_token	*prev_token;
	t_token	*head_token;

	head_token = (*tokens);
	prev_token = NULL;
	while (*tokens)
	{
		if ((*tokens)->to_conc)
		{
			if (conc_tokens(tokens, &temp_token, &prev_token, &head_token) == 0)
				return (0);
		}
		if (*tokens)
			(*tokens) = (*tokens)->next;
		if (!prev_token)
			prev_token = head_token;
		else
			prev_token = prev_token->next;
	}
	(*tokens) = head_token;
	return (1);
}
