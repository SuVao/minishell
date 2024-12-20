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
		if (tmp_env1)
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

int	ft_count_args(char **args)
{
	int	i;
	int	count;

	count = 0;
	i = 1;
	while (args[i])
	{
		if (is_var(args[i]) && check_var(args[i]))
			count++;
		i++;
	}
	return (i);
}

char	**ft_split_vars(t_node *node)
{
	char	**tmp;
	int		k;
	int		i;

	tmp = NULL;
	k = 1;
	i = 0;
	tmp = malloc(sizeof(char *) * (ft_count_args(node->args) + 1));
	while (node->args[k])
	{
		if (is_var(node->args[k]))
		{
			if (check_var(node->args[k]))
			{
				tmp[i] = ft_strdup(node->args[k]);
				printf("node->args[%d]: %s\n", i, node->args[k]);
				if (!tmp[i])
					return (NULL);
				i++;
			}
		}
		else
			printf("export: `%s': not a valid identifier\n", node->args[k]);
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
	int 	j;
	int		count_lines;
	char	**valid_vars;
	count_lines = 0;
	i = 0;
	j = 0;
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
	while (mini->env[j])
		j++;
	while (valid_vars[i])
	{
		if (already_exist(valid_vars[i], mini) == 0)
		{
			temp = ft_substr(ft_memchr(valid_vars[i], '=', ft_strlen(valid_vars[i])) + 1, 0,\
			 ft_strlen(ft_memchr(valid_vars[i], '=', ft_strlen(valid_vars[i])) + 1));
			tmp_env[j] = ft_cpyline(tmp_env[j], NULL, temp, valid_vars);
			free(temp);
		}
		else
			ft_replace_var(tmp_env, valid_vars[i], valid_vars[i]);
		i++;
	}
	free_args(valid_vars);
	tmp_env[++i] = NULL;
	ft_free_env(mini->env);
	mini->env = tmp_env;

	//count_lines = checking_if_already_exits(node, mini);
	//i = 0;
	//while (mini->env[i] != NULL)
	//	i++;
	//tmp_env = copy_tmp(count_lines, i, mini);
	//tmp_env = ft_cenas_com_vars(node, tmp_env, i, mini);
	//if (!tmp_env)
	//	return ;
}
