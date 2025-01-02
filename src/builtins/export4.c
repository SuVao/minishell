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

void	tmps_env(char **valid_vars, t_mini *mini, char **tmp_env)
{
	int	i;

	i = 0;
	tmp_env = copy_tmp(ft_count_lines(valid_vars), \
			ft_count_lines(mini->env), mini);
	if (!tmp_env)
	{
		free_args(valid_vars);
		return ;
	}
	while (valid_vars[i])
	{
		tmp_env = exportar_variaveis(tmp_env, valid_vars[i]);
		i++;
	}
	if (mini->env)
		ft_free_env(mini->env);
	mini->env = tmp_env;
}

void	ft_export(t_node *node, t_mini *mini)
{
	char	**tmp_env;
	char	**valid_vars;
	int		i;
	int		j;

	i = 0;
	j = 0;
	tmp_env = NULL;
	valid_vars = NULL;
	if (node->args[1] == NULL)
	{
		ft_print_export(mini);
		return ;
	}
	valid_vars = ft_split_vars(node);
	if (!valid_vars)
		return ;
	tmps_env(valid_vars, mini, tmp_env);
	free_args(valid_vars);
}

char	*ft_name_var(char *var)
{
	int		i;
	char	*name_var;

	name_var = NULL;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	name_var = malloc(i + 2);
	if (!name_var)
		return (NULL);
	i = 0;
	while (var[i] && var[i] != '=')
	{
		name_var[i] = var[i];
		i++;
	}
	name_var[i++] = '=';
	name_var[i] = '\0';
	return (name_var);
}
