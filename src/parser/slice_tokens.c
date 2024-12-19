/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   slice_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:18:50 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/11/20 21:45:25 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	fill_sliced(char **sliced, int *count, char **tokens, int *i)
{
	int	j;

	sliced[*count] = ft_strdup(tokens[*i]);
	if (!sliced[*count])
	{
		j = 0;
		while (j < *count)
		{
			free(sliced[j]);
			j++;
		}
		free(sliced);
		return (0);
	}
	(*count)++;
	(*i)++;
	return (1);
}

void	free_sliced(char **sliced, int count)
{
	int	j;

	j = 0;
	while (j < count)
	{
		free(sliced[j]);
		j++;
	}
	free(sliced);
}

int	get_end(char **tokens, int *end, int *count)
{
	while (tokens[*count] != NULL)
		(*count)++;
	*end = *count;
	return (*end);
}

char	**slice_tokens(char **tokens, int i, int end)
{
	int		count;
	int		n_free;
	char	**sliced;

	count = i;
	if (end == -1)
		end = get_end(tokens, &end, &count);
	n_free = end - i + 1;
	sliced = malloc((end - i + 1) * sizeof(char *));
	if (!sliced)
		return (NULL);
	count = 0;
	while (i < end)
	{
		if (!fill_sliced(sliced, &count, tokens, &i))
		{
			free_sliced(sliced, n_free);
			return (NULL);
		}
	}
	sliced[count] = NULL;
	return (sliced);
}
