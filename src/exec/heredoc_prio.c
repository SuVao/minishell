/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_prio.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:33:03 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/20 21:43:57 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	finish1(t_node *temp, t_node *cur)
{
	temp = cur;
	while (cur->next)
		cur = cur->next;
	cur->next = temp;
	temp->previous = cur;
	temp->next = NULL;
}

void	finish(t_node **sliced_tokens_list, t_node *temp, t_node *cur)
{
	while (cur)
	{
		if (cur->redirs && cur->redirs->type == HEREDOC)
		{
			if (!cur->next)
				return ;
			if (cur->previous)
			{
				cur->previous->next = cur->next;
				if (cur->next)
					cur->next->previous = cur->previous;
			}
			else
			{
				*sliced_tokens_list = cur->next;
				if (cur->next)
					cur->next->previous = NULL;
			}
			finish1(temp, cur);
			break ;
		}
		cur = cur->next;
	}
}

void	heredoc_last(t_node **sliced_tokens_list)
{
	t_node	*cur;
	t_node	*temp;

	temp = NULL;
	cur = *sliced_tokens_list;
	if (!cur->next)
		return ;
	while (cur)
	{
		if (cur->redirs && cur->redirs->type == HEREDOC)
			cur = cur->next;
		else
			break ;
	}
	if (!cur)
		return ;
	cur = *sliced_tokens_list;
	while (cur->next)
	{
		cur = cur->next;
	}
	if (cur->outfile == 1)
		return ;
	cur = *sliced_tokens_list;
	finish(sliced_tokens_list, temp, cur);
}
