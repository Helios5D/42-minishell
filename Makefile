NAME = minishell
MACOS = minishell_macos

CC = cc

SRCS_DIR = srcs
PARSING_DIR = parsing
LEXING_DIR = lexing
EXECUTION_DIR = execution
UTILS_DIR = utils
BUILTINS_DIR = builtins
INCLUDES_DIR = includes

UTILS_SRCS =$(UTILS_DIR)/ft_putchar_fd.c $(UTILS_DIR)/ft_putstr_fd.c \
			$(UTILS_DIR)/ft_split.c $(UTILS_DIR)/ft_strcmp.c \
			$(UTILS_DIR)/ft_strdup.c $(UTILS_DIR)/ft_strlen.c \
			$(UTILS_DIR)/ft_strlcat.c $(UTILS_DIR)/ft_realloc.c \
			$(UTILS_DIR)/ft_read_fd.c $(UTILS_DIR)/ft_tablen.c \
			$(UTILS_DIR)/ft_strjoin.c $(UTILS_DIR)/ft_strtrim.c \
			$(UTILS_DIR)/ft_tabdup.c $(UTILS_DIR)/ft_print_tab.c \
			$(UTILS_DIR)/ft_free_tab.c $(UTILS_DIR)/env_utils.c \
			$(UTILS_DIR)/free_structures.c $(UTILS_DIR)/ft_count_char.c \
			$(UTILS_DIR)/ft_copy_and_sort_tab.c $(UTILS_DIR)/ft_isspace.c \
			$(UTILS_DIR)/ft_strchr.c $(UTILS_DIR)/ft_addr_to_str.c \
			$(UTILS_DIR)/ft_itoa.c

BUILTINS_SRCS =	$(BUILTINS_DIR)/cd.c $(BUILTINS_DIR)/echo.c \
				$(BUILTINS_DIR)/env.c $(BUILTINS_DIR)/exit.c \
				$(BUILTINS_DIR)/export_1.c $(BUILTINS_DIR)/export_2.c \
				$(BUILTINS_DIR)/pwd.c $(BUILTINS_DIR)/unset.c

EXECUTION_SRCS =	$(EXECUTION_DIR)/executing_1.c $(EXECUTION_DIR)/executing_2.c \
					$(EXECUTION_DIR)/redirections.c

LEXING_SRCS =	$(LEXING_DIR)/create_tokens.c $(LEXING_DIR)/concatenate_tokens.c \
				$(LEXING_DIR)/tokenize_chars_1.c $(LEXING_DIR)/tokenize_chars_2.c \
				$(LEXING_DIR)/tokenizer.c

PARSING_SRCS =	$(PARSING_DIR)/create_commands.c $(PARSING_DIR)/expand.c \
				$(PARSING_DIR)/parser.c $(PARSING_DIR)/struct_list_op.c \
				$(PARSING_DIR)/syntax_checker.c $(PARSING_DIR)/heredoc.c \
				$(PARSING_DIR)/wildcard.c $(PARSING_DIR)/wildcard_checker.c \
				$(PARSING_DIR)/parsing_replacement.c

SRCS =	$(addprefix $(SRCS_DIR)/, $(LEXING_SRCS)) $(addprefix $(SRCS_DIR)/, $(PARSING_SRCS)) \
		$(addprefix $(SRCS_DIR)/, $(UTILS_SRCS)) $(addprefix $(SRCS_DIR)/, $(EXECUTION_SRCS)) \
		$(addprefix $(SRCS_DIR)/, $(BUILTINS_SRCS)) \
		$(SRCS_DIR)/prompt.c $(SRCS_DIR)/signals.c \
		$(SRCS_DIR)/minishell.c

OBJS_DIR = objs
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

# ANSI color codes
GREEN = \033[1;32m
YELLOW = \033[1;33m
WHITE = \033[1;97m
BLUE = \033[1;34m
CYAN = \033[1;36m
RED = \033[1;31m
RESET = \033[0m

COL1 = \033[38;2;255;182;193m
COL2 = \033[38;2;255;150;175m
COL3 = \033[38;2;255;118;157m
COL4 = \033[38;2;255;86;139m
COL5 = \033[38;2;255;54;121m
COL6 = \033[38;2;255;0;102m

all: CFLAGS = -Wall -Wextra -Werror
all: LDFLAGS = -lreadline
all: $(NAME)

macos: READLINE_DIR += $(shell brew --prefix readline)
macos: CFLAGS = -Wall -Wextra -Werror -I$(READLINE_DIR)/include
macos: LDFLAGS = -L$(READLINE_DIR)/lib -lreadline
macos: $(MACOS)

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INCLUDES_DIR)

$(NAME): $(OBJS)
	@echo "\n$(YELLOW)Linking objects...$(RESET)"
	@$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "$(BLUE)Progress: 100%$(RESET)"
	@echo "$(GREEN)Compilation complete!$(RESET)\n"
	@echo "$(COL1)███╗░░░███╗██╗███╗░░██╗██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░"
	@echo "$(COL2)████╗░████║██║████╗░██║██║██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░"
	@echo "$(COL3)██╔████╔██║██║██╔██╗██║██║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░"
	@echo "$(COL4)██║╚██╔╝██║██║██║╚████║██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░"
	@echo "$(COL5)██║░╚═╝░██║██║██║░╚███║██║██████╔╝██║░░██║███████╗███████╗███████╗"
	@echo "$(COL6)╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝$(RESET)\n"

$(MACOS): $(OBJS)
	@echo "\n$(YELLOW)Linking objects...$(RESET)"
	@$(CC) $(OBJS) -o $(MACOS) $(LDFLAGS)
	@echo "$(BLUE)Progress: 100%$(RESET)"
	@echo "$(GREEN)Compilation complete!$(RESET)\n"
	@echo "$(COL1)███╗░░░███╗██╗███╗░░██╗██╗░██████╗██╗░░██╗███████╗██╗░░░░░██╗░░░░░"
	@echo "$(COL2)████╗░████║██║████╗░██║██║██╔════╝██║░░██║██╔════╝██║░░░░░██║░░░░░"
	@echo "$(COL3)██╔████╔██║██║██╔██╗██║██║╚█████╗░███████║█████╗░░██║░░░░░██║░░░░░"
	@echo "$(COL4)██║╚██╔╝██║██║██║╚████║██║░╚═══██╗██╔══██║██╔══╝░░██║░░░░░██║░░░░░"
	@echo "$(COL5)██║░╚═╝░██║██║██║░╚███║██║██████╔╝██║░░██║███████╗███████╗███████╗"
	@echo "$(COL6)╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═════╝░╚═╝░░╚═╝╚══════╝╚══════╝╚══════╝$(RESET)\n"

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(RED)Cleaned object files.$(RESET)"

fclean: clean
	@rm -f $(NAME) $(MACOS)
	@echo "$(RED)Removed executable.$(RESET)"

re: fclean all

re_macos: fclean macos

.PHONY: all clean fclean re macos re_macos
