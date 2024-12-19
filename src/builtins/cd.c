/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:28:12 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/04 19:30:27 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_cd_with_minus(char *pwd, t_mini *mini, int status)
{
	char	*s;

	s = get_var_env(mini->env, "OLDPWD=");
	if (chdir(s) == -1)
	{
		status = 256;
		perror("Error");
	}
	else
	{
		status = 0;
		update_env(s, pwd, mini);
	}
	free(s);
}

char	*ft_new_pwd(char *new_pwd, t_node *node, char *home, char *pwd)
{
	if (node->args[1][0] == '/')
		new_pwd = ft_strdup(node->args[1]);
	else
		new_pwd = ft_append_str(pwd, "/", node->args[1]);
	if (!new_pwd)
	{
		write(2, "Error: malloc failed\n", 22);
		free(pwd);
		free(home);
		return (NULL);
	}
	return (new_pwd);
}

void	ft_cd_with_dash(t_node *node, t_mini *mini, char *home, int *status)
{
	char	*new_pwd;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	new_pwd = NULL;
	if (!pwd)
	{
		write(2, "Error: getcwd failed\n", 22);
		free(home);
		return ;
	}
	new_pwd = ft_new_pwd(new_pwd, node, home, pwd);
	if (chdir(new_pwd) == -1)
	{
		*status = 256;
		perror("Error");
	}
	else
	{
		*status = 0;
		update_env(new_pwd, pwd, mini);
	}
	free(pwd);
	free(new_pwd);
}

void	ft_cd(t_node *node, t_mini *mini, int *status)
{
	char	*home;
	char	*pwd;

	home = get_var_env(mini->env, "HOME=");
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		write(2, "Error: getcwd failed\n", 22);
		free(home);
		return ;
	}
	if (!node->args[1])
		ft_cd_no_args(home, pwd, mini, *status);
	else if (ft_strncmp(node->args[1], ".. ", 2) == 0)
		ft_cd_go_back(pwd, mini, *status);
	else if (ft_strncmp(node->args[1], "-", 1) == 0)
		ft_cd_with_minus(pwd, mini, *status);
	else
		ft_cd_with_dash(node, mini, home, status);
	main_pid()->status = *status;
	free(pwd);
	free(home);
}
