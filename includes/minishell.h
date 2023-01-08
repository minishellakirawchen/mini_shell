/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/08 21:39:22 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <string.h>
# include <limits.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "./../libs/libft/libft.h"
# include "./../libs/libftprintf/ft_printf.h"

/* func return value */
# define SUCCESS	0
# define FAILURE	1

/* fd */
# define READ			0
# define WRITE			1

/* pid */
# define CHILD_PROCESS	0

/* exit_status */
# define CHDIR_FAILURE				1
# define EXIT_TOO_MANY_ARGS			1
# define CMD_NOT_FOUND				127
# define EXIT_NUMERIC_ARGS_REQUIRED	255
# define SYNTAX_ERROR				258

/* string */
# define	PATH				"PATH"
# define	PWD					"PWD"
# define 	PATH_DELIMITER		':'
# define	ISSPACE				"\t\n\v\f\r "
# define	REDIRECT_IN			"<"
# define	REDIRECT_OUT		">"
# define	REDIRECT_HEREDOC	"<<"
# define	REDIRECT_APPEND		">>"



/* ---------------- */
/*  typedef struct  */
/* ---------------- */
typedef struct s_minishell_param	t_info;
typedef struct s_env_elem			t_env_elem;
typedef struct s_tree				t_tree;
typedef struct s_redirect_info		t_redirect_info;

/* -------------- */
/*  typedef enum  */
/* -------------- */
typedef enum e_exe_type			t_exe_type;
typedef enum e_export_type		t_export_type;
typedef enum e_input_from		t_input_from;
typedef enum e_output_to		t_output_to;


/* -------*/
/*  enum  */
/* ------ */
/* separate group and decide command running order */
//node:branch
//	priority: ; > &&,|| > () > |
//		root		:start_point
//		semicoron	: ;
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
	E_NODE_SHELL,
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
	E_STDIN,
};

enum e_output_to
{
	E_REDIRECT_OUT,
	E_REDIRECT_APPEND,
	E_STDOUT,
};

/* -------- */
/*  struct  */
/* -------- */
//TODO: <, <<, >, >> が複数混在するケースに対応するためには...?
struct s_redirect_info
{
	t_input_from	input_from;
	t_output_to		ouput_to;
	char 			**infiles;
	char 			**outfiles;
	char			**here_doc_limiters;
};

struct s_env_elem
{
	char	*key;
	char	*value;
};

struct s_tree
{
	t_exe_type		exe_type;
	char 			**cmds;
	t_redirect_info	*redirect_info;
	t_tree			*left;
	t_tree			*right;
};

struct s_minishell_param
{
	int		exit_status;
	t_list	*env_list;
	char	*input_line;
	t_tree	*tree_root;
};


/* ------- */
/*  input  */
/* ------- */
// input.c
int		prompt_loop(t_info *info);
t_list	*get_env_list(void);
char	*get_env_value(char *search_key, t_list *env_list_head);
int		add_env_elem_to_list(t_list **list_head, char *key, char *value);
int		overwrite_env_value(t_list **list_head, char *search_key, char *value);
int		delete_env_elem(t_list **list_head, char *search_key);


/* ---------- */
/*  analysis  */
/* ---------- */
// analysis.c
int		analysis(t_info *info); // tmp
int		add_redirect_param(t_tree **node);

// redirection_split.c
char **split_redirect_and_word_controller(const char **cmds);

// redirection_split_helper.c
size_t	count_chr_in_src(const char *src, char chr);
ssize_t	count_split_point_count_by_chr(const char *str, char chr, bool *is_error);
ssize_t	get_split_redirect_char_size(const char **cmds);
size_t	get_split_idx_by_chr(const char *str, char chr);


/* ----------- */
/*  execution  */
/* ----------- */
// execute.c
int		execute_command_line(t_info *info);

/* execute_builtin */
bool	is_builtins(char *cmd_head);
int		execute_builtins(t_info *info, char **cmds);


/* ----------- */
/*  expansion  */
/* ----------- */
// expansion.c
int		expand_variable(void); // tmp


/* ------ */
/*  exit  */
/* ------ */
// exit.c
void	free_alloc(t_info **info);
int		free_and_return_no(t_info **info, int exit_status);
char	**free_array(char **array);
int		perror_and_return_int(char *err, int ret_value);
void	*perror_and_ret_nullptr(char *err);
void	*free_2d_array_ret_nullptr(void ***array);
void	*free_1d_array_ret_nullptr(void **array);
void	*free_1d_2d_array_ret_nullptr(void **array1d, void ***array2d);


/* --------- */
/*  builtin  */
/* --------- */
// builtin.c
int		ft_echo(t_info *info, char **cmds);
int		ft_cd(t_info *info, char **cmds);
int		ft_pwd(t_info *info);
int		ft_export(t_info *info, char **cmds);
int		ft_unset(t_info *info, char **cmds);
int		ft_env(t_info *info, char **cmds);
int		ft_exit(t_info *info, char **cmds);
/* builtin helper.c */
char	*get_current_path(void);


/* -------- */
/*  helper  */
/* -------- */
// tree_node_create.c
char	**splitset_and_trim(char *src, char delim, char set, char *trimchar);
t_tree	*create_tree_node(t_exe_type type, char *raw_cmd_str);

// tree_operation.c
t_tree	*pop_tree_elem_from_top(t_tree **root);
t_tree	*pop_tree_elem_from_bottom(t_tree **root);
void	add_top_of_tree(t_tree **root, t_tree *elem);
void	add_bottom_of_tree(t_tree **root, t_tree *elem);

// tree_helper
t_tree	*get_last_elem(t_tree *elem);
void	tree_clear(t_tree **root);
size_t	get_tree_size(t_tree *root);

// debug print
void	debug_print_stack(t_tree *root, char *str);
void	debug_print_2d_arr(char **arr, char *str);

// env_list
int		add_env_elem_to_list(t_list **list_head, char *key, char *value);
int		overwrite_env_value(t_list **list_head, char *search_key, char *value);
int		delete_env_elem(t_list **list_head, char *search_key);
int		append_env_value(t_list **list_head, char *search_key, char *append_value);
char	*get_env_value(char *search_key, t_list *env_list_head);



#endif
