/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_replacement.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 16:32:38 by mrochedy          #+#    #+#             */
/*   Updated: 2024/10/05 17:38:29 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Replaces tilde ('~') in tokens
 *
 * This function iterates through a list of tokens and checks for
 * those starting with '~'. If the '~' symbol is followed by a char,
 * different than '/' or if it's between quotes, or if the token
 * is to be concatenated with another one (meaning it's followed by
 * an env variable), the '~' symbol is left as is. Otherwise, it's
 * replaced by the home path, saved in the data object.
 *
 * @param data Struct object containing all the program data
 * @param tokens Pointer to the first element of a list of tokens
  */
void	replace_tilde(t_data *data, t_token *tokens)
{
	char	*temp_value;

	while (tokens)
	{
		if (tokens->content[0] == '~' && !tokens->is_in_quote
			&& (!tokens->content[1] || tokens->content[1] == '/')
			&& !tokens->to_conc)
		{
			temp_value = ft_strjoin(data->home_path, tokens->content + 1);
			free(tokens->content);
			if (!temp_value)
				clean_and_exit(data);
			tokens->content = temp_value;
		}
		tokens = tokens->next;
	}
}

static void	err_code_case(t_data *data, t_token *tokens, char **temp_value)
{
	char	*err_code;

	err_code = ft_itoa(data->err_code);
	if (!err_code)
		clean_and_exit(data);
	free(*temp_value);
	*temp_value = ft_strjoin(err_code, tokens->content + 2);
	free(err_code);
	if (!*temp_value)
		clean_and_exit(data);
}

/**
 * @brief This function replace all tokens with env variables with their values
 *
 * This function iterates through a list of tokens and checks for $ symbol
 * at the beginning of each token's content. Upon finding the symbol, if
 * followed by a '?' char, we replace the token's content with the error
 * code, followed by what followed the '?' char. Otherwise, the
 * function searches for the value of the environment variable and replaces
 * the token's content with the value (empty string if key not found)
 *
 * @param data Struct object containing all the program data
 * @param tokens Pointer to the first element of a list of tokens
  */
void	replace_env_vars(t_data *data, t_token *tokens)
{
	char	*temp_value;

	while (tokens)
	{
		temp_value = NULL;
		if (tokens->next && tokens->to_conc && !tokens->is_in_quote
			&& !ft_strcmp(tokens->content, "$") && tokens->next->is_in_quote)
			temp_value = ft_strdup("");
		else if (tokens->content[0] == '$' && tokens->is_in_quote != 1
			&& ft_strlen(tokens->content) > 1 && tokens->content[1] != ' ')
		{
			temp_value = find_value(data, tokens->content + 1);
			if (tokens->content[1] == '?')
				err_code_case(data, tokens, &temp_value);
		}
		if (temp_value)
		{
			free(tokens->content);
			if (!temp_value)
				clean_and_exit(data);
			tokens->content = temp_value;
			tokens->is_var = 1;
		}
		tokens = tokens->next;
	}
}

/**
 * @brief Concatenate a file to the previous list of files
 *
 * This function adds a space and the new file name after the String
 * containing the list of files matching the wildcard.
 *
 * @param value Pointer to the String containing the files
 * @param value_space Pointer to the String containing the files, followed by ' '
 * @param matching_files An array of Strings containing the file names
 * @param i Pointer to the index of the matching file
 * @return int Equal to 0 if something went wrong, 1 otherwise
  */
int	add_file_to_value(char **value, char **value_space,
		char **matching_files, int *i)
{
	if (*value)
	{
		free(*value);
		*value = NULL;
	}
	*value = ft_strjoin(*value_space, matching_files[*i]);
	free(*value_space);
	*value_space = NULL;
	if (!(*value))
		return (ft_free_tab((void **)matching_files, -1), 0);
	if (matching_files[*i + 1])
	{
		*value_space = ft_strjoin(*value, " ");
		free(*value);
		*value = NULL;
		if (!(*value_space))
			return (ft_free_tab((void **)matching_files, -1), 0);
	}
	(*i)++;
	return (1);
}

/**
 * @brief Replaces wildcards in tokens
 *
 * This function iterates through a list of tokens and checks for
 * those containing a single '*' symbol, outside quotes, and not followed
 * by an env variable and gets a list of matching files. It then
 * concatenates each file, separating them with a space character,
 * and replaces the token's content with those.
 *
 * @param data Struct object containing all the program data
 * @param tokens Pointer to the first element of a list of tokens
  */
void	replace_wildcards(t_data *data, t_token *tokens)
{
	char	**matching_files;

	while (tokens)
	{
		if (ft_count_char(tokens->content, '*') >= 1 && !tokens->is_in_quote
			&& !tokens->to_conc)
		{
			matching_files = get_matching_files(data, tokens->content);
			if (matching_files[0] && !add_tab_to_tokens(tokens, matching_files))
				free(tokens);
			free(matching_files);
		}
		tokens = tokens->next;
	}
}
