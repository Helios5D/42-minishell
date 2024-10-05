/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrochedy <mrochedy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/26 14:56:20 by mrochedy          #+#    #+#             */
/*   Updated: 2024/10/05 20:34:21 by mrochedy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* INCLUDES */

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <termios.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

/* GLOBALS */

extern int	g_signal_received;

/* STRUCTURES */

typedef enum e_token_type
{
	WORD,
	REDIRECT,
	PIPE,
	NONE
}	t_token_type;

typedef enum e_redir
{
	NO_REDIR,
	REDIRECT_INPUT,
	REDIRECT_OUTPUT,
	HEREDOC,
	REDIRECT_OUTPUT_APPEND
}	t_redir;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	int				is_in_quote;
	int				is_var;
	int				is_space_after;
	int				to_conc;
	int				is_err;
	struct s_token	*next;
}	t_token;

typedef struct s_file
{
	char			*path;
	int				fd;
	int				is_heredoc;
	char			*content;
	t_redir			redir;
	struct s_file	*next_file;
}	t_file;

typedef struct s_cmd
{
	char			**argv;
	int				is_var;
	int				pid;
	int				std[2];
	t_redir			redir;
	t_file			*next_file;
	struct s_cmd	*next_cmd;
}	t_cmd;

typedef struct s_dict
{
	char			*key;
	char			*value;
	struct s_dict	*next;
}	t_dict;

typedef struct s_data
{
	int				err_code;
	int				main_stdin;
	int				main_stdout;
	char			*home_path;
	char			*cwd;
	char			*user;
	char			*prompt_line;
	char			**env;
	t_cmd			*cmd_line;
	struct termios	*term;
}	t_data;

/* PROTOTYPES */

void	clean_and_exit(t_data *data);

// LEXING
// tokenize_chars_1.c
void	tokenize_redir_output_append(t_token **tokens,
			t_token *current_token, int *i);
void	tokenize_redir_output(t_token **tokens, t_token *current_token);
void	tokenize_redir_input(t_token **tokens, t_token *current_token);
void	tokenize_heredoc(t_token **tokens, t_token *current_token, int *i);
void	tokenize_pipe(t_token **tokens, t_token *current_token);
// tokenize_chars_2.c
void	tokenize_simple_quote(t_token **tokens, t_token *current_token);
void	tokenize_double_quote(t_token **tokens, t_token *current_token);
void	tokenize_env_variable(t_token **tokens,
			t_token *current_token, char *str);
void	tokenize_space(t_token **tokens, t_token *current_token, char *str);
void	tokenize_other(t_token **tokens, t_token *current_token, char *str);
// createa_tokens.c
t_token	*init_token(void);
int		write_in_token(t_token *token, char *str, int size);
int		add_token_copy(t_token **tokens, t_token *temp_token);
// concatenate_tokens.c
int		conc_tokens_list(t_token **tokens);
// tokenizer.c
t_token	*tokenize_input(t_data *data, char *input);

// PARSING
// parser.c
t_redir	get_redirection(char *str);
t_cmd	*handle_input(char *input, t_data *data);
// heredoc.c
void	handle_heredocs(t_data *data, t_cmd *cmds);
// parser_syntax.c
int		syntax_check(t_data *data, t_token *tokens);
// parsing_replacement.c
void	replace_tilde(t_data *data, t_token *tokens);
void	replace_env_vars(t_data *data, t_token *tokens);
void	replace_wildcards(t_data *data, t_token *tokens);
// expand.c
int		add_cmds_path(t_data *data, t_cmd *cmds);
// create_commands.c
char	**create_argv(t_token **tokens);
int		fill_redirections(t_file **files, t_token *tokens, int *is_redir);
// wildcard.c
int		add_tab_to_tokens(t_token *tokens, char **args);
char	**get_matching_files(t_data *data, char *wildcard);
char	**ft_split_wildcard(t_data *data, char *wildcard);
// wildcard_checker.c
int		check_wildcard_match(char *file_name, char *wildcard);

// BUILTINS
void	ft_cd(t_data *data, t_cmd *cmd);
void	ft_echo(t_data *data, t_cmd *cmd);
void	ft_env(t_data *data, t_cmd *cmd);
void	ft_exit(t_data *data, t_cmd *cmd);
void	ft_export(t_data *data, t_cmd *cmd);
void	ft_pwd(t_data *data);
void	ft_unset(t_data *data, t_cmd *cmd);

// EXPORT BUILTIN UTILS
void	display_env_vars(t_data *data);
int		check_valid_key_name(const char *key, int size);

// EXECUTION
void	exec_line(t_data *data, t_cmd *cmd);
void	check_and_exec_cmd(t_data *data, t_cmd *cmd);
int		redirect_output(t_data *data, t_cmd *cmd, t_file *file);
int		redirect_output_append(t_data *data, t_cmd *cmd, t_file *file);
int		redirect_input(t_data *data, t_cmd *cmd, t_file *file);
int		redirect_input_heredoc(t_data *data, t_cmd *cmd, t_file *file);

// PROMPT
char	*get_pwd_rel(t_data *data);
char	*get_prompt(t_data *data);

// SIGNALS
void	sigint_handler(int sig);
void	sigquit_handler(int sig);
void	hide_sigquit_symbol(t_data *data);
void	show_sigquit_symbol(t_data *data);

// UTILS
char	*ft_strchr(const char *s, int c);
int		ft_isspace(char c);
char	*ft_itoa(int n);
int		ft_count_char(const char *str, char c);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *str, int fd);
int		ft_strlen(const char *str);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strdup(const char *s);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
char	*ft_strtrim(char const *s1, char const *set);
void	*ft_realloc(void *ptr, size_t original_size, size_t new_size);
char	*ft_read_fd(int fd);
void	ft_free_tab(void **tab, int n);
int		ft_tablen(char **tab);
char	**ft_tabdup(char **tab);
void	ft_print_tab(char **tab);
char	**ft_copy_and_sort_tab(char **tab);
void	ft_addr_to_str(char *str, unsigned long long addr);

void	free_tokens(t_token *tokens);
void	free_files(t_file *files);
void	free_commands(t_cmd **cmds);

int		get_key_index(char **env, char *key);
void	add_new_env_variable(t_data *data,
			char *key, char *value, int is_empty);
char	*find_value(t_data *data, char *key);
void	add_env_variable(t_data *data, char *key, char *value, int is_empty);
void	*del_env_variable(t_data *data, char *key);

t_cmd	*ft_cmd_new(void);
void	ft_cmd_add(t_cmd **cmd, t_cmd *new);
t_file	*ft_file_new(void);
void	ft_file_add(t_file **file, t_file *new);

#endif
