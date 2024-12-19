/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 20:14:44 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/11 14:10:19 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	check_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == ';')
		{
			printf("Error: ';' is not allowed\n");
			exit(0);
		}
		i++;
	}
}

bool	if_stats_input(t_mini *mini)
{
	if (!mini->line)
	{
		printf("exit\n");
		ft_exit(mini);
		return (false);
	}
	if (*mini->line)
	{
		add_history(mini->line);
		if (ft_strncmp(mini->line, "exit", 4) == 0)
		{
			printf("exit\n");
			ft_exit(mini);
			return (false);
		}
	}
	return (true);
}

void	init_mini(t_mini *mini)
{
	int	i;

	ft_bzero(mini, sizeof(t_mini));
	i = 0;
	while (i < 100)
	{
		mini->filenames[i] = NULL;
		i++;
	}
	i = 0;
	while (i < 1024)
	{
		mini->pids[i] = -1;
		i++;
	}
}

int	main(int ac, char **av, char **env)
{
	t_mini	*mini;

	main_pid()->pid = getpid();
	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (0);
	(void)ac;
	(void)av;
	init_mini(mini);
	if (env[0] == NULL)
		init_myown_envp(mini);
	else
		init_envp(mini, env);
	shell_looping(mini);
	free(mini);
	return (0);
}
