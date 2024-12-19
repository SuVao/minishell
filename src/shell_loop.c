/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:10:54 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/12 16:52:53 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	input_check(t_mini *mini)
{
	if (!main_pid()->sigint)
		mini->line = readline("minishell> ");
	else
		return (0);
	return (if_stats_input(mini));
}

int	handle_input(t_mini *mini)
{
	if (!checker_quotes(mini->line))
		return (0);
	mini->new_tokens = tokenize(mini->line);
	if (!mini->new_tokens)
	{
		free(mini->line);
		return (0);
	}
	mini->exp_tokens = expand_vars(mini->new_tokens, mini->env, mini);
	if (!mini->exp_tokens)
	{
		free_all(mini->new_tokens, mini->line);
		return (0);
	}
	return (1);
}

int	execute_program(t_mini *mini, t_node **sliced_tokens_list)
{
	parse_tokens(mini->exp_tokens, sliced_tokens_list);
	final_sliced_list(sliced_tokens_list);
	mini->nr_pipes = 0;
	fill_node(sliced_tokens_list, mini);
	mini->nr_pipes--;
	if (open_redirs(*sliced_tokens_list, mini))
	{
		cleanup_heredoc_files(mini);
		free_ast(sliced_tokens_list);
		free_all(mini->exp_tokens, mini->line);
		main_pid()->sigint = 0;
		return (0);
	}
	heredoc_last(sliced_tokens_list);
	free(mini->line);
	mini->line = NULL;
	execute(*sliced_tokens_list, mini);
	return (1);
}

void	shell_looping(t_mini *mini)
{
	t_node	*sliced_tokens_list;

	ft_sig_init(mini);
	while (1)
	{
		if (!input_check(mini))
			continue ;
		if (!handle_input(mini))
			continue ;
		sliced_tokens_list = NULL;
		if (*mini->line)
		{
			if (!execute_program(mini, &sliced_tokens_list))
				continue ;
		}
		free_ast(&sliced_tokens_list);
		free_all(mini->exp_tokens, mini->line);
		if (mini)
			cleanup_heredoc_files(mini);
		main_pid()->sigint = 0;
	}
}
