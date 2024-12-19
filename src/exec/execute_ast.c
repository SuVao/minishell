/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:01:41 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/11 13:26:05 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	exec(t_node *cur, t_mini *mini)
{
	char	*path;

	mini->is_busy = 1;
	ft_check_cmd(cur, mini);
	if (!is_echo(cur, mini))
	{
		path = get_path(cur->cmd, mini->env);
		if (!path)
		{
			main_pid()->status = 127;
			printf("%s: Command not found\n", cur->cmd);
			free_mini_things(cur, mini);
			exit(main_pid()->status);
		}
		execve(path, cur->args, mini->env);
		perror("execve error");
		free(path);
		exit(1);
	}
	free_mini_things(cur, mini);
}

void	handle_child(t_node *cur, t_mini *mini, int *i, int *infd)
{
	if (*i != 0)
	{
		dup2(*infd, STDIN_FILENO);
		close(*infd);
	}
	if (*i != mini->nr_pipes)
		dup2(mini->pipe_fd[1], STDOUT_FILENO);
	close(mini->pipe_fd[0]);
	close(mini->pipe_fd[1]);
	if (cur->infile != 0)
	{
		dup2(cur->infile, STDIN_FILENO);
		close(cur->infile);
	}
	if (cur->outfile != 1)
	{
		dup2(cur->outfile, STDOUT_FILENO);
		close(cur->outfile);
	}
	ft_close_all_fds(mini);
	exec(cur, mini);
	exit(EXIT_FAILURE);
}

void	do_pipe(t_node *cur, t_mini *mini, int *i, int *infd)
{
	pid_t	pid;

	main_pid()->exec = 1;
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		handling_sig_children();
		handle_child(cur, mini, i, infd);
	}
	else
	{
		mini->pids[*i] = pid;
		if (*infd != STDIN_FILENO)
			close(*infd);
		*infd = mini->pipe_fd[0];
		if (cur->infile != 0)
			close(cur->infile);
		if (cur->outfile != 1)
			close(cur->outfile);
		close(mini->pipe_fd[1]);
	}
}

static void	ft_loop_wait(int i, int status, t_mini *mini)
{
	int	j;

	j = 0;
	while (j <= i)
	{
		if (mini->pids[j] != -1)
		{
			waitpid(mini->pids[j], &main_pid()->status, 0);
			if (WIFEXITED(main_pid()->status))
				status = WEXITSTATUS(main_pid()->status);
			else if (WIFSIGNALED(main_pid()->status))
				status = 128 + WTERMSIG(main_pid()->status);
		}
		j++;
	}
}

void	execute(t_node *sliced_tokens_list, t_mini *mini)
{
	int		status;
	t_node	*cur;
	int		i;
	int		infd;

	infd = STDIN_FILENO;
	i = 0;
	cur = sliced_tokens_list;
	status = 0;
	if (is_builtins(cur, mini, &status))
		return ;
	while (cur)
	{
		if (pipe(mini->pipe_fd) == -1)
			perror("error pipe");
		do_pipe(cur, mini, &i, &infd);
		cur = cur->next;
		i++;
	}
	ft_close_all_fds(mini);
	ft_loop_wait(i, status, mini);
}
