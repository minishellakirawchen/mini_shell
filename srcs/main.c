/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 11:18:07 by wchen             #+#    #+#             */
/*   Updated: 2023/01/11 15:15:20 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static t_info	*init_params(void)
{
	t_info	*info;

	info = (t_info *)malloc(sizeof(t_info));
	if (!info)
	{
		perror("malloc");
		free_and_return_no(NULL, EXIT_FAILURE);
	}
	// init params
	info->exit_status = EXIT_SUCCESS;
	info->env_list = get_env_list();//if env_list==NULL -> operate with env=NULL
	info->tree_root = NULL;
	info->pid = getpid();
	return (info);
}

static void	display_ascii_art(void)
{
	ft_printf("           _       _      _          _ _ \n"
			  " _ __ ___ (_)_ __ (_)___ | |__   ___| | |\n"
			  "| '_ ` _ \\| | '_ \\| / __|| '_ \\ / _ \\ | |\n"
			  "| | | | | | | | | | \\__ \\| | | |  __/ | |\n"
			  "|_| |_| |_|_|_| |_|_|___/|_| |_|\\___|_|_|\n"
			  "                        By: takira, wchen\n");
}

int main(void)
{
	t_info	*info;
	int		exit_status;

	display_ascii_art();
	info = init_params();
	exit_status = prompt_loop(info);
	free_info(&info);
 	return (exit_status);
}
