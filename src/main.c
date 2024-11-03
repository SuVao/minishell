
#include "../inc/minishell.h"
#include <stdbool.h>

extern char **envp;

void free_args(char **args)
{
	int i = 0;
	if (!args)
		return;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
}

char	*path_find(char **envp, char *cmd)
{
	int		i;
	char	**possible_ways;
	char	*path;

	if (!envp || !cmd)
		return (NULL);
	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	possible_ways = ft_split(envp[i] + 19, ':');
	i = 0;
	while (possible_ways[i])
	{
		path = ft_append_str(possible_ways[i], "/", cmd);
		if (access(path, F_OK) == 0)
			break ;
		else
		{
			free(path);
			path = NULL;
			i++;
		}
	}
	free_args(possible_ways);
	return (path);
}


void check_line(char *line)
{
	int i = 0;
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

void ft_free_env(char **env)
{
	int i = 0;
	if (!env) return;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void ft_exit(t_mini *mini)
{
	clear_history();
	ft_free_env(mini->env);
	free_args(mini->args);
	free(mini->line);
	free(mini);
	exit(0);
}

void ft_pwd()
{
	char *pwd;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		printf("Error: getcwd failed\n");
		return ;
	}
	printf("%s\n", pwd);
	free(pwd);
}

void ft_export(t_mini *mini)
{
	int i = 0;
	while (mini->env[i] != NULL)
		i++;
	mini->env[i] = ft_strjoin("NEW_VAR=", mini->args[1]);
}



void ft_unset(t_mini *mini)
{
	int i;
	int j;
	char **new_env;

	i = 0;
	j = 0;
	while (mini->env[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * i);
	i = 0;
	while (mini->env[i])
	{
		if (ft_strncmp(mini->env[i], mini->args[1], ft_strlen(mini->args[1])) != 0)
		{
			new_env[j] = mini->env[i];
			j++;
		}
		else
		{
			free(mini->env[i]);
		}
		i++;
	}
	new_env[j] = NULL;
	free(mini->env);
	mini->env = new_env;
}

void ft_echo(t_mini *mini)
{
	int i = 1;
	while (mini->args[i])
	{
		printf("%s", mini->args[i]);
		i++;
		if (mini->args[i])
			printf(" ");
	}
	printf("\n");
}

void ft_exec(t_mini *mini)
{
	char	**cmd;
	char	*path;

	cmd = NULL;
	path = NULL;

	path = path_find(mini->env, mini->args[0]);
	if (!path)
	{
		free_all(cmd, path);
		write(2, "Command not found\n", 19);
		exit(127);
	}
	if (execve(path, cmd, mini->env) == -1)
		free_all(cmd, path);
}

void ft_execute(t_mini *mini)
{
	int pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		printf("pid error!\n");
		ft_exit(mini);
	}
	if (pid == 0)
		ft_exec(mini);
	else
	waitpid(pid, &status, 0);
}

void choose_args(t_mini *mini)
{
	if (!mini->line)
		printf("ola\n");
	mini->args = ft_split(mini->line, ' ');
	if (!mini->args)
	{
		printf("Error: split failed\n");
		exit(0);
	}
	if (ft_strncmp(mini->args[0], "echo", 4) == 0)
		ft_echo(mini);
	else if (ft_strncmp(mini->args[0], "cd", 2) == 0)
		ft_cd(mini);
	else if (ft_strncmp(mini->args[0], "pwd", 3) == 0)
		ft_pwd();
	else if (ft_strncmp(mini->args[0], "export", 6) == 0)
		ft_export(mini);
	else if (ft_strncmp(mini->args[0], "unset", 5) == 0)
		ft_unset(mini);
	else if (ft_strncmp(mini->args[0], "env", 3) == 0)
		ft_env(mini);
	else if (ft_strncmp(mini->args[0], "exit", 4) == 0)
		ft_exit(mini);
	else
		ft_execute(mini);
//	wait()
	if (mini->args)
		free_args(mini->args);
}

void init_envp(t_mini *mini, char **envp)
{
	int i;

	i = 0;
	while (envp[i])
		i++;
	mini->env = (char **)malloc(sizeof(char *) * (i + 1));
	i = -1;
	while (envp[++i])
		mini->env[i] = ft_strdup(envp[i]);
	mini->env[i] = NULL;
}

void init_myown_envp(t_mini *mini)
{
	int i;
	char *pwd;

	i = 0;
	mini->env = (char **)malloc(sizeof(char *) * 3);
	mini->shlvl = 1;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		printf("Error: getcwd failed\n");
		return ;
	}
	mini->env[0] = ft_strjoin("PWD=", pwd);
	mini->env[1] = ft_strjoin("SHLVL=", ft_itoa(mini->shlvl));
	mini->env[2] = NULL;
	free(pwd);
}

