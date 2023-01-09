/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/09 19:23:02 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_input(t_info **info)
{
	if (!info || !*info)
		return ;
	free_2d_array_ret_nullptr((void ***)&(*info)->splitted_cmds);
	tree_clear(&(*info)->tree_root);
	(*info)->splitted_cmds = NULL;
	(*info)->tree_root = NULL;
}

int	prompt_loop(t_info	*info)
{
	int		exit_status;
	char	*input_line;
	char 	**space_splitted_line;

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

//		info->input_line = ft_split_set(input_line, ' ', '"'); //temp, for feature, update group flag set; char set -> char *set={'"} ()も
//		debug_print_2d_arr(info->commands, "commands");
		space_splitted_line = ft_split_set(input_line, ' ', '"');
		if (!space_splitted_line)
		{
			free(input_line);
			return (perror_and_return_int("malloc", EXIT_FAILURE));
		}
		info->splitted_cmds = space_splitted_line;

		analysis(info);
		expand_variable();
		exit_status = execute_command_line(info);
		add_history(input_line);//where?
		init_input(&info);
		free(input_line);
	}
	return (exit_status);
}
