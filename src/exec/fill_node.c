/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:01:36 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/20 17:40:55 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	get_redir_type(char *token)
{
	if (!ft_strcmp(token, ">"))
		return (REDIR_OUT);
	else if (!ft_strcmp(token, ">>"))
		return (REDIR_APPEND);
	else if (!ft_strcmp(token, "<"))
		return (REDIR_IN);
	else if (!ft_strcmp(token, "<<"))
		return (HEREDOC);
	return (-1);
}

void	find_cmd(t_node *cur, int *redir_type, int *cmd_index)
{
	int	i;

	i = 0;
	while (cur->tokens[i] != NULL)
	{
		*redir_type = get_redir_type(cur->tokens[i]);
		if (*redir_type != -1)
			i += 2;
		else
		{
			*cmd_index = i;
			return ;
		}
	}
}

void	find_other_redirs(t_node *cur, int *redir_type)
{
	int	i;

	i = 0;
	while (cur->tokens[i] != NULL)
	{
		*redir_type = get_redir_type(cur->tokens[i]);
		if (*redir_type != -1 && *redir_type != HEREDOC)
		{
			if (cur->tokens[i + 1] == NULL)
				perror("Syntax error: missing file for redirection");
			add_redirection(cur, *redir_type, cur->tokens[i + 1]);
			i += 2;
		}
		else
			i++;
	}
}

void	find_heredoc(t_node *cur, int *redir_type)
{
	int	i;

	i = 0;
	while (cur->tokens[i] != NULL)
	{
		*redir_type = get_redir_type(cur->tokens[i]);
		if (*redir_type == HEREDOC)
		{
			if (cur->tokens[i + 1] == NULL)
				perror("Syntax error: missing file for redirection");
			add_redirection(cur, HEREDOC, cur->tokens[i + 1]);
			i += 2;
		}
		else
			i++;
	}
}

void	fill_node(t_node **sliced_tokens_list, t_mini *mini)
{
	t_node	*cur;
	int		redir_type;
	int		cmd_index;

	cur = *sliced_tokens_list;
	while (cur)
	{
		cmd_index = -1;
		find_heredoc(cur, &redir_type);
		find_other_redirs(cur, &redir_type);
		find_cmd(cur, &redir_type, &cmd_index);
		if (cmd_index != -1)
		{
			cur->cmd = cur->tokens[cmd_index];
			cur->args = filter(slice_tokens(cur->tokens, cmd_index, -1));
			expand_args(cur->args, mini->env, mini);
		}
		cur = cur->next;
		mini->nr_pipes++;
	}
}
