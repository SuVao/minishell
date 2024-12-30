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

	tmp_env = (char **)malloc((i + count_lines + 1) * sizeof(char *));
	if (!tmp_env)
		return (NULL);
	ft_bzero(tmp_env, sizeof(tmp_env));
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

//primeiro verificar se é uma variavel valida
//depois verificar se ja existe
//se ja existir substituir
//se nao existir adicionar

//variavel valida = tem que ter um = e nao pode ter caracteres especiais
//variavel valida = tem que comecar por uma letra e nao pode ser numeros
//ja existe = verificar se a variavel ja existe no env

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
