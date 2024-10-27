
#include "../inc/minishell.h"

void free_args(char **args)
{
	int i = 0;
	if (!args) return;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
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
	char *pwd = getcwd(NULL, 0);
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
	int i = 0;
	while (mini->env[i])
	{
		printf("%s\n", mini->env[i]);
		i++;
	}
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

	i = 0;
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", mini->args[1]);
		i++;
		break;
	}
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

void ft_cd(t_mini *mini)
{
	char *home;
	char *pwd;

	home = get_var_env(mini->env, "HOME=");
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		printf("Error: getcwd failed\n");
		exit(0);
	}
	if (!mini->args[1])
	{
		if (chdir(home) == -1)
			printf("Error: chdir failed\n");
		update_env_abs(pwd, home, mini);
	}
	else
	{
		if (chdir(mini->args[1]) == -1)
			ft_exit(mini);
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
		printf("Error: command not found\n");
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
		exit(0);
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

int main(int ac, char **av, char **envp)
{
    t_mini *main_mini;

    (void)av;
    if (ac != 1)
		return 0;
	main_mini = (t_mini *)malloc(sizeof(t_mini));
    if (!envp)
    	init_myown_envp(main_mini);
    else
    	init_envp(main_mini, envp);
    char *line;
    while (1)
    {
        printf("minishell> ");
        line = readline("");
        if (line)
        {
            main_mini->line = ft_strdup(line);
            if (main_mini->line && *main_mini->line != '\n')
            {
                add_history(main_mini->line);
                choose_args(main_mini);
                free(main_mini->line);
            }
            free(line);
        }
    }
    return 0;
}
