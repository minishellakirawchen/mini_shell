/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:46:12 by takira            #+#    #+#             */
/*   Updated: 2023/01/02 21:46:13 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H

#include "./../includes/minishell.h"
#include "./../includes/input.h"

void	free_alloc(t_info	**info);
int		free_and_return_no(t_info **info, int exit_status);

#endif
