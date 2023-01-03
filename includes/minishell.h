/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 10:06:25 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <string.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "./../libs/libft/libft.h"
//# include "input.h"
//# include "exit.h"
//# include "execution.h"

typedef struct s_minishell_param	t_info;

typedef struct s_env_elem	t_env_elem;

struct s_env_elem
{
	char *key;
	char *value;
};

t_list	*get_env_list(void);
int		prompt_loop(t_info	*info);

// builtin needed param
//  cd		: current path
//  pwd		: current path
//  export	: env(key, value)
//  unset	: env(key, value)
//	env		: env(key + = + value)
//  exit	: exit status?

struct s_minishell_param
{
	int				exit_status;
	struct	s_list	*env_list;
	char			*input_line; //tmp

};

int		execute(t_info *info);
void	free_alloc(t_info	**info);
int		free_and_return_no(t_info **info, int exit_status);


#endif
