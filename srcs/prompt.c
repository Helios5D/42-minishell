/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:52:07 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/14 17:08:42 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief This function returns the path to be displayed on the prompt
 *
 * This function gets the PATH and HOME environment variables and
 * selects the part of PATH that starts after HOME.
 *
 * @param data Data structure which contains a user variable
 * @return char* Pointer to the path to be displayed on the prompt
  */
char	*get_pwd_rel(t_data *data)
{
	char	*temp_pwd;
	char	*pwd_rel;
	char	*home;
	int		i;

	i = 0;
	home = find_value(data, "HOME");
	while (data->cwd[i] && home[i] && data->cwd[i] == home[i])
		i++;
	if (i == ft_strlen(home) && home[0] == '/'
		&& (data->cwd[i] == '/' || !data->cwd[i]))
	{
		pwd_rel = ft_strdup(data->cwd + i);
		if (!pwd_rel)
			clean_and_exit(data);
		temp_pwd = ft_strjoin("~", pwd_rel);
		free(pwd_rel);
	}
	else
		temp_pwd = ft_strdup(data->cwd);
	if (!temp_pwd)
		clean_and_exit(data);
	return (free(home), temp_pwd);
}

/**
 * @brief This function returns the string to be displayed on the prompt
 *
 * This function gets the user variable from data, and executes a series of
 * ft_strjoin to create the prompt line to be displayed. It also calls
 * get_pwd_rel to get the path without /home/user.$
 *
 * @param data Data structure which contains a user variable
 * @return char* Pointer to the string to be displayed on the prompt
  */
char	*get_prompt(t_data *data)
{
	char	*temp_str_1;
	char	*temp_str_2;
	char	*pwd_rel;
	char	*prompt_line;

	temp_str_1 = ft_strjoin(data->user, "@minishell:");
	if (!temp_str_1)
		clean_and_exit(data);
	pwd_rel = get_pwd_rel(data);
	temp_str_2 = ft_strjoin(temp_str_1, pwd_rel);
	free(temp_str_1);
	free(pwd_rel);
	if (!temp_str_2)
		clean_and_exit(data);
	prompt_line = ft_strjoin(temp_str_2, "$ ");
	free(temp_str_2);
	if (!prompt_line)
		clean_and_exit(data);
	return (prompt_line);
}
