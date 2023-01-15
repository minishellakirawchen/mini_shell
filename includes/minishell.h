/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/15 11:17:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <string.h>
# include <limits.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "./../libs/libft/libft.h"
# include "./../libs/libgnl/get_next_line.h"
# include "./../libs/libftprintf/ft_printf.h"

/* func return value */
# define SUCCESS	0
# define FAILURE	1

/* fd */
# define READ			0
# define WRITE			1
# define R_FD_INFILE	0
# define R_FD_OUTFILE	1
# define R_FD_HEREDOC	2

/* pid */
# define CHILD_PROCESS	0

/* exit_status */
# define CHDIR_FAILURE				1
# define EXIT_TOO_MANY_ARGS			1
# define FILE_OPEN_ERROR			1
# define CMD_NOT_FOUND				1
# define EXIT_SIGQUIT				131
# define EXIT_NUMERIC_ARGS_REQUIRED	255
# define SYNTAX_ERROR				258

/* string */
# define PATH	"PATH"
# define PWD	"PWD"

# define HEREDOC_TMP_FILE	".heredoc_tmp"

# define ISSPACE_CHARS		"\t\n\v\f\r "
# define SET_CHARS			"\"'"

# define CONTROL_OPERATORS	"|&;()"

# define REDIRECT_IN		"<"
# define REDIRECT_OUT		">"
# define REDIRECT_HEREDOC	"<<"
# define REDIRECT_APPEND	">>"

# define CHR_REDIRECT_IN	'<'
# define CHR_REDIRECT_OUT	'>'

# define CHR_DOLLAR			'$'
# define CHR_QUESTION		'?'

# define CHR_SINGLE_QUOTE	'\''


# define CHA_PATH_DELIM		':'

# define CHR_PIPE			'|'
# define STR_PIPE			"|"
# define STR_AND			"&&"
# define STR_OR				"||"
# define STR_SEMICOLON		";"

# define CHR_CMD_OPTION_FLG		'-'
# define STR_CMD_ECHO_OPTIONS	"n"

# define ALPHABET_CNT			26

# define PROMPT				"minishell $> "

# define min(a, b)	((a) <= (b) ? (a) : (b))
# define max(a, b)	((a) >= (b) ? (a) : (b))

/* ---------------- */
/*  typedef struct  */
/* ---------------- */
typedef struct s_minishell_info		t_info;
typedef struct s_env_elem			t_env_elem;
typedef struct s_tree				t_tree;
typedef struct s_redirect_info		t_redirect_info;
typedef struct s_token				t_token;

/* -------------- */
/*  typedef enum  */
/* -------------- */
typedef enum e_exe_type			t_exe_type;
typedef enum e_export_type		t_export_type;
typedef enum e_input_from		t_input_from;
typedef enum e_output_to		t_output_to;
typedef enum e_token_type		t_token_type; // bonus
typedef enum e_fopen_type		t_fopen_type;

/* -------*/
/*  enum  */
/* ------ */
/* separate group and decide command running order */
//node:branch
//	priority: ; > &&,|| > () > |
//		root		:start_point
//		semicolon	: ;
//		and			: &&
//		or			: ||
//		subshell	: ()
//		pipe		: |
//leaf:bottom
enum e_exe_type
{
	E_NODE_ROOT,
	E_NODE_SEMICOLON,
	E_NODE_AND,
	E_NODE_OR,
	E_NODE_SUBSHELL,
	E_NODE_PIPE,
	E_NODE_NO_PIPE,
	E_LEAF_COMMAND,
};

enum e_export_type
{
	E_VAR_CREATE,
	E_VAR_ADD,
};

enum e_input_from
{
	E_REDIRECT_IN,
	E_HERE_DOC,
	E_STDOUT,
};

enum e_output_to
{
	E_REDIRECT_OUT,
	E_REDIRECT_APPEND,
	E_STDIN,
};

enum e_token_type
{
	e_word,
	e_ope_semicolon,
	e_ope_and,
	e_ope_or,
	e_ope_subshell,
	e_ope_pipe,
};

enum e_fopen_type
{
	e_read,
	e_overwrite,
	e_append,
};

/* -------- */
/*  struct  */
/* -------- */
struct s_redirect_info
{
	t_input_from	input_from;
	t_output_to		ouput_to;
	char 			**infiles;
	char 			**outfiles;
	char			**heredoc_delims;
	char 			*heredoc_file;
	int				r_fd[3]; //in,out,heredoc
};

struct s_env_elem
{
	char	*key;
	char	*value;
};

struct s_token // for bonus
{
	t_token_type	type;
	char			*content;
	t_token			*prev;
	t_token			*next;
};

