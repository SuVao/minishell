/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:29:53 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/02 19:29:56 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_print_echo(t_node *node, int i)
{
	while (node->args[i])
	{
		if (!ft_strncmp(node->args[i], "-n", 3))
		{
			i++;
			continue ;
		}
		printf("%s", node->args[i]);
		i++;
		if (node->args[i])
			printf(" ");
	}
}

int	is_echo(t_node *cur, t_mini *mini)
{
	if (cur->cmd == NULL)
		return (0);
	if (ft_strncmp(cur->cmd, "echo ", 5) == 0)
		ft_echo(cur, mini);
	else
		return (0);
	return (1);
}

void	ft_echo(t_node *node, t_mini *mini)
{
	int	i;
	int	newline;

	(void)*mini;
	i = 1;
	newline = 1;
	if (node->args[i] && !ft_strncmp(node->args[i], "-n", 3))
	{
		newline = 0;
		i++;
	}
	ft_print_echo(node, i);
	if (newline)
		printf("\n");
}
