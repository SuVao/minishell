/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:30:23 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/04 19:00:37 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_encontra_o_home(void)
{
	int		i;
	int		k;
	char	*home;
	char	*new_home;

	i = 0;
	home = NULL;
	new_home = getcwd(NULL, 0);
	if (!new_home)
		return (NULL);
	k = 0;
	while (new_home[i])
	{
		if (new_home[i] == '/')
			k++;
		if (new_home[i + 1] == '/' && k == 1)
			break ;
		i++;
	}
	k = 0;
	while (new_home[k] != '/')
		k++;
	home = ft_substr(new_home, k, i + 1);
	return (home);
}

static void	ft_fill_env(t_mini *mini, char *home, char *pwd)
{
	mini->env[0] = ft_strjoin("HOME=", home);
	mini->env[1] = ft_strjoin("PWD=", pwd);
	mini->env[2] = ft_strjoin("SHLVL=", ft_itoa(mini->shlvl));
	mini->env[3] = ft_strjoin("OLDPWD=", pwd);
	mini->env[4] = NULL;
	if (!mini->env[0] || !mini->env[1] || !mini->env[2] || !mini->env[3])
	{
		free_args(mini->env);
		exit(EXIT_FAILURE);
	}
}

void	init_myown_envp(t_mini *mini)
{
	char	*pwd;
	char	*home;

	mini->env = (char **)malloc(sizeof(char *) * 5);
	mini->shlvl = 1;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		free(pwd);
		printf("Error: getcwd failed\n");
		return ;
	}
	home = ft_encontra_o_home();
	ft_fill_env(mini, home, pwd);
	free(pwd);
}
