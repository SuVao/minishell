/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 20:23:04 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/04 19:01:13 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_count_lines(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	checks_vars(t_node *node, char **tmp)
{
	int	i;
	int	k;

	k = 1;
	i = 0;
	while (node->args[k])
	{
		if (is_var(node->args[k]))
		{
			if (check_var(node->args[k]))
			{
				tmp[i] = ft_strdup(node->args[k]);
				if (!tmp[i])
					return ;
				i++;
			}
		}
		else
			printf("export: `%s': not a valid identifier\n", node->args[k]);
		k++;
	}
	tmp[i] = NULL;
}

char	**ft_split_vars(t_node *node)
{
	char	**tmp;

	tmp = NULL;
	tmp = malloc(sizeof(char *) * (ft_count_lines(node->args) + 1));
	checks_vars(node, tmp);
	return (tmp);
}

char	**ft_find_it(char **tmp_env, int i, char *var, char *name_var)
{
	if (tmp_env[i] == NULL)
	{
		tmp_env[i] = ft_strdup(var);
		if (!tmp_env[i])
		{
			free(name_var);
			return (NULL);
		}
	}
	else if (ft_strncmp(tmp_env[i], name_var, ft_strlen(name_var)) == 0)
	{
		free(tmp_env[i]);
		tmp_env[i] = ft_strdup(var);
		if (!tmp_env[i])
		{
			free(name_var);
			return (NULL);
		}
	}
	tmp_env[++i] = NULL;
	return (tmp_env);
}

char	**exportar_variaveis(char **tmp_env, char *var)
{
	int		i;
	char	*name_var;

	name_var = ft_name_var(var);
	if (!name_var)
		return (NULL);
	i = 0;
	while (tmp_env[i] && ft_strncmp(tmp_env[i], name_var, \
			ft_strlen(name_var)) != 0)
		i++;
	tmp_env = ft_find_it(tmp_env, i, var, name_var);
	free(name_var);
	return (tmp_env);
}
