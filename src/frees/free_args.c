/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:27:40 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/28 18:03:32 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_all(char **tokens, char *input)
{
	int	i;

	i = 0;
	if (tokens)
	{
		while (tokens[i])
		{
			free(tokens[i]);
			tokens[i] = NULL;
			i++;
		}
		free(tokens);
		tokens = NULL;
	}
	if (input)
	{
		free(input);
		input = NULL;
	}
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

void	free_2_all(char **args1, char **args2)
{
	free_args(args1);
	free_args(args2);
}
