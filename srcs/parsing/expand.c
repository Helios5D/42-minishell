/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 15:39:17 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/21 13:38:28 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_path(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static char	**get_path_tab(t_data *data)
{
	char	*env_path;
	char	**path_tab;

	env_path = find_value(data, "PATH");
	path_tab = ft_split(env_path, ':');
	free(env_path);
	return (path_tab);
}

static int	check_add_cmd_path(char **path_tab, t_cmd *cmd)
{
	int		i;
	char	*temp_path;
	char	*temp_file_path;

	i = 0;
	while (path_tab[i])
	{
		temp_file_path = ft_strjoin("/", cmd->argv[0]);
		temp_path = ft_strjoin(path_tab[i], temp_file_path);
		free(temp_file_path);
		if (!temp_path)
			return (0);
		if (access(temp_path, 0) == 0)
		{
			free(cmd->argv[0]);
			cmd->argv[0] = temp_path;
			break ;
		}
		else
			free(temp_path);
		i++;
	}
	return (1);
}

/**
 * @brief This function adds paths to commands in a command list
 *
 * This function iterats through a list of commands and checks for
 * commands which arent builtins. Upon finding one, the function checks
 * if the command exists in every path in the PATH environment variable
 * and if the path is found, it adds the path to argv[0].
 *
 * @param data Data structure which contains the env table
 * @param cmds Pointer to the first element of a list of commands
 * @return
  */
int	add_cmds_path(t_data *data, t_cmd *cmds)
{
	char	**path_tab;

	path_tab = get_path_tab(data);
	if (!path_tab)
		return (0);
	while (cmds)
	{
		if ((cmds->argv[0]) && (cmds->argv[0][0]))
		{
			if (!is_path(cmds->argv[0]) && ft_strcmp(cmds->argv[0], "echo") != 0
				&& ft_strcmp(cmds->argv[0], "cd") != 0
				&& ft_strcmp(cmds->argv[0], "pwd") != 0
				&& ft_strcmp(cmds->argv[0], "export") != 0
				&& ft_strcmp(cmds->argv[0], "unset") != 0
				&& ft_strcmp(cmds->argv[0], "env") != 0
				&& ft_strcmp(cmds->argv[0], "exit") != 0)
			{
				if (check_add_cmd_path(path_tab, cmds) == 0)
					return (ft_free_tab((void **)path_tab, -1), 0);
			}
		}
		cmds = cmds->next_cmd;
	}
	return (ft_free_tab((void **)path_tab, -1), 1);
}
