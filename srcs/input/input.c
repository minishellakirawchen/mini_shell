/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/07 08:52:33 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompt_loop(t_info	*info)
{
	int		exit_status;
	char	*input_line;

	while (true)
	{
		input_line = readline("minishell $> ");
		if (!input_line)
			break ;

		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		// if (input signal == ^C)
		//		print "^C" and not add history
		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		/* Maybe unnecessary, "clear" clear prompt on default */
		// if (strncmp("clear", inpuf_line, ft_strlen("clear")) == 0)
		//		rewrite prompt
		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */
		/*  for demo  */
		// "" '' must not split -> ex)echo "hello world" -> analysis
//		info->input_line = ft_split_set(input_line, ' ', '\'');
		info->commands = ft_split_set(input_line, ' ', '\"'); // for demo
		/*  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ */

//		info->input_line = ft_split_set(input_line, ' ', '"'); //temp, for feature, update group flag set; char set -> char *set={'"} ()も
//		debug_print_2d_arr(info->commands, "commands");
		info->input_line = input_line;

		analysis(info);
		expand_variable();
		exit_status = execute_command_line(info);

		add_history(input_line);//where?
		free(input_line);
//		free input and commands in info
	}
	return (exit_status);
}
