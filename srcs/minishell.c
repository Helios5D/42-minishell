/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:17:56 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/22 13:12:21 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_received = 0;

int	event(void)
{
	return (0);
}

/**
 * @brief Cleans all the allocated memory and exits the program
 *
 * This function takes an object containing all the program data and frees
 * and cleans all the allocated memory and exits, returning the corresponding
 * error code.
 *
 * @param data Struct object containing all the program data
 */
void	clean_and_exit(t_data *data)
{
	if (data->prompt_line)
		free(data->prompt_line);
	if (data->user)
		free(data->user);
	if (data->env)
		ft_free_tab((void **)data->env, -1);
	if (data->cmd_line)
		free_commands(&data->cmd_line);
	if (data->home_path)
		free(data->home_path);
	if (data->cwd)
		free(data->cwd);
	close(data->main_stdin);
	close(data->main_stdout);
	show_sigquit_symbol(data);
	exit(data->err_code);
}

void	init_data(t_data *data, char **envp)
{
	data->term = NULL;
	data->prompt_line = NULL;
	data->err_code = 0;
	data->cmd_line = NULL;
	data->user = NULL;
	data->home_path = NULL;
	data->cwd = NULL;
	data->main_stdin = dup(STDIN_FILENO);
	data->main_stdout = dup(STDOUT_FILENO);
	data->env = ft_tabdup(envp);
	if (!data->env)
		exit(1);
	if (get_key_index(data->env, "PATH") < 0)
		add_env_variable(data, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", 0);
	data->user = find_value(data, "USER");
	data->home_path = find_value(data, "HOME");
	data->cwd = find_value(data, "PWD");
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
	hide_sigquit_symbol(data);
	rl_event_hook = event;
}

void	reset_line(t_data *data, char *input_line)
{
	g_signal_received = 0;
	rl_done = 0;
	free(input_line);
	free_commands(&data->cmd_line);
	free(data->prompt_line);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input_line;
	t_data	data;

	argv[argc] = argv[argc];
	init_data(&data, envp);
	while (1)
	{
		data.prompt_line = get_prompt(&data);
		input_line = readline(data.prompt_line);
		if (!input_line)
			ft_exit(&data, NULL);
		if (g_signal_received != 1)
		{
			add_history(input_line);
			data.cmd_line = handle_input(input_line, &data);
			handle_heredocs(&data, data.cmd_line);
			exec_line(&data, data.cmd_line);
		}
		if (g_signal_received == 1)
			data.err_code = 130;
		reset_line(&data, input_line);
	}
	return (0);
}
