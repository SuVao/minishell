/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:01:50 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/04 19:11:18 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_redir	*new_redirection(int type, char *target)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = target;
	redir->next = NULL;
	return (redir);
}

void	add_redirection(t_node *node, int type, char *target)
{
	t_redir	*redir;
	t_redir	*temp;

	temp = NULL;
	redir = new_redirection(type, target);
	if (!redir)
		return ;
	if (!node->redirs)
		node->redirs = redir;
	else
	{
		temp = node->redirs;
		while (temp->next)
			temp = temp->next;
		temp->next = redir;
	}
}

char	**filter(char **tokens)
{
	int	i;
	int	count;
	int	num;

	i = 0;
	count = 0;
	while (tokens[i])
	{
		num = get_redir_type(tokens[i]);
		if (num != -1)
		{
			free(tokens[i]);
			free(tokens[i + 1]);
			i += 2;
		}
		else
		{
			tokens[count] = tokens[i];
			count++;
			i++;
		}
	}
	tokens[count] = NULL;
	return (tokens);
}

void	expand_args(char **args, char **env_copy, t_mini *mini)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	while (args[i])
	{
		len = ft_strlen(args[i]);
		if (!strcmp(args[i], "\"|\"") || !strcmp(args[i], "\">\"") || \
			!strcmp(args[i], "\"<\"") || !strcmp(args[i], "\">>\"") || \
			!strcmp(args[i], "\"<<\""))
		{
			str = ft_strdup(args[i]);
			free(args[i]);
			args[i] = expand_var(ft_strndup(str, len), env_copy, mini);
			free(str);
			i++;
		}
		else
			i++;
	}
}

void	final_sliced_list(t_node **sliced_tokens_list)
{
	int		i;
	t_node	*current;
	t_node	*temp_next;

	current = *sliced_tokens_list;
	i = 0;
	while (current)
	{
		temp_next = current->next;
		if (i % 2 != 0)
		{
			if (current->previous)
				current->previous->next = current->next;
			else
				*sliced_tokens_list = current->next;
			if (current->next)
				current->next->previous = current->previous;
			free_args(current->tokens);
			free(current);
		}
		current = temp_next;
		i++;
	}
}