void handling_signals(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
	}
}

// int main(int ac, char **av, char **envp)
// {
//     t_mini *main_mini;
//     char *line;
//     struct sigaction sa;

//     line = NULL;
//     ft_bzero(&sa, sizeof(sa));
//     sa.sa_flags = SA_RESTART;
//     sa.sa_sigaction = &handling_signals;
//    	sigaction(SIGINT, &sa, NULL);
//     sigaction(SIGTSTP, &sa, NULL);
//     (void)av, (void)ac;

//     signal(SIGQUIT, SIG_IGN);
// 	main_mini = (t_mini *)malloc(sizeof(t_mini));
//     if (!envp)
//     	init_myown_envp(main_mini);
//     else
//     	init_envp(main_mini, envp);
//     while (1)
//     {
//         line = readline("minishell> ");
//         if (!line)
//         {
//            	printf("Exiting of shell!\n");
//            	break ;
//         }
//         if (*line)
//         {
//             main_mini->line = ft_strdup(line);
//             if (main_mini->line && *main_mini->line != '\n')
//             {
//                 add_history(main_mini->line);
//                 choose_args(main_mini);
//                 free(main_mini->line);
//             }
//         }
//         free(line);
//     }
//     free(main_mini);
//     return 0;
// }

bool check_for_unclosed_quotes(char *input)
{
	int i;
	int single_quote_open;
	int double_quote_open;

	i = 0;
	single_quote_open = 0;
	double_quote_open = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !double_quote_open)
			single_quote_open = !single_quote_open;
		else if (input[i] == '"' && !single_quote_open)
			double_quote_open = !double_quote_open;
		i++;
	}
	if (single_quote_open || double_quote_open)
		return (false);
	return (true);
}

bool	if_stats_input(char *input)
{	//false para break //true para continue
	if (!input)
	{
		printf("exit\n");
		return (false);
	}
	if (!*input)
	{
		free(input);
		return (true);
	}
	if (*input)
	{
		add_history(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			printf("exit\n");
			return (false);
		}
	}
	return (false);
}

int semicolon_checker(char *str)
{
	int	i;
	int	s_quote;
	int	d_quote;

	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '"' && !s_quote)
			d_quote = !d_quote;
		else if (str[i] == '\'' && !d_quote)
			s_quote = !s_quote;
		else if (str[i] == ';' && !s_quote && !d_quote)
			return (false);
		else if (str[i] == '\\' && !s_quote && !d_quote)
			return (false);
		i++;
	}
	return (true);
}

bool checker_quotes(char *input)
{
	if (!check_for_unclosed_quotes(input))
	{
		write(2, "Error unclosed quotes\n", 22);
		free(input);
		return (false);
	}
	if (!semicolon_checker(input))
	{
		write(2, "Error semicolon not supported\n", 30);
		free(input);
		return (false);
	}
	return (true);
}

void	init_mini(t_mini *mini)
{
	mini->ac = 0;
	mini->new_tokens = NULL;
	mini->exp_tokens = NULL;
	mini->env = NULL;
	mini->line = NULL;
	mini->args = NULL;
	mini->shlvl = 0;
}

int main(int ac, char **av, char **env)
{
	int 		i;
	t_ast_node *ast_root;
	t_mini		*mini;

	mini = malloc(sizeof(t_mini*));
	(void)ac;
	(void)av;
	mini = NULL;
	ast_root = NULL;
	init_mini(mini);
	i = 0;
	if (!envp)
   		init_myown_envp(mini);
    else
   		init_envp(mini, envp);
	while (1)
	{
		mini->line = readline("minishell> ");
		if (!if_stats_input(mini->line))
			break;
		if (!checker_quotes(mini->line))
			continue;

		i = 0;
		mini->new_tokens = tokenize(mini->line);
		if (!mini->new_tokens)
		{
    		free(mini->line);
    		perror("Error in tokenize");
    		continue;
		}
	}
	free(mini);
	return (0);
}
