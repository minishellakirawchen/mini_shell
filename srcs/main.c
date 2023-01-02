/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 11:18:07 by wchen             #+#    #+#             */
/*   Updated: 2023/01/02 17:23:51 by wchen            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "./../includes/minishell.h"
# include "./../includes/input.h"

static t_info	*init_minishell_params(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
	{
		perror("malloc");
		return (NULL);
	}
	// init params
	info->exit_status = EXIT_SUCCESS;

	return (info);
}

void	free_alloc(t_info	**info)
{
	if (!info || !*info)
		return ;
	free(*info);
	*info = NULL;
}

int main(void)
{
	t_info	*info;
	char 	*input_line;
	int		exit_status;

	info = init_minishell_params();
	exit_status = EXIT_SUCCESS;

	while (true)
	{
		input_line = readline("minishell >");
		if (!input_line)
			break ;
		// if (input signal == ^C)
		//		print "" and not add history

		// if (strncmp("clear", inpuf_line, ft_strlen("clear")) == 0)
		//		rewrite prompt

		add_history(input_line);

		analysys(input_line, info);
		expantion(info);
		exit_status = excution(info);
		free(input_line);
	}

	free_alloc(&info);
	return (exit_status);
}
