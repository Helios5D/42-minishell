/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdaher <hdaher@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 10:51:43 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/22 14:25:37 by hdaher           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Allocates the key_value array
 *
 * This function allocates the key_value array, as well as the Strings
 * it contains to the right size. It counts the amount of chars before
 * and after the first equal sign.
 *
 * @param data Struct object containing all the program data
 * @param str The String containing the key and the value
 * @return char** The empty key_value array
 */
static char	**allocate_key_value(t_data *data, const char *str)
{
	int		i;
	int		j;
	char	**key_value;

	key_value = malloc(sizeof(char *) * 2);
	if (!key_value)
		clean_and_exit(data);
	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		i++;
	while (str[i + j])
		j++;
	key_value[0] = malloc(sizeof(char) * (i + 1));
	if (!key_value[0])
		return (free(key_value), clean_and_exit(data), NULL);
	key_value[1] = malloc(sizeof(char) * (j + 1));
	if (!key_value[1])
		return (ft_free_tab((void **)key_value, 1), clean_and_exit(data), NULL);
	return (key_value);
}

/**
 * @brief Splits the key and the value of the env variable
 *
 * This function first allocates the key_value array and then fills
 * it with what's before the first equal, then with what's after it.
 * This way, we get the key and the value splitted.
 *
 * @param data Struct object containing all the program data
 * @param str The String containing the key and the value
 * @return char** The key_value array
 */
static char	**split_key_value(t_data *data, const char *str)
{
	char	**key_value;
	int		i;
	int		j;

	key_value = allocate_key_value(data, str);
	i = 0;
	j = 0;
	while (str[i] && str[i] != '=')
	{
		key_value[0][i] = str[i];
		i++;
	}
	key_value[0][i] = '\0';
	if (str[i] == '=')
		i++;
	while (str[i + j])
	{
		key_value[1][j] = str[i + j];
		j++;
	}
	key_value[1][j] = '\0';
	return (key_value);
}

/**
 * @brief Deals with an export error
 *
 * This function gets executed when the given key name is invalid.
 * It sets the error code and displays the corresponding error message
 * on the standard error. If the key_value array was passed as argument,
 * it frees it.
 *
 * @param data Struct object containing all the program data
 * @param arg String containing the invalid argument
 * @param key_value Array of Strings containing the key and the value
 */
static void	export_error_case(t_data *data, char *arg, char **key_value)
{
	data->err_code = 1;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	if (key_value)
		ft_free_tab((void **)key_value, 2);
}

/**
 * @brief Updates home_path or cwd in data object, if needed
 *
 * This function first checks if the environment variable that was
 * updated was HOME or PWD. If it was, their value saved in the data
 * object under the name of respectively home_path or cwd are updated.
 *
 * @param data Struct object containing all the program data
 * @param key String containing the key of the updated env variable
 * @param value String containing the value of the updated env variable
 */
static void	update_home_or_cwd(t_data *data, char *key, char *value)
{
	if (!value || !value[0])
		return ;
	if (ft_strcmp(key, "HOME") == 0)
	{
		free(data->home_path);
		data->home_path = ft_strdup(value);
		if (!data->home_path)
			clean_and_exit(data);
	}
	else if (ft_strcmp(key, "PWD") == 0)
	{
		free(data->cwd);
		data->cwd = ft_strdup(value);
		if (!data->cwd)
			clean_and_exit(data);
	}
}

/**
 * @brief Sets new env variables
 *
 * This function sets new env variables if arguments are passed to it,
 * otherwise it just prints a sorted list of env variables. For each passed
 * argument, it checks if an equal sign is present. If there's not any, it
 * just checks if it's valid or not. If there's at least one, it splits the
 * key and the value and checks if the key is valid. If it is, it adds it to
 * the env variables list. In case of error, the right error message is
 * printed and the corresponding error code is set.
 *
 * @param data Struct object containing all the program data
 * @param cmd Struct object containing data about the command
 */
void	ft_export(t_data *data, t_cmd *cmd)
{
	int		i;
	char	**key_value;

	data->err_code = 0;
	if (ft_tablen(cmd->argv) == 1)
		display_env_vars(data);
	i = 0;
	while (cmd->argv[++i])
	{
		key_value = split_key_value(data, cmd->argv[i]);
		if (!check_valid_key_name(key_value[0], ft_strlen(key_value[0])))
		{
			export_error_case(data, cmd->argv[i], key_value);
			continue ;
		}
		if (ft_count_char(cmd->argv[i], '=') == 0)
			add_env_variable(data, key_value[0], key_value[1], 1);
		else
			add_env_variable(data, key_value[0], key_value[1], 0);
		update_home_or_cwd(data, key_value[0], key_value[1]);
		ft_free_tab((void **)key_value, 2);
	}
}
