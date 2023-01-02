/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takira <takira@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:04 by takira            #+#    #+#             */
/*   Updated: 2023/01/02 21:00:04 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# include "./../libs/libft/libft.h"

# include <readline/readline.h>
# include <readline/history.h>

t_list	*get_env_list(void);
int		prompt_loop(t_info	*info);

#endif
