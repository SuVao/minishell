/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:01:23 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/11 13:12:51 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*add_token_to_list(t_node *head, char **tokens)
{
	t_node	*new_node;
	t_node	*current;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
	{
		perror("Memory allocation-LIST error");
		return (NULL);
	}
	new_node->tokens = tokens;
	new_node->next = NULL;
	new_node->cmd = NULL;
	new_node->previous = NULL;
	new_node->redirs = NULL;
	new_node->args = NULL;
	new_node->infile = STDIN_FILENO;
	new_node->outfile = STDOUT_FILENO;
	if (head == NULL)
		return (new_node);
	current = head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
	current->next->previous = current;
	return (head);
}

void	parse_tokens(char **tokens, t_node **sliced_tokens_list)
{
	t_pt_data	data;

	data.pipe_index = find_pipe(tokens);
	if (data.pipe_index != -1)
	{
		data.left_slice = slice_tokens(tokens, 0, data.pipe_index);
		data.right_slice = slice_tokens(tokens, data.pipe_index + 1, -1);
		*sliced_tokens_list = add_token_to_list(*sliced_tokens_list, \
								data.left_slice);
		*sliced_tokens_list = add_token_to_list(*sliced_tokens_list, \
								data.right_slice);
		parse_tokens(data.right_slice, sliced_tokens_list);
	}
	else
		*sliced_tokens_list = add_token_to_list(*sliced_tokens_list, \
								slice_tokens(tokens, 0, -1));
}
