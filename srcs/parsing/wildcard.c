/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 08:46:06 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/22 11:32:55 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Adds a given file name to the array of matching files
 *
 * This function reallocates the matching_files array to contain one
 * more file name. It then adds the file name and NULL at the end of
 * the array.
 *
 * @param data Struct object containing all the program data
 * @param matching_files Array containing the matching files' names
 * @param tab_size Pointer to the int containing the size of the array
 * @param file_name String containing the file name
 * @return char** The array of matching files, containing the new file name
 */
static char	**file_matches_case(t_data *data, char **matching_files,
		int *tab_size, char *file_name)
{
	matching_files = ft_realloc(matching_files, *tab_size * sizeof(char *),
			(*tab_size + 2) * sizeof(char *));
	if (!matching_files)
		clean_and_exit(data);
	matching_files[*tab_size] = ft_strdup(file_name);
	if (!matching_files[*tab_size])
	{
		ft_free_tab((void **)matching_files, *tab_size);
		clean_and_exit(data);
	}
	matching_files[*tab_size + 1] = NULL;
	(*tab_size)++;
	return (matching_files);
}

int	add_tab_to_tokens(t_token *tokens, char **args)
{
	int		i;
	t_token	*temp_token;
	t_token	*end_token;

	i = 1;
	temp_token = NULL;
	end_token = tokens->next;
	free(tokens->content);
	tokens->content = args[0];
	tokens->next = NULL;
	temp_token = tokens;
	while (args[i])
	{
		tokens = init_token();
		if (!tokens)
			return (free_tokens(end_token), 0);
		temp_token->next = tokens;
		tokens->content = args[i];
		temp_token = tokens;
		if (args[++i])
			tokens = tokens->next;
	}
	tokens->next = end_token;
	return (1);
}

/**
 * @brief Returns a list of files matching a wildcard
 *
 * This function opens the current directory and loops through every entry
 * to find those matching the wildcard. Everytime it finds a corresponding
 * one, it reallocates the matching_files array, adding the new entry to it.
 * It eventually returns the matching_files array.
 *
 * @param data Struct object containing all the program data
 * @param start String composing the beginning of the wildcard
 * @param end String composing the end of the wildcard
 * @return char** The array of matching files
 */
char	**get_matching_files(t_data *data, char *wildcard)
{
	char			**matching_files;
	struct dirent	*entry;
	DIR				*dp;
	int				tab_size;

	matching_files = malloc(sizeof(char *));
	if (!matching_files)
		clean_and_exit(data);
	matching_files[0] = NULL;
	dp = opendir(".");
	if (!dp)
		return (matching_files);
	tab_size = 0;
	entry = readdir(dp);
	while (entry)
	{
		if (check_wildcard_match(entry->d_name, wildcard))
			matching_files = file_matches_case(data,
					matching_files, &tab_size, entry->d_name);
		entry = readdir(dp);
	}
	closedir(dp);
	return (matching_files);
}
