/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:00:04 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 10:01:13 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

# include <readline/readline.h>
# include <readline/history.h>

# include "minishell.h"

typedef struct s_env_elem	t_env_elem;

struct s_env_elem
{
	char *key;
	char *value;
};

t_list	*get_env_list(void);
int		prompt_loop(t_info	*info);

#endif
