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

int	checking_if_already_exits(t_node *node, t_mini *mini)
{
	char	**tmp;
	int		k;
	int		count_rep;

	k = 0;
	count_rep = 0;
	while (node->args[k])
	{
		tmp = ft_split(node->args[k], '=');
		if (!tmp)
			return (0);
		if (already_exist(tmp[0], mini) == 1)
			count_rep++;
		free_args(tmp);
		k++;
	}
	return (((k - 1) - count_rep) + 2);
}

char	**copy_tmp(int count_lines, int i, t_mini *mini)
{
	int		j;
	char	**tmp_env;

	tmp_env = (char **)malloc((i + count_lines) * sizeof(char *));
	if (!tmp_env)
		return (NULL);
	j = 0;
	while (j < i)
	{
		tmp_env[j] = ft_strdup(mini->env[j]);
		if (!tmp_env[j])
		{
			ft_free_env(tmp_env);
			return (NULL);
		}
		j++;
	}
	tmp_env[j] = NULL;
	return (tmp_env);
}

char	*ft_cpyline(char *tmp_env, char **tmp_env1, char *args, char **tmp)
{
	tmp_env = ft_strdup(args);
	if (!tmp_env)
	{
		ft_free_env(tmp_env1);
		free_args(tmp);
		return (NULL);
	}
	return (tmp_env);
}

char	**ft_cenas_com_vars(t_node *node, char **tmp_env, int i, t_mini *mini)
{
	char	**tmp;
	int		k;

	k = 1;
	while (node->args[k])
	{
		if (is_var(node->args[k]) == 0)
			k++;
		tmp = ft_split(node->args[k], '=');
		if (!tmp)
			return (NULL);
		if (already_exist(tmp[0], mini) == 1)
			ft_replace_var(tmp_env, tmp[0], node->args[k]);
		else
		{
			tmp_env[i] = ft_cpyline(tmp_env[i], tmp_env, node->args[k], tmp);
			i++;
		}
		free_args(tmp);
		k++;
	}
	return (tmp_env);
}

char	**ft_split_vars(t_node *node)
{
	char	**tmp;
	int		k;
	int		i;

	tmp = NULL;
	k = 1;
	i = 0;
	while (node->args[k])
	{
		if (check_var(node->args[k]) && is_var(node->args[k]))
		{
			tmp[i] = ft_strdup(node->args[k]);
			if (!tmp[i])
				return (NULL);
			i++;
		}
		else
		{
			printf("export: `%s': not a valid identifier\n", node->args[k]);
		}
		k++;
	}
	tmp[i] = NULL;
	return (tmp);
}

void	ft_print_export(t_mini *mini)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (mini->env[i] != NULL)
	{
		if (ft_strncmp(mini->env[i], "_=", 2) != 0)
		{
			j = 0;
			while (mini->env[i][j] != '=')
				j++;
			write(1, "declare -x ", 11);
			write(1, mini->env[i], j + 1);
			write(1, "\"", 1);
			write(1, mini->env[i] + j + 1, ft_strlen(mini->env[i] + j + 1));
			write(1, "\"\n", 2);
		}
		i++;
	}
}

void	ft_export(t_node *node, t_mini *mini)
{
	char	**tmp_env;
	char	*new_var;
	char	*temp;
	int		i;
	int		count_lines;
	char	**valid_vars;
	count_lines = 0;
	i = 0;
	new_var = NULL;
	temp = NULL;
	tmp_env = NULL;

	if (node->args[1] == NULL)
	{
		ft_print_export(mini);
		return ;
	}
	valid_vars = ft_split_vars(node);
	if (valid_vars == NULL)
		return ;
	// while (valid_vars[i])
	// {
	// 	if (check_var(valid_vars[i]) == 0)
	// 		return ;
	// 	i++;
	// }
	printf("ola\n");
	count_lines = checking_if_already_exits(node, mini);
	i = 0;
	while (mini->env[i] != NULL)
		i++;
	tmp_env = copy_tmp(count_lines, i, mini);
	tmp_env = ft_cenas_com_vars(node, tmp_env, i, mini);
	if (!tmp_env)
		return ;
	tmp_env[++i] = NULL;
	ft_free_env(mini->env);
	mini->env = tmp_env;
}
