/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:28:00 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/28 18:02:51 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_redirections(t_redir *redir)
{
	t_redir	*temp;

	while (redir)
	{
		temp = redir;
		redir = redir->next;
		free(temp);
		temp = NULL;
	}
}

void	free_tokens(t_node *cur)
{
	int	i;

	i = 0;
	while (cur->tokens[i])
	{
		free(cur->tokens[i]);
		cur->tokens[i] = NULL;
		i++;
	}
	free(cur->tokens);
	cur->tokens = NULL;
}

void	free_args2(t_node *cur)
{
	int	i;

	i = 0;
	while (cur->args[i] != NULL)
	{
		free(cur->args[i]);
		cur->args[i] = NULL;
		i++;
	}
	free(cur->args);
	cur->args = NULL;
}

void	free_ast(t_node **node)
{
	t_node	*cur;
	t_node	*temp;

	cur = *node;
	if (cur)
	{
		while (cur)
		{
			temp = cur->next;
			if (cur->tokens)
				free_tokens(cur);
			if (cur->args)
				free_args2(cur);
			free_redirections(cur->redirs);
			free(cur);
			cur = temp;
		}
	}
}
