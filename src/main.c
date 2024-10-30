
#include "../inc/minishell.h"
char *get_var_env(char **env, char *to_find)
{
	int i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], to_find, ft_strlen(to_find)) == 0)
			return (ft_strdup(ft_strchr(env[i], '/') ));
		i++;
	}
	return NULL;
}

void free_args(char **args)
{
	int i = 0;
	if (!args) return;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	args = NULL;
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

void ft_env(t_mini *mini)
{
	int i = 0;
	while (mini->env[i])
		printf("%s\n", mini->env[i++]);
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

void replace_var_env(char **envp, char *to_found, char *to_replace)
{
	int i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], to_found, ft_strlen(to_found)) == 0)
		{
			free(envp[i]);
			envp[i] = ft_strjoin(to_found, to_replace);
			break;
		}
		i++;
	}
}

void update_env(char *pwd, t_mini *mini)
{
	int i;
	char *new_pwd;

	new_pwd = NULL;
	i = 0;
	if (!pwd)
	{
		free(pwd);
		new_pwd = getcwd(NULL, 0);
	}
	new_pwd = ft_strjoin(pwd, mini->args[1]);
	if (!new_pwd)
		new_pwd = getcwd(NULL, 0);
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", new_pwd);
		replace_var_env(mini->env, "PWD=", mini->args[1]);
		i++;
		break;
	}
	if (new_pwd)
		free(new_pwd);
}

void update_env_abs(char *pwd, char *home, t_mini *mini)
{
	int i;

	i = 0;
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", home);
		i++;
		break;
	}
}


char *encontra_barra(char *s)
{
	char *new_pwd;
	int j;
	int i;

	new_pwd = NULL;
	i = 0;
	while (s[i])
		i++;
	while (i > 0)
	{
		if (s[i] == '/')
			break ;
		i--;
	}
	new_pwd = malloc(sizeof(char) * (i + 1));
	if (!new_pwd)
		return (NULL);
	j = 0;
	while (j != i)
	{
		new_pwd[j] = s[j];
		j++;
	}
	new_pwd[j] = '\0';
	return (new_pwd);
}

void update_env_back_cd(char *new_pwd, char*pwd, t_mini *mini)
{
	int i;

	i = 0;
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", new_pwd);
		i++;
		break;
	}
}

void ft_cd(t_mini *mini)
{
	char *home;
	char *pwd;
	char *new_pwd;

	new_pwd = NULL;
	home = get_var_env(mini->env, "HOME=");
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{

		free(home);
		free(pwd);
		printf("Error: getcwd failed\n");
		return ;
	}
	if (!mini->args[1])
	{
		if (chdir(home) == -1)
		{
			if (mini->args)
				free_args(mini->args);
			free(home);
			free(pwd);
			printf("Error: chdir failed\n");
			exit(0);
		}
		update_env_abs(pwd, home, mini);
	//	free(mini->args[0]);
	}
	else if (ft_strncmp(mini->args[1], "..", 2) == 0)
	{
		new_pwd = encontra_barra(pwd);
		if (chdir(new_pwd) == -1)
		{
			write(2, "Error: chdir failed1\n", 21);
			//if (mini->args)
			//	free_args(mini->args);
			free(pwd);
			free(home);
			return;
		}
		update_env_back_cd(new_pwd, pwd, mini);
		free(new_pwd);

	}
	else if (mini->args[1])
	{
		if (chdir(mini->args[1]) == -1)
		{
			write(2, "Error: chdir failed2\n", 21);
			//if (mini->args)
			//	free_args(mini->args);
			free(pwd);
			free(home);
			return;
		}
		update_env(pwd, mini);
	}

	free(pwd);
	free(home);
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

void choose_args(t_mini *mini)
{
	if (!mini->line)
		printf("ola\n");
	mini->args = ft_split(mini->line, ' ');
	if (!mini->args)
	{
		printf("Error: split failed5\n");
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
		printf("Error: command not found\n");
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
	// while (mini->env[i])
	// {
	// 	printf("%s\n", mini->env[i]);
	// 	i++;
	// }
	free(pwd);
}

void handling_signals(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
	}
}

int main(int ac, char **av, char **envp)
{
    t_mini *main_mini;
    char *line;
    struct sigaction sa;

    line = NULL;
    ft_bzero(&sa, sizeof(sa));
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handling_signals;
   	sigaction(SIGINT, &sa, NULL);
    (void)av, (void)ac;

	main_mini = (t_mini *)malloc(sizeof(t_mini));
    if (!envp)
    	init_myown_envp(main_mini);
    else
    	init_envp(main_mini, envp);
    while (1)
    {
        line = readline("minishell> ");
        if (*line)
        {
            main_mini->line = ft_strdup(line);

            if (main_mini->line && *main_mini->line != '\n')
            {
                add_history(main_mini->line);
                choose_args(main_mini);
                free(main_mini->line);
            }
            // free(line);
        }
        free(line);
    }
    free(main_mini);
    return 0;
}
