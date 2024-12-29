/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 19:04:53 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/12 17:32:08 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_pid	*main_pid(void)
{
	static t_pid	main_pid;

	return (&main_pid);
}

void	handling_signals(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		main_pid()->status = 130;
		main_pid()->sigint = 130;
		if (main_pid()->here_doc == 0 && main_pid()->exec == 0)
		{
			write(1, "\n", 2);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else if (main_pid()->exec == 1)
		{
			write(1, "\n", 2);
			main_pid()->exec = 0;
		}
		else if (main_pid()->here_doc == 1)
		{
			main_pid()->here_doc = 0;
		}
	}
}

void	ft_sig_init(t_mini *mini)
{
	mini->sa.sa_flags = SA_SIGINFO;
	mini->sa.sa_sigaction = &handling_signals;
	sigaction(SIGINT, &mini->sa, NULL);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	handling_sig_children(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
