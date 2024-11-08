
#include "../inc/minishell.h"

int	here_doc(char *str)
{
	char	buffer[BUFFER_SIZE];
	int		fd;
	ssize_t	bytes_read;

	fd = open("temp.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		perror("error failed to open");
	while (1)
	{
		/* write(STDOUT_FILENO, "> ", 2); */
		bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
		if (bytes_read < 0)
			perror("error read");
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		if (ft_strncmp(buffer, str, ft_strlen(str)) == 0 && \
	(buffer[ft_strlen(str)] == '\n' || buffer[ft_strlen(str)] == '\0'))
			break ;
		write(fd, buffer, bytes_read);
	}
	close(fd);
	fd = open("temp.txt", O_RDONLY);
	if (fd == -1)
    {
        perror("Failed to reopen temporary file");
        return -1;
    }
	unlink("temp.txt");
	ft_close_all_fds();
	return (fd);
}
