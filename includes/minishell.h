/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:08 by takira            #+#    #+#             */
/*   Updated: 2023/01/02 21:00:09 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <string.h>

#include "./../libs/libft/libft.h"

typedef struct s_minishell_param	t_info;

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
	char			*input_line;


};

#endif
