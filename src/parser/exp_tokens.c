/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 18:22:41 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/11 13:14:16 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*replace(char *var_name, char **env_copy)
{
	int		i;
	char	*var_string;
	char	*value;

	i = 0;
	while (env_copy[i])
	{
		var_string = env_copy[i];
		if ((ft_strncmp(var_name, var_string, ft_strlen(var_name)) == 0)
			&& var_string[ft_strlen(var_name)] == '=')
		{
			value = ft_strdup(var_string + ft_strlen(var_name) + 1);
			free(var_name);
			return (value);
		}
		i++;
	}
	free(var_name);
	return (ft_strdup(""));
}

void	token_expand(t_expand_data *d, char *str, char **env_copy)
{
	d->start = d->i + 1;
	while (str[d->i + 1] && str[d->i + 1] != ' ' && \
			str[d->i + 1] != '"' && str[d->i + 1] != '\'' && \
			str[d->i + 1] != '$')
	{
		d->i++;
		d->var_start = ft_strndup(&str[d->start], d->i - d->start + 1);
		d->temp = replace(d->var_start, env_copy);
		d->exp = (char *)ft_realloc(d->exp, ft_strlen(d->exp)
				+ ft_strlen(d->temp) + 1, ft_strlen(d->exp) + 1);
		strcat(d->exp, d->temp);
		free(d->temp);
	}
}

void	handle_exp(char *str, t_expand_data *d, t_mini *mini, char **env_copy)
{
	(void)mini;
	if (str[d->i] == '$' && str[d->i + 1] == '?' )
	{
		d->i++;
		d->temp = ft_itoa(main_pid()->status / 256);
		d->exp = (char *)ft_realloc(d->exp, ft_strlen(d->exp) \
				+ ft_strlen(d->temp) + 1, ft_strlen(d->exp) + 1);
		ft_strlcat(d->exp, d->temp, ft_strlen(d->exp) + ft_strlen(d->temp) + 1);
		free(d->temp);
	}
	else
		token_expand(d, str, env_copy);
}

char	*expand_var(char *str, char **env_copy, t_mini *mini)
{
	t_expand_data	d;

	d.exp = ft_strdup("");
	d.i = 0;
	d.d_quote = 0;
	d.s_quote = 0;
	while (str[d.i])
	{
		if (str[d.i] == '"' && !d.s_quote)
			d.d_quote = !d.d_quote;
		else if (str[d.i] == '\'' && !d.d_quote)
			d.s_quote = !d.s_quote;
		else if (str[d.i] == '$' && str[d.i + 1] && (d.d_quote || !d.s_quote))
			handle_exp(str, &d, mini, env_copy);
		else
		{
			d.len = ft_strlen(d.exp);
			d.exp = (char *)ft_realloc(d.exp, d.len + 2, ft_strlen(d.exp) + 1);
			d.exp[d.len] = str[d.i];
			d.exp[d.len + 1] = '\0';
		}
		d.i++;
	}
	free(str);
	return (d.exp);
}
