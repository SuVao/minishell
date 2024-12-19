/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_operators.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:15:52 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/20 20:16:16 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	find_pipe(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{
		if (strcmp(tokens[i], "|") == 0)
			return (i);
		i++;
	}
	return (-1);
}
