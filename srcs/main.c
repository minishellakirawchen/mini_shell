/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wchen <wchen@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 11:18:07 by wchen             #+#    #+#             */
/*   Updated: 2023/01/04 21:10:11 by takira           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static t_info	*init_minishell_params(void)
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
	return (info);
}

int main(void)
{
	t_info	*info;
	int		exit_status;

	info = init_minishell_params();
	exit_status = prompt_loop(info);
	free_alloc(&info);
 	return (exit_status);
}
