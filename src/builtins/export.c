/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:31:05 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/02 20:27:46 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_replace_var(char **env, char *tofound, char *to_replace)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		if (ft_strncmp(env[i], tofound, ft_strlen(tofound)) == 0
			&& env[i][ft_strlen(tofound)] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(to_replace);
			if (!env[i])
				return ;
			break ;
		}
		i++;
	}
}

int	is_var(char *s)
{
	int	i;

	i = 0;
	if (!s || s[0] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] == '=' && i != 0)
			return (1);
		i++;
	}
	return (0);
}

int	already_exist(char *arg, t_mini *mini)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strjoin(arg, "=");
	if (!tmp)
		return (0);
	while (mini->env[i] != NULL)
	{
		if (ft_strncmp(mini->env[i], tmp, ft_strlen(tmp)) == 0)
		{
			free(tmp);
			return (1);
		}
		i++;
	}
	free(tmp);
	return (0);
}

void	ft_zero_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_bzero(env[i], ft_strlen(env[i]));
		i++;
	}
}

int	var_accept(t_node *node)
{
	int	i;

	i = 0;
	while (node->args[1][i])
	{
		if (node->args[1][i] == '=')
			return (1);
		i++;
	}
	return (0);
}
