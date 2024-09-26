/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:53:40 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/22 16:08:28 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*init_token(void)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->content = NULL;
	new_token->to_conc = 0;
	new_token->is_var = 0;
	new_token->is_space_after = 0;
	new_token->is_err = 0;
	new_token->is_in_quote = 0;
	new_token->type = WORD;
	new_token->next = NULL;
	return (new_token);
}

/**
 * @brief Writes size characters of a string into a token's content.
 *
 * This function takes a token a string and a size as parameters
 * and writes size characters of the string into the token's content attribute.
 *
 * @param token Token structure to write the string into
 * @param str String to write into tht token's content
 * @param size Number of characters to write from str into the token's content
 * @return int Equal to 0 if the allocation fails, 1 if it suceeds
 */
int	write_in_token(t_token *token, char *str, int size)
{
	int	content_len;

	if (str == NULL)
		return (0);
	if (size == 0)
		return (1);
	if (token->content == NULL)
	{
		content_len = 0;
		token->content = ft_realloc(token->content,
				content_len + 1, content_len + size + 1);
		if (token->content == NULL)
			return (0);
		token->content[0] = '\0';
	}
	else
	{
		content_len = ft_strlen(token->content);
		token->content = ft_realloc(token->content,
				content_len + 1, content_len + size + 1);
		if (token->content == NULL)
			return (0);
	}
	ft_strlcat(token->content, str, content_len + size + 1);
	return (1);
}

/**
 * @brief This functions adds an existing allocated token to a list of tokens
 *
 * This function checks if a token list is empty (NULL).
 * If it is, the pointer to the first element of the list
 * becomes the new token. Otherwise, the last token of the list is changed
 * so that it points to the new_token
 *
 * @param tokens Pointer to a pointer to the first element of a list of tokens
 * @return
 */
static void	adjust_tokens_head(t_token **tokens, t_token *new_token)
{
	t_token	*head_token;

	head_token = *tokens;
	if ((*tokens) == NULL)
		(*tokens) = new_token;
	else
	{
		while ((*tokens)->next)
			(*tokens) = (*tokens)->next;
		(*tokens)->next = new_token;
		(*tokens) = head_token;
	}
}

/**
 * @brief Creates a copy of a token and adds it to a token list.
 *
 * This function copies the type, to_conc and content of a token into
 * a new token and adds it to a token list.
 * The function then erases the string inside temp_token->content.
 *
 * @param tokens Pointer to a list of tokens
 * @param token Pointer to a token
 * @return int Equal to 0 if the allocation fails, 1 if it suceeds
 */
int	add_token_copy(t_token **tokens, t_token *temp_token)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (temp_token->is_err = 1, 0);
	new_token->type = temp_token->type;
	new_token->to_conc = temp_token->to_conc;
	new_token->is_in_quote = temp_token->is_in_quote;
	new_token->is_space_after = temp_token->is_space_after;
	if (temp_token->content)
		new_token->content = ft_strdup(temp_token->content);
	else
		new_token->content = ft_strdup("");
	if (!new_token->content)
		return (free(new_token), temp_token->is_err = 1, 0);
	new_token->is_var = 0;
	new_token->next = NULL;
	adjust_tokens_head(tokens, new_token);
	free(temp_token->content);
	temp_token->is_var = 0;
	temp_token->is_space_after = 0;
	temp_token->content = NULL;
	return (1);
}
