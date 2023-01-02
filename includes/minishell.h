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

typedef struct s_minishell_param	t_info;

struct s_minishell_param
{
	int	exit_status;
};


#endif
