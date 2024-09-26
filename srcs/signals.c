/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 13:56:51 by mrochedy          #+#    #+#             */
/*   Updated: 2024/08/20 13:40:01 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the interruption signal (SIGINT)
 *
 * This function is called every time the interruption signal is received
 * by the program. It redisplays the input prompt on a newline and sets
 * the g_sigint_received global to 1, so that readline does not also
 * redisplay the prompt when a command is interrupted with the interruption
 * signal.
 *
 * @param sig Int containing the type of signal received
 */
void	sigint_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	g_signal_received = 1;
	rl_done = 1;
}

/**
 * @brief Handles the quit signal (SIGQUIT)
 *
 * This function is called every time the quit signal is received
 * by the program. It's actually only executed when a child process
 * receives SIGQUIT because for the rest of the execution, the
 * attributes of the terminal have been updated so SIGQUIT is not
 * sent with CTRL + \. It prints "Quit (core dumped)" to replicate
 * bash behavior.
 *
 * @param sig Int containing the type of signal received
 */
void	sigquit_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("Quit (core dumped)\n", 1);
	g_signal_received = 2;
}

/**
 * @brief Prevents SIGQUIT from printing "^\" when received
 *
 * This function is called once at the beginning of the program. It stores
 * the attributes of the terminal in the data object and updates them to
 * disable the printing of the "^\" symbol when receiving SIGQUIT.
 *
 * @param data Struct object containing all the program data
 */
void	hide_sigquit_symbol(t_data *data)
{
	if (!data->term)
	{
		data->term = malloc(sizeof(struct termios));
		if (!data->term)
			clean_and_exit(data);
		tcgetattr(STDIN_FILENO, data->term);
	}
	data->term->c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, data->term);
}

/**
 * @brief Allows SIGQUIT to print "^\" when received
 *
 * This function is called once at the end of the program. It enables the
 * printing of the "^\" symbol when receiving SIGQUIT to restore the
 * original behavior of the terminal.
 *
 * @param data Struct object containing all the program data
 */
void	show_sigquit_symbol(t_data *data)
{
	if (data->term)
	{
		data->term->c_cc[VQUIT] = 034;
		tcsetattr(STDIN_FILENO, TCSANOW, data->term);
		free(data->term);
		data->term = NULL;
	}
}
