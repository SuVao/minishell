/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_redirs1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:01:43 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/11 14:01:48 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	redir_cases(t_mini *mini)
{
	if (mini->redir->type == REDIR_IN)
		add_redir_in(mini);
	else if (mini->redir->type == REDIR_OUT)
		add_redir_out(mini);
	else if (mini->redir->type == REDIR_APPEND)
		add_redir_app(mini);
}
