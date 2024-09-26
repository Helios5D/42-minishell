/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:41:10 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/22 16:00:15 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	tokenize_char(t_token **tokens,
	t_token *curr_token, char *input, int *i)
{
	if (ft_isspace(input[(*i)]))
		tokenize_space(tokens, curr_token, input + (*i));
	else if (input[(*i)] == '|' && !curr_token->is_in_quote)
		tokenize_pipe(tokens, curr_token);
	else if (input[(*i)] == '>' && input[(*i) + 1] == '>'
		&& !curr_token->is_in_quote)
		tokenize_redir_output_append(tokens, curr_token, i);
	else if (input[(*i)] == '>' && !curr_token->is_in_quote)
		tokenize_redir_output(tokens, curr_token);
	else if (input[(*i)] == '<' && input[(*i) + 1] == '<'
		&& !curr_token->is_in_quote)
		tokenize_heredoc(tokens, curr_token, i);
	else if (input[(*i)] == '<' && !curr_token->is_in_quote)
		tokenize_redir_input(tokens, curr_token);
	else if (input[(*i)] == '\'' && (curr_token->is_in_quote
			|| (!curr_token->is_in_quote && ft_strchr(input + (*i) + 1, '\''))))
		tokenize_simple_quote(tokens, curr_token);
	else if (input[(*i)] == '"' && (curr_token->is_in_quote
			|| (!curr_token->is_in_quote && ft_strchr(input + (*i) + 1, '"'))))
		tokenize_double_quote(tokens, curr_token);
	else if (input[(*i)] == '$' && (curr_token->type == NONE
			|| curr_token->type == WORD))
		tokenize_env_variable(tokens, curr_token, input + (*i));
	else
		tokenize_other(tokens, curr_token, input + (*i));
}

/**
 * @brief Parses a string of commands into a tokens list
 *
 * This function takes a string containing a series of commands
 * and operators and constructs a list of tokens that contains either
 * commands, arguments or operators.
 *
 * @param input String that contains a list of commands and operators
 * @return t_tokens* A pointer to the first element of a list of tokens.
 */
t_token	*tokenize_input(t_data *data, char *input)
{
	t_token		*tokens;
	t_token		curr_token;
	int			i;

	curr_token.content = NULL;
	curr_token.is_err = 0;
	curr_token.to_conc = 0;
	curr_token.is_var = 0;
	curr_token.is_space_after = 0;
	curr_token.is_in_quote = 0;
	curr_token.type = NONE;
	curr_token.next = NULL;
	i = 0;
	tokens = NULL;
	while (input[i])
	{
		tokenize_char(&tokens, &curr_token, input, &i);
		if (curr_token.is_err != 0)
			return (free(curr_token.content), clean_and_exit(data), NULL);
		i++;
	}
	if (curr_token.type != NONE)
		add_token_copy(&tokens, &curr_token);
	return (free(curr_token.content), tokens);
}
