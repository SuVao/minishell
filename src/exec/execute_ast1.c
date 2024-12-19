/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:26:30 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/11 13:27:17 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_mini_things(t_node *node, t_mini *mini)
{
	free_args(mini->env);
	free_ast(&node);
	free_all(mini->exp_tokens, mini->line);
	free(mini);
}

void	ft_check_cmd(t_node *cur, t_mini *mini)
{
	if (!cur->cmd || cur->cmd[0] == '\0')
	{
		free_mini_things(cur, mini);
		cleanup_heredoc_files(mini);
		exit(main_pid()->status);
	}
	if (cur->cmd[0] == '/' || cur->cmd[0] == '.')
	{
		execve(cur->cmd, cur->args, mini->env);
		perror("Is a directory");
		free_mini_things(cur, mini);
		main_pid()->status = 126;
		exit(main_pid()->status);
	}
}
