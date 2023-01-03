/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:46:12 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 09:23:55 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H

#include "./../includes/minishell.h"

void	free_alloc(t_info	**info);
int		free_and_return_no(t_info **info, int exit_status);

#endif
