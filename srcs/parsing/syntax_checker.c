/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 08:03:25 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/21 13:42:33 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_syntax_err(t_data *data, char *str)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
	data->err_code = 2;
}

int	syntax_check(t_data *data, t_token *tokens)
{
	t_token	*prev;

	if (tokens->type == PIPE)
		return (print_syntax_err(data, "|"), 0);
	prev = tokens;
	tokens = tokens->next;
	while (tokens)
	{
		if ((prev->type == REDIRECT && tokens->type == REDIRECT)
			|| (prev->type == PIPE && tokens->type == PIPE)
			|| (prev->type == REDIRECT && tokens->type == PIPE))
			return (print_syntax_err(data, tokens->content), 0);
		prev = prev->next;
		tokens = tokens->next;
	}
	if (prev->type == PIPE || prev->type == REDIRECT)
		return (print_syntax_err(data, "newline"), 0);
	return (1);
}
