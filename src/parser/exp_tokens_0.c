/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_tokens_0.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pesilva- <pesilva-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 20:15:20 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/04 20:10:13 by pesilva-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_redirs(char **new_tokens, int *i)
{
	while (new_tokens[*i])
	{
		if (ft_strcmp(new_tokens[*i], ">") == 0 || \
			ft_strcmp(new_tokens[*i], "<") == 0 || \
			ft_strcmp(new_tokens[*i], ">>") == 0 || \
			ft_strcmp(new_tokens[*i], "<<") == 0)
		{
			if (!new_tokens[*i + 1])
			{
				perror("Syntax error: missing file for redirection");
				return (0);
			}
			if (ft_strcmp(new_tokens[*i + 1], ">") == 0 || \
				ft_strcmp(new_tokens[*i + 1], "<") == 0 || \
				ft_strcmp(new_tokens[*i + 1], ">>") == 0 || \
				ft_strcmp(new_tokens[*i + 1], "<<") == 0)
			{
				perror("syntax error near unexpected token `>'/`<'");
				return (0);
			}
		}
		(*i)++;
	}
	return (1);
}

int	fin(char **new_tokens, char **env_copy, t_mini *mini, char **exp_tokens)
{
	int		len;
	char	*var;
	int		i;

	i = 0;
	while (new_tokens[i])
	{
		len = ft_strlen(new_tokens[i]);
		if (!ft_strcmp(new_tokens[i], "\"|\"") || \
			!ft_strcmp(new_tokens[i], "\">\"") || \
			!ft_strcmp(new_tokens[i], "\"<\"") || \
			!ft_strcmp(new_tokens[i], "\">>\"") || \
			!ft_strcmp(new_tokens[i], "\"<<\""))
		{
			exp_tokens[i] = ft_strdup(new_tokens[i]);
			free(new_tokens[i]);
			i++;
			continue ;
		}
		var = expand_var(ft_strndup(new_tokens[i], len), env_copy, mini);
		free(new_tokens[i]);
		exp_tokens[i] = var;
		i++;
	}
	return (i);
}

char	**expand_vars(char **new_tokens, char **env_copy, t_mini *mini)
{
	int		i;
	char	**exp_tokens;

	i = 0;
	if (new_tokens[i] && new_tokens[i][0] == '|')
	{
		perror("pipe error");
		return (NULL);
	}
	if (!check_redirs(new_tokens, &i))
		return (NULL);
	exp_tokens = malloc((i + 1) * sizeof(char *));
	if (!exp_tokens)
		return (NULL);
	i = fin(new_tokens, env_copy, mini, exp_tokens);
	free(new_tokens);
	exp_tokens[i] = NULL;
	return (exp_tokens);
}
