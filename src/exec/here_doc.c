/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:22:15 by mcarepa-          #+#    #+#             */
/*   Updated: 2024/12/11 13:43:22 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	create_filename(char *filename, t_mini *mini)
{
	char	*num_str;

	num_str = ft_itoa(mini->num);
	ft_strlcpy(filename, "temp", sizeof(filename));
	ft_strlcat(filename, num_str, sizeof(filename));
	free(num_str);
	mini->filenames[mini->num] = strdup(filename);
	mini->num++;
}

static int	ft_sig_int_hd(char *filename, int fd)
{
	if (main_pid()->sigint)
	{
		unlink(filename);
		close(fd);
		write(1, "\n", 2);
		main_pid()->status = 130;
		return (1);
	}
	return (0);
}

void	open_fd(int *fd, char *filename, t_mini *mini)
{
	main_pid()->here_doc = 1;
	create_filename(filename, mini);
	*fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (*fd == -1)
		perror("error failed to open");
}

int	here_doc(t_mini *mini, char *str)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	ssize_t	bytes_read;
	char	filename[50];

	open_fd(&fd, filename, mini);
	while (1)
	{
		bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
		if (bytes_read < 0)
		{
			if (ft_sig_int_hd(filename, fd))
				return (-1);
			perror("error read");
		}
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (ft_strncmp(buffer, str, ft_strlen(str)) == 0 && \
			(buffer[ft_strlen(str)] == '\n' || buffer[ft_strlen(str)] == '\0'))
			break ;
		write(fd, buffer, bytes_read);
	}
	close(fd);
	return (open(filename, O_RDONLY));
}
