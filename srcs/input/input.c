/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 21:49:54 by takira            #+#    #+#             */
/*   Updated: 2023/01/03 09:28:31 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minishell.h"
#include "./../includes/excution.h"
#include "./../includes/input.h"

int	prompt_loop(t_info	*info)
{
	int		exit_status;
	char	*input_line;

	while (true)
	{
		input_line = readline("minishell >");
		if (!input_line)
			break ;
		// if (input signal == ^C)
		//		print "" and not add history

		// if (strncmp("clear", inpuf_line, ft_strlen("clear")) == 0)
		//		rewrite prompt
		info->input_line = input_line;// tmp for execute_test

		add_history(input_line);
//		analysys(input_line, info);
//		expantion(info);
		exit_status = execute(info);
		free(input_line);
	}
	return (exit_status);
}
