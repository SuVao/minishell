/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:01:00 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/20 16:01:08 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_free(char *list[])
{
	int	i;

	if (!*list)
		return ;
	i = 0;
	while (list[i] != NULL)
	{
		free(list[i]);
		i++;
	}
	free(list);
}

char	*get_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	t_getpath	path;

	if (!cmd || !*cmd)
		return (NULL);
	path.env_path = get_env(env);
	if (!path.env_path)
		return (NULL);
	path.allpath = ft_split(path.env_path, ':');
	if (!path.allpath)
		return (NULL);
	path.i = 0;
	while (path.allpath[path.i])
	{
		path.path_part = ft_strjoin(path.allpath[path.i++], "/");
		path.exec = ft_strjoin(path.path_part, cmd);
		free(path.path_part);
		if (access(path.exec, F_OK | X_OK) == 0)
		{
			ft_free(path.allpath);
			return (path.exec);
		}
		free(path.exec);
	}
	ft_free(path.allpath);
	return (NULL);
}
