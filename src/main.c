
#include "../inc/minishell.h"

void check_line(char *line)
{
	int i;

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

void	ft_free(void **args)
{
	while (*(args))
	{
		free(*args);
		args++;
	}
	free(*(args));
}

void ft_free_env(char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void ft_exit(t_mini *mini)
{
	int i;

	i = 0;
	clear_history();
	// while (mini->env[i])
	// {
	// 	free(mini->env[i]);
	// 	i++;
	// }
	free(mini->env);
	i = 0;
	while (mini->args[i])
	{
		free(mini->args[i]);
		i++;
	}
	free(mini->args);
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
		exit(0);
	}
	printf("%s\n", pwd);
	free(pwd);
}

void ft_env(t_mini *mini)
{
	int i;

	i = 0;
	while (mini->env[i])
	{
		printf("%s\n", mini->env[i]);
		i++;
	}
}

void	ft_export(t_mini *mini)
{
	int i;
	char *tmp;
	i = 0;
	while (mini->env[i] != NULL)
		i++;
	tmp = ft_strjoin(mini->env[i], mini->args[1]);
//	free(mini->env[i]);
	mini->env[i] = tmp;
	free(tmp);
}

void free_args(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void ft_unset(t_mini *mini)
{
	int i;
	int j;
	int k;
	char **new_env;

	i = 0;
	j = 0;
	k = 0;
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
		i++;
	}
	new_env[j] = NULL;
	free(mini->env);
	mini->env = new_env;
}

void ft_cd(t_mini *mini)
{
	char *home;
	char *pwd;

	pwd = getcwd(NULL, 0);
	home = getenv("HOME");
	if (pwd == NULL)
	{
		printf("Error: getcwd failed\n");
		exit(0);
	}
	if (mini->args[1] == NULL)
	{
		chdir(home);
	}
	else if (chdir(mini->args[1]) == -1)
	{
		printf("Error: chdir failed\n");
	}
}

void ft_echo(t_mini *mini)
{
	int i;

	i = 1;
	while (mini->args[i])
	{
		printf("%s", mini->args[i]);
		i++;
		if (mini->args[i])
			printf(" ");
	}
	printf("\n");
	i = 0;
	while (mini->args[i])
	{
		free(mini->args[i]);
		i++;
	}
	free(mini->args);
}

void choose_args(t_mini *mini)
{
	int i;

	i = 0;
	mini->args = ft_split(mini->line, ' ');
	if (mini->args == NULL)
	{
		printf("Error: split failed\n");
		exit(0);
	}
	if (ft_strncmp(mini->args[0], "echo", 4) == 0)
	{
		ft_echo(mini);
	}
	else if (ft_strncmp(mini->args[0], "cd" , 2) == 0)
	{
		ft_cd(mini);
	}
	else if (ft_strncmp(mini->args[0], "pwd", 3) == 0)
	{
		free_args(mini->args);
		ft_pwd();
	}
	else if (ft_strncmp(mini->args[0], "export", 7) == 0)
	{
		free_args(mini->args);
		ft_export(mini);
	}
	else if (ft_strncmp(mini->args[0], "unset", 5) == 0)
	{
		free_args(mini->args);
		ft_unset(mini);
	}
	else if (ft_strncmp(mini->args[0], "env", 3) == 0)
	{
		free_args(mini->args);
		ft_env(mini);
	}
	else if (ft_strncmp(mini->args[0], "exit", 4) == 0)
	{
		ft_exit(mini);
	}
	else
	{
		free_args(mini->args);
		printf("Error: command not found\n");
	}
	// else
	// {
	// 	ft_execve(mini, args);
	// }
//	ft_free((void**)args);
}

void init_args(t_mini *mini, int ac, char **envp)
{
	(void)envp;
	mini->ac = ac;
}

void init_envp(t_mini *mini, char **envp)
{
	int i;
	char **env;

	i = 0;
	env = envp;
	while (env[i])
		i++;
	mini->env = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		mini->env[i] = env[i];
		i++;
	}
	mini->env[i] = NULL;
}

int main(int ac, char **av, char **envp)
{
    t_mini *main_mini;
    char *line;

    (void)av;
    main_mini = (t_mini *)malloc(sizeof(t_mini));
    init_envp(main_mini, envp);
    line = NULL;
    while (1)
    {
        printf("minishell> ");
        line = readline("");
        while (line != NULL)
        {
        	main_mini->line = ft_strdup(line);
         	if (main_mini->line && *main_mini->line != '\n')
          	{
         		add_history(main_mini->line);
           		check_line(main_mini->line);
           		init_args(main_mini, ac, envp);
           		choose_args(main_mini);
             	free(main_mini->line);
             	free(line);
             	break;
		 	}
			printf("\n");
			free(main_mini->line);
        	free(line);
           	line = readline("");
        }
    }
    return 0;
}
