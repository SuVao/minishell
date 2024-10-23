
// #include "../inc/minishell.h"

// int	if_builtins(char *cmd)
// {
// 	if (ft_strcmp(cmd, "echo") == 0)
// 		return (1);
// 	if (ft_strcmp(cmd, "cd") == 0)
// 		return (1);
// 	if (ft_strcmp(cmd, "pwd") == 0)
// 		return (1);
// 	if (ft_strcmp(cmd, "export") == 0)
// 		return (1);
// 	if (ft_strcmp(cmd, "unset") == 0)
// 		return (1);
// 	if (ft_strcmp(cmd, "env") == 0)
// 		return (1);
// 	if (ft_strcmp(cmd, "exit") == 0)
// 		return (1);
// 	return (0);
// }

// void	check_line(char *line, t_mini *mini)
// {
// 	int i;
// 	char **args;

// 	i = 0;
// 	args = ft_split(line, ' ');
// 	if (!args)
// 		return ;
// 	if (if_builtins(args[0]) == 1)
// 	{

// 	}


// }

// void	loop(t_mini *mini, char *line)
// {
// 	while (line != NULL)
// 	{
// 		check_line(line, mini);
// 	}
// }

// int main(int ac, char **av, char **envp)
// {
// 	t_mini *mini;
// 	char *line;

// 	if (ac != 1)
// 		return (0);
// 	mini = malloc(sizeof(t_mini));
// 	init_envp(mini, envp);
// 	line = NULL;
// 	while (1)
// 	{
// 		write(1 , "minishell> ", 11);
// 		line = readline("");
// 		loop(mini, line);

// 	}
// }
