/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 13:49:57 by takira           ###   ########.fr       */
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

# define SUCCESS	0
# define	FAILURE	1

# define READ	0
# define WRITE	1

# define CHDIR_FAILURE				1
# define CMD_NOT_FOUND				127
# define EXIT_TOO_MANY_ARGS			1
# define EXIT_NUMERIC_ARGS_REQUIRED	255

# define	PATH			"PATH"
# define	PWD				"PWD"
# define 	PATH_DELIMITER	':'
# define	ISSPACE			"\t\n\v\f\r "


/* ---------------- */
/*  typedef struct  */
/* ---------------- */
typedef struct s_minishell_param	t_info;
typedef struct s_env_elem			t_env_elem;
typedef struct s_execute_cmds_info	t_cmds;
typedef struct s_exe_elem			t_exe_elem;
typedef struct s_tree				t_tree;

/* -------------- */
/*  typedef enum  */
/* -------------- */
typedef enum e_cmd_group	t_group;
typedef enum e_export_type	t_export_type;

/* -------*/
/*  enum  */
/* ------ */
enum e_cmd_group
{
	E_ROOT,
	E_PIPE,
	E_CMD,
	E_SHELL,
	E_SUBSHELL,
	E_AND,
	E_OR,
};

enum e_export_type
{
	E_NEW,
	E_ADD,
};

/* -------- */
/*  struct  */
/* -------- */
struct s_env_elem
{
	char	*key;
	char	*value;
};

struct s_execute_cmds_info
{
	t_group	type;
	char	**cmds;
};

struct s_exe_elem
{
	t_group exe_type;
	char 	**cmds;
};

struct s_tree
{
	void			*content;
	struct s_tree	*left;
	struct s_tree	*right;
};

// builtin needed param
//  cd		: current path
//  pwd		: current path
//  export	: env(key, value)
//  unset	: env(key, value)
//	env		: env(key + = + value)
//  exit	: exit status?

struct s_minishell_param
{
	int		exit_status;
	t_list	*env_list;
	char	*input_line;
	char	**commands;   // for demo

	t_tree	*exe_root;

//	t_tree	*exe_stack;
//	t_list	*exe_root;
};

/* ------- */
/*  input  */
/* ------- */
/* input.c */
int		prompt_loop(t_info *info);
t_list	*get_env_list(void);
char	*get_env_value(char *search_key, t_list *env_list_head);
int		add_env_elem_to_list(t_list **list_head, char *key, char *value);
int		overwrite_env_value(t_list **list_head, char *search_key, char *value);
int		delete_env_elem(t_list **list_head, char *search_key);

/* ---------- */
/*  analysis  */
/* ---------- */
/* analysis.c */
int		analysis(t_info *info); // tmp

/* ----------- */
/*  execution  */
/* ----------- */
/* execute.c */
int		execute_command_line(t_info *info);

/* execute_builtin */
bool	is_builtins(char *cmd_head);
int		execute_builtins(t_info *info, char **cmds);


/* ----------- */
/*  expansion  */
/* ----------- */
/* expansion.c */
int		expand_variable(void); // tmp

/* ------ */
/*  exit  */
/* ------ */
/* exit.c */
void	free_alloc(t_info **info);
int		free_and_return_no(t_info **info, int exit_status);
char	**free_array(char **array);
int		perror_and_return_int(char *err, int ret_value);

/* --------- */
/*  builtin  */
/* --------- */
/* builtin.c */
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
/* tree_operation.c */
t_tree	*pop_tree_elem_from_top(t_tree **root);
t_tree	*pop_tree_elem_from_bottom(t_tree **root);
void	add_top_of_tree(t_tree **root, t_tree *elem);
void	add_bottom_of_tree(t_tree **root, t_tree *elem);

/* tree_helper */
t_tree	*create_tree_elem(void *content);
t_tree	*get_last_elem(t_tree *elem);
void	tree_clear(t_tree **root);
size_t	get_tree_size(t_tree *root);

/* debug print */
void	debug_print_stack(t_tree *root, char *str);
void	debug_print_2d_arr(char **arr, char *str);

/* env_list */
int		add_env_elem_to_list(t_list **list_head, char *key, char *value);
int		overwrite_env_value(t_list **list_head, char *search_key, char *value);
int		delete_env_elem(t_list **list_head, char *search_key);
int		append_env_value(t_list **list_head, char *search_key, char *append_value);
char	*get_env_value(char *search_key, t_list *env_list_head);



#endif
