/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 20:19:08 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/02 20:26:22 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_var(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (!ft_isalpha(str[0]))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '=' && str[i] != '\''
			&& str[i] != '\"' && str[i] != '_')
			return (0);
		str++;
	}
	return (1);
}

void	ft_unsetting(t_mini *mini, int i)
{
	free(mini->env[i]);
	while (mini->env[i + 1])
	{
		mini->env[i] = mini->env[i + 1];
		i++;
	}
	mini->env[i] = NULL;
}

void	ft_unset(t_mini *mini, t_node *node)
{
	int		i;
	int		j;
	int		k;
	char	*var;

	k = 0;
	if (!node->args[1] || !*node->args)
		return ;
	while (node->args[k])
	{
		i = 0;
		j = 0;
		var = ft_name_var(node->args[k]);
		while (mini->env[i])
		{
			if (ft_strncmp(mini->env[i], var,
					ft_strlen(var)) == 0)
			{
				ft_unsetting(mini, i);
				break ;
			}
			i++;
		}
		k++;
	}
}
