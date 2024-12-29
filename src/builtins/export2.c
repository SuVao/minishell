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

int	ft_count_lines(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char **ft_split_vars(t_node *node)
{
	char	**tmp;
	int		k;
	int		i;

	tmp = NULL;
	k = 1;
	i = 0;
	tmp = malloc(sizeof(char *) * (ft_count_lines(node->args) + 1));
	while (node->args[k])
	{
		if (is_var(node->args[k]))
		{
			if (check_var(node->args[k]))
			{
				tmp[i] = ft_strdup(node->args[k]);
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

char *ft_name_var(char *var)
{
	int i;
	char *name_var;

	name_var = NULL;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	name_var = malloc(i + 2);
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

char **exportar_variaveis(char **tmp_env, char *var, t_mini *mini)
{
	int i;
	char *name_var;

	(void)mini;
	name_var = NULL;
	name_var = ft_name_var(var);
	if (!name_var)
		return (NULL);
	i = 0;
	while (tmp_env[i] && ft_strncmp(tmp_env[i], name_var, ft_strlen(name_var)) != 0)
		i++;
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
	free(name_var);
	return (tmp_env);
}

void	ft_export(t_node *node, t_mini *mini)
{
	char	**tmp_env;
	char	**valid_vars;
	int i;
	int j;

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
	tmp_env = copy_tmp(ft_count_lines(valid_vars), \
			ft_count_lines(mini->env), mini);
	if (!tmp_env)
	{
		free_args(valid_vars);
		return ;
	}
	while (valid_vars[i])
	{
		tmp_env = exportar_variaveis(tmp_env, valid_vars[i], mini);
		if (mini->env)
			ft_free_env(mini->env);
		mini->env = tmp_env;
		i++;
	}
	free_args(valid_vars);
}
