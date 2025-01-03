/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_heredocs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:44:19 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/20 21:44:39 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	cleanup_heredoc_files(t_mini *mini)
{
	int	i;

	i = 0;
	while (i < 100)
	{
		if (mini->filenames[i] != NULL)
		{
			unlink(mini->filenames[i]);
			free(mini->filenames[i]);
			mini->filenames[i] = NULL;
		}
		i++;
	}
}
