/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 18:58:18 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/04 19:01:28 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_env_back_cd(char *new_pwd, char *pwd, t_mini *mini)
{
	int	i;

	i = 0;
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", new_pwd);
		i++;
		break ;
	}
}

char	*get_var_env(char **env, char *to_find)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], to_find, ft_strlen(to_find)) == 0)
			return (ft_strdup(ft_strchr(env[i], '/')));
		i++;
	}
	return (NULL);
}

void	init_envp(t_mini *mini, char **envp)
{
	int		i;
	char	*tmp;

	i = 0;
	while (envp[i])
		i++;
	mini->env = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "SHLVL=", 6))
		{
			mini->shlvl = ft_atoi(&envp[i][6]);
			mini->shlvl++;
			tmp = ft_itoa(mini->shlvl);
			mini->env[i] = ft_strjoin("SHLVL=", tmp);
			free(tmp);
			i++;
		}
		mini->env[i] = ft_strdup(envp[i]);
		i++;
	}
	mini->env[i] = NULL;
}
