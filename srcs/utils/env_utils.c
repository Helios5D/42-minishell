/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 13:56:21 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/22 14:42:27 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_key_index(char **env, char *key)
{
	int	i;
	int	j;
	int	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] == key[j])
		{
			if (j + 1 == key_len)
			{
				if (env[i][j + 1] && env[i][j + 1] == '=')
					return (i);
				else if (!env[i][j + 1])
					return (-i - 2);
				else
					break ;
			}
			j++;
		}
		i++;
	}
	return (-1);
}

void	add_new_env_variable(t_data *data, char *key, char *value, int is_empty)
{
	char	*key_equal;
	int		tablen;

	tablen = ft_tablen(data->env);
	data->env = ft_realloc(data->env, tablen * sizeof(char *),
			(tablen + 2) * sizeof(char *));
	if (!data->env)
		clean_and_exit(data);
	if (!is_empty)
	{
		key_equal = ft_strjoin(key, "=");
		if (!key_equal)
			clean_and_exit(data);
		data->env[tablen] = ft_strjoin(key_equal, value);
		free(key_equal);
	}
	else
		data->env[tablen] = ft_strdup(key);
	if (!data->env[tablen])
		clean_and_exit(data);
	data->env[tablen + 1] = NULL;
}

/**
 * @brief Finds a variable value in env from it's key
 *
 * This function finds a value in env from it's key by finding
 * the corresponding index then calling ft_strdup on the part after the '='.
 *
 * @param data Data structure with the env member
 * @param key Key (name) of the variable to find
 * @return char* Value of the corresponding key
 */
char	*find_value(t_data *data, char *key)
{
	char	*value;
	int		key_index;

	key_index = get_key_index(data->env, key);
	if (key_index < 0)
	{
		value = malloc(sizeof(char));
		if (!value)
			clean_and_exit(data);
		value[0] = '\0';
		return (value);
	}
	value = ft_strdup(data->env[key_index] + ft_strlen(key) + 1);
	if (!value)
		clean_and_exit(data);
	return (value);
}

/**
 * @brief Adds a variable to env
 *
 * This function tranforms a key and value into a string such as "key=value"
 * and adds it to env. It first searches for the key in the table.
 * If the key doesnt exist, the function creates the key and value.
 * If the key already exists, it simply replaces the value.
 *
 * @param data Data structure with the env member
 * @param key Key (name) of the variable to add
 * @param value Value of the variable to add
 */
void	add_env_variable(t_data *data, char *key, char *value, int is_empty)
{
	int		key_index;
	char	*key_equal;

	if (!key || !value)
		return ;
	key_index = get_key_index(data->env, key);
	if (key_index == -1)
		add_new_env_variable(data, key, value, is_empty);
	else if (!is_empty)
	{
		if (key_index < -1)
			key_index = -key_index - 2;
		free(data->env[key_index]);
		key_equal = ft_strjoin(key, "=");
		if (!key_equal)
			clean_and_exit(data);
		data->env[key_index] = ft_strjoin(key_equal, value);
		free(key_equal);
		if (!data->env[key_index])
			clean_and_exit(data);
	}
}

/**
 * @brief Deletes a variable from env
 *
 * This function searches for a key in enve
 * If the key isnt found, the function returns. Otherwise, it deletes
 * the variable from the table.
 *
 * @param data Data structure with the env member
 * @param key Key (name) of the variable to delete
 */
void	*del_env_variable(t_data *data, char *key)
{
	int		indexes[2];
	int		key_index;
	char	**new_env;

	key_index = get_key_index(data->env, key);
	if (key_index == -1)
		return (NULL);
	if (key_index < -1)
		key_index = -key_index - 2;
	indexes[0] = 0;
	indexes[1] = 0;
	new_env = malloc((ft_tablen(data->env)) * sizeof(char *));
	if (!new_env)
		clean_and_exit(data);
	while (data->env[indexes[0]])
	{
		if (indexes[0]++ != key_index)
		{
			new_env[indexes[1]] = ft_strdup(data->env[indexes[0] - 1]);
			if (!new_env[indexes[1]++])
				clean_and_exit(data);
		}
	}
	new_env[indexes[1]] = NULL;
	return (ft_free_tab((void **)data->env, -1), data->env = new_env, NULL);
}
