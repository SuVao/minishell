/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 20:22:38 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/04 19:02:05 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd_go_back(char *pwd, t_mini *mini, int status)
{
	char	*new_pwd;

	new_pwd = encontra_barra(pwd);
	if (!new_pwd || chdir(new_pwd) == -1)
	{
		status = 256;
		perror("Error");
	}
	else
	{
		status = 0;
		update_env_back_cd(new_pwd, pwd, mini);
	}
	free(new_pwd);
}

void	ft_cd_no_args(char *home, char *pwd, t_mini *mini, int status)
{
	if (!home)
		home = ft_encontra_o_home();
	if (chdir(home) == -1)
	{
		status = 256;
		free(home);
		perror("Error");
	}
	else
	{
		status = 0;
		update_env_abs(pwd, home, mini);
	}
}
