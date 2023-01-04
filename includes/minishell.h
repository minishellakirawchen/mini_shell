/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/04 16:49:41 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <errno.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "./../libs/libft/libft.h"
#include "./../libs/libftprintf/ft_printf.h"

#define BUFSIZE	1024

/* typedef struct */
typedef struct s_minishell_param	t_info;
typedef struct s_env_elem			t_env_elem;
typedef struct s_execute_cmds_info	t_cmds;

/* typedef enum */
typedef enum e_cmd_group	t_group;

/* enum */
enum e_cmd_group
{
	E_SHELL,
	E_PIPE,
	E_SUBSHELL,
	E_AND,
	E_OR
};

/* struct */
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
	char	**input_line; // tmp
	char	**commands;   // tmp
	char 	*pwd;
	t_list	*execute_cmds;
};

/* input */
t_list	*get_env_list(void);
int		prompt_loop(t_info *info);

/* analysis */
int		analysis(void); // tmp

/* execution */
int		execute(t_info *info);

/* expansion */
int		expand_variable(void); // tmp

/* exit */
void	free_alloc(t_info **info);
int		free_and_return_no(t_info **info, int exit_status);

/* builtin */
// return exit_status
int		ft_echo(t_info *info);
int		ft_cd(t_info *info);
int		ft_pwd(t_info *info);
int		ft_export(t_info *info);
int		ft_unset(t_info *info);
int		ft_env(t_info *info);
int		ft_exit(t_info *info);
// builtin helper
char	*get_current_path(void);

#endif
