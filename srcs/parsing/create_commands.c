/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:52:29 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/21 13:46:57 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Calculates the number of WORD tokens after a specific token
 *
 * This function takes a pointer to a token list as parameter.
 * It loops through the list as long as it finds tokens of type WORD and
 * iterates an index. It then returns the index.
 *
 * @param tokens Pointer to a list of tokens
 * @return int Number of tokens of type WORD found after the given token.
 */
static int	calculate_argc(t_token **tokens)
{
	int		i;
	int		is_redir;
	t_token	*temp_token;

	temp_token = (*tokens);
	is_redir = 0;
	i = (temp_token->type == REDIRECT);
	while (temp_token && temp_token->type == WORD)
	{
		if (!(temp_token->is_var && !temp_token->content[0]))
			i++;
		temp_token = temp_token->next;
	}
	while (temp_token && temp_token->type != PIPE)
	{
		if (temp_token->type == REDIRECT)
			is_redir = 1;
		else if (temp_token->type == WORD && is_redir)
			is_redir = 0;
		else if (temp_token->type == WORD && !is_redir
			&& !(temp_token->is_var && !temp_token->content[0]))
			i++;
		temp_token = temp_token->next;
	}
	return (i);
}

static int	find_args_in_redir(t_token **temp_token,
	char **argv, int *i, int *is_redir)
{
	while ((*temp_token) && (*temp_token)->type != PIPE)
	{
		if ((*temp_token)->content[0] == '\0' && (*temp_token)->is_var)
		{
			(*temp_token) = (*temp_token)->next;
			continue ;
		}
		if ((*temp_token)->type == REDIRECT)
			*is_redir = 1;
		else if ((*temp_token)->type == WORD && (*is_redir))
			*is_redir = 0;
		else if ((*temp_token)->type == WORD && !(*is_redir))
		{
			argv[(*i)] = ft_strdup((*temp_token)->content);
			if (!argv[(*i)])
				return (0);
			(*i) += 1;
		}
		(*temp_token) = (*temp_token)->next;
	}
	return (1);
}

static int	fill_argv(t_token **temp_token, char **argv, int *i, int *is_redir)
{
	while ((*temp_token) && (*temp_token)->type == WORD)
	{
		if ((*temp_token)->content[0] == '\0' && (*temp_token)->is_var)
		{
			(*temp_token) = (*temp_token)->next;
			continue ;
		}
		argv[(*i)] = ft_strdup((*temp_token)->content);
		if (!argv[(*i)])
			return (0);
		(*temp_token) = (*temp_token)->next;
		(*i) += 1;
	}
	if (find_args_in_redir(temp_token, argv, i, is_redir) == 0)
		return (0);
	return (argv[(*i)] = NULL, 1);
}

/**
 * @brief Fills argv with a command and it's arguments found in tokens
 *
 * This function iterates through tokens and fills argv with every
 * token's content until it finds a non WORD token.
 *
 * @param tokens Pointer to a token in a list of tokens
 * @return
 */
char	**create_argv(t_token **tokens)
{
	int		i;
	int		is_redir;
	char	**argv;
	t_token	*temp_token;

	is_redir = 0;
	argv = malloc((calculate_argc((tokens)) + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	if ((*tokens)->type == REDIRECT && calculate_argc(tokens) == 2)
	{
		argv[0] = malloc(sizeof(char));
		if (!argv[0])
			return (NULL);
		argv[0][0] = '\0';
		argv[1] = NULL;
	}
	temp_token = *tokens;
	if (!fill_argv(&temp_token, argv, &i, &is_redir))
		return (NULL);
	while ((*tokens) && (*tokens)->type == WORD)
		(*tokens) = (*tokens)->next;
	return (argv);
}

/**
 * @brief Fills a list of files with all files found after the tokens pointer
 *
 * This function is called iteratively through the tokens list and
 * adds each file to files. It also changes each file's redir member
 * to it's matching redirection and it's next_file member
 * to a pointer to the next file.
 *
 * @param files Empty list of t_file elements to fill
 * @param tokens List of tokens to search files in.
 * @param is_redir Bool equal to 1 if last token was a redirection
 * @return
 */
int	fill_redirections(t_file **files, t_token *tokens, int *is_redir)
{
	t_file	*temp_file;

	if (tokens->type == REDIRECT)
		return (*is_redir = 1, 1);
	if (tokens->type == WORD && (*is_redir))
	{
		*is_redir = 0;
		temp_file = ft_file_new();
		if (!temp_file)
			return (0);
		temp_file->path = ft_strdup(tokens->content);
		if (!temp_file->path)
			return (0);
		while (tokens)
		{
			if (tokens->type == REDIRECT)
			{
				temp_file->redir = get_redirection(tokens->content);
				break ;
			}
			tokens = tokens->next;
		}
		ft_file_add(files, temp_file);
	}
	return (1);
}
