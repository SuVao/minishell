/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 20:19:17 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/02 20:20:34 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*path_find(char **envp, char *cmd)
{
	int		i;
	char	**possible_ways;
	char	*path;

	if (!envp || !cmd)
		return (NULL);
	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	possible_ways = ft_split(envp[i] + 19, ':');
	i = 0;
	while (possible_ways[i])
	{
		path = ft_append_str(possible_ways[i], "/", cmd);
		if (access(path, F_OK) == 0)
			break ;
		else
		{
			free(path);
			path = NULL;
			i++;
		}
	}
	free_args(possible_ways);
	return (path);
}

void	ft_free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	ft_pwd(t_mini *mini)
{
	int i = 0;
	char *pwd;

	while (mini->env[i] && ft_strncmp(mini->env[i], "PWD=", 4))
		i++;
	pwd = ft_substr(mini->env[i], 4, ft_strlen(mini->env[i]) - 4);
	printf("%s\n", pwd);
	free(pwd);
}

int	is_builtins(t_node *node, t_mini *mini, int *status)
{
	if (node->cmd == NULL)
		return (0);
	else if (ft_strncmp(node->cmd, "cd", 2) == 0)
		ft_cd(node, mini, status);
	else if (ft_strncmp(node->cmd, "pwd", 4) == 0)
		ft_pwd(mini);
	else if (ft_strcmp(node->cmd, "export") == 0)
		ft_export(node, mini);
	else if (ft_strcmp(node->cmd, "unset") == 0)
		ft_unset(mini, node);
	else if (ft_strcmp(node->cmd, "env") == 0)
		ft_env(mini);
	else if (ft_strcmp(node->cmd, "exit") == 0)
		ft_exit(mini);
	else
		return (0);
	return (1);
}
