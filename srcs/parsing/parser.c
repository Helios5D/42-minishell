/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:51:16 by hdaher            #+#    #+#             */
/*   Updated: 2024/08/22 11:52:42 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief This function interprets a string as a redirection type.
 *
 * This function compares a string with all redirections to find it's
 * type and returns the t_redir type that corresponds to the strng.
 *
 * @param str String that contains a redirection.
 * @return t_redir Redirection type that corresponds to the string.
 */
t_redir	get_redirection(char *str)
{
	if (ft_strcmp(str, ">") == 0)
		return (REDIRECT_OUTPUT);
	else if (ft_strcmp(str, ">>") == 0)
		return (REDIRECT_OUTPUT_APPEND);
	else if (ft_strcmp(str, "<") == 0)
		return (REDIRECT_INPUT);
	else if (ft_strcmp(str, "<<") == 0)
		return (HEREDOC);
	return (NO_REDIR);
}

/**
* @brief Creates a cmd from a pointer to a pointer to a token in a tokens list
 *
 * This function iterates through a list of tokens from token and
 * creates a t_cmd that corresponds to the command including redirecitons.
 * It stops iterating when finding a pipe or the end of the list of tokens
 * and returns a poiter to the created t_cmd.
 *
 * @param token Pointer to the first element of a list of tokens
 * @return t_cmd* A pointer to the created command
 */
static t_cmd	*parse_command(t_token **token)
{
	t_cmd	*temp_cmd;
	int		is_redir;

	is_redir = 1;
	temp_cmd = ft_cmd_new();
	if (!temp_cmd)
		return (NULL);
	temp_cmd->argv = create_argv(token);
	if (!temp_cmd->argv)
		return (free_commands(&temp_cmd), NULL);
	if ((*token) && (*token)->type == REDIRECT)
	{
		temp_cmd->redir = get_redirection((*token)->content);
		(*token) = (*token)->next;
	}
	while ((*token) && (*token)->type != PIPE)
	{
		if (!fill_redirections(&(temp_cmd->next_file), (*token), &is_redir))
			return (free_commands(&temp_cmd), NULL);
		(*token) = (*token)->next;
	}
	return (temp_cmd);
}

/**
 * @brief Transforms a list of tokens into a list of commands to execute.
 *
 * This function takes a pointer to a list of tokens representing commands,
 * arguments and operators and constructs a list of t_cmd containing
 * the list of commands between pipes and their redirecstions.
 *
 * @param tokens Pointer to the first element of a list of tokens
 * @return t_cmd* A pointer to the first element of the command list.
 */
static t_cmd	*list_commands(t_data *data, t_token **tokens)
{
	t_cmd	*cmds;
	t_cmd	*temp_cmd;
	t_token	*temp_token;

	temp_token = (*tokens);
	cmds = NULL;
	while (temp_token)
	{
		temp_cmd = parse_command(&temp_token);
		if (!temp_cmd)
			return (free_tokens(*tokens), free_commands(&cmds),
				clean_and_exit(data), NULL);
		ft_cmd_add(&cmds, temp_cmd);
		if (temp_token)
			temp_token = temp_token->next;
	}
	return (cmds);
}

/**
 * @brief Transforms a string into a list of commands to execute.
 *
 * This function takes an inut string as a parameter and
 * parses it into a list of t_cmd containing the list of
 * commands between pipes and their redirections.
 *
 * @param input Input string to parse
 * @return t_cmd* A pointer to the first element of the command list.
 */
t_cmd	*handle_input(char *input, t_data *data)
{
	t_token	*tokens;
	t_cmd	*cmds;

	(void)data;
	if (!input)
		return (NULL);
	tokens = tokenize_input(data, input);
	if (!tokens)
		return (NULL);
	replace_env_vars(data, tokens);
	replace_tilde(data, tokens);
	replace_wildcards(data, tokens);
	if (!conc_tokens_list(&tokens))
		return (free_tokens(tokens), clean_and_exit(data), NULL);
	if (!syntax_check(data, tokens))
		return (free_tokens(tokens), NULL);
	cmds = list_commands(data, &tokens);
	if (cmds && add_cmds_path(data, cmds) == 0)
		return (free_tokens(tokens), free_commands(&cmds),
			clean_and_exit(data), NULL);
	return (free_tokens(tokens), cmds);
}

// // MAIN TO TEST PARSING

// const char* REDIRECTION_TYPE_STRING[] = {
// 	"NO_REDIR",
// 	"REDIRECT_INPUT",
// 	"REDIRECT_OUTPUT",
// 	"HEREDOC",
// 	"REDIRECT_OUTPUT_APPEND"
// };

// const char* TOKEN_TYPE_STRING[] = {
// 	"WORD",
// 	"REDIRECT",
// 	"PIPE",
// 	"NONE"
// };

// void	init_data(t_data *data, char **envp);

// int	main(int argc, char **argv, char **envp)
// {
// 	t_data	data;

// 	(void)argc;
// 	(void)argv;
// 	init_data(&data, envp);
// 	t_cmd	*commands = handle_input("export PATH=$HOME/s: $HOME/s", &data);
// 	t_cmd	*head_cmd;
// 	t_file	*temp_file;

// 	head_cmd = commands;
// 	handle_heredocs(&data, commands);
// 	printf("\n");
// 	while (commands != NULL)
// 	{
// 		printf("Command : \n");
// 		ft_print_tab(commands->argv);
// 		printf("Redir : %s\n", REDIRECTION_TYPE_STRING[commands->redir]);
// 		temp_file = commands->next_file;
// 		while (temp_file)
// 		{
// 			printf("File : %s\n", temp_file->path);
// 			printf("Next file : %p\n", temp_file->next_file);
// 			printf("Redir : %s\n", REDIRECTION_TYPE_STRING[temp_file->redir]);
// 			if (temp_file->content)
// 				printf("Heredoc content : %s\n", temp_file->content);
// 			temp_file = temp_file->next_file;
// 		}
// 		commands = commands->next_cmd;
// 		printf("\n");
// 	}
// 	data.cmd_line = head_cmd;
// 	clean_and_exit(&data);
// }