struct s_tree
{
	char 			**cmds;
	t_redirect_info	*redirect_info;
	t_exe_type		exe_type;
	pid_t			pid;
	t_tree			*prev;
	t_tree			*next;
};

struct s_minishell_info
{
	int		exit_status;
	t_list	*env_list;
	char	**splitted_cmds;
	t_tree	*tree_root;
	int		pid; // use getpid()
	bool	is_exit;
};

typedef void sigfunc(int);

/* ------- */
/*  input  */
/* ------- */
// input.c
int		prompt_loop(t_info *info);
t_list	*get_env_list(void);
char	*get_env_value(const char *search_key, t_list *env_list_head);
int		add_env_elem_to_list(t_list **list_head, char *key, char *value);
int		overwrite_env_value(t_list **list_head, char *search_key, char *value);
int		delete_env_elem(t_list **list_head, char *search_key);

void	sig_handler(int signo);


/* ---------- */
/*  analysis  */
/* ---------- */
// analysis.c
int		analysis(t_info *info, char *readline_input); // tmp
//int		add_redirect_param(t_tree **node);

// pipe_split.c
char	**split_pipe_and_word_controller(const char *readline_input);

// redirection_split.c
char	**split_redirect_and_word_controller(const char **cmds);

// split_helper.c
size_t	count_chr_in_src(const char *src, char chr);
ssize_t	count_split_point_count_by_chr(const char *str, char chr);
size_t	get_split_idx_by_chr(const char *str, char chr);

// analysis_redirect.c
int				valid_redirection(const char **cmds);
t_redirect_info	*get_redirection_info(const char **cmds);

// create_tree.c
int		create_tree(t_info **info);
size_t	count_pipe(char **cmds);

/* ----------- */
/*  execution  */
/* ----------- */
// execute.c
int		execute_command_line(t_info *info);

/* execute_builtin */
bool	is_builtins(const char **cmds);
int		execute_builtin(t_info *info, const char **cmds);

/* execute_redirect.c */
int		handle_fd_for_redirection(t_redirect_info *r_info);
int		openfile_and_heredoc_for_redirect(t_tree **root);
int		execute_redirect(t_tree **root);

/* execute_heredoc.c */
int		execute_heredoc(int fd, const char *delimiter);
/* ----------- */
/*  expansion  */
/* ----------- */
// expansion.c
int		expansion(t_info *info);
int		arrange_command_line(t_info *info);
char	**arrange_cmd_opton(char **src, const char *options);

// expand_variable.c
int		is_name(const char *str);


/* ------ */
/*  exit  */
/* ------ */
// exit.c
void	free_info(t_info **info);
int		free_and_return_no(t_info **info, int exit_status);
char	**free_array(char **array);
int		perror_and_return_int(char *err, int ret_value);
void	*perror_and_return_nullptr(char *err);
void	*free_2d_array_ret_nullptr(void ***array);
void	*free_1d_array_ret_nullptr(void **array);
void	*free_1d_2d_array_ret_nullptr(void **array1d, void ***array2d);


/* --------- */
/*  builtin  */
/* --------- */
// builtin.c
int		ft_echo(t_info *info, const char **cmds);
int		ft_cd(t_info *info, const char **cmds);
int		ft_pwd(t_info *info);
int		ft_export(t_info *info, const char **cmds);
int		ft_unset(t_info *info, const char **cmds);
int		ft_env(t_info *info, const char **cmds);
int		ft_exit(t_info *info, const char **cmds);
/* builtin helper.c */
//char	*get_current_path(void);


/* -------- */
/*  helper  */
/* -------- */
// tree_node_create.c
char	*splitset_and_trim(char *src, char delim, char set, char *trimchar);
t_tree	*create_tree_node(t_exe_type type,  const char **cmds);
size_t	get_2d_array_size(const char **array);

// tree_operation.c
t_tree	*pop_tree_elem_from_top(t_tree **root);
t_tree	*pop_tree_elem_from_bottom(t_tree **root);
void	add_top_of_tree(t_tree **root, t_tree *elem);
void	add_bottom_of_tree(t_tree **root, t_tree *elem);

// tree_helper.c
t_tree	*get_last_node(t_tree *elem);
void	tree_clear(t_tree **root);
size_t	get_tree_size(t_tree *root);

// debug print.c
void	debug_print_stack(t_tree *root, char *str);
void	debug_print_2d_arr(char **arr, char *str);

// env_list.c
int		add_env_elem_to_list(t_list **list_head, char *key, char *value);
int		overwrite_env_value(t_list **list_head, char *search_key, char *value);
int		delete_env_elem(t_list **list_head, char *search_key);
int		append_env_value(t_list **list_head, char *search_key, char *append_value);
char	*get_env_value(const char *search_key, t_list *env_list_head);

// is_same_str.c
int		is_same_str(const char *str1, const char *str2);

#endif
