
#include "../inc/minishell.h"

void ft_env(t_mini *mini)
{
	int i = 0;
	while (mini->env[i])
		printf("%s\n", mini->env[i++]);
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

void update_env(char *new_pwd, char *pwd,t_mini *mini)
{
	int i;

	i = 0;
	if (!pwd)
	{
		free(pwd);
		pwd = getcwd(NULL, 0);
	}
	if (!new_pwd)
		new_pwd = getcwd(NULL, 0);
	while (mini->env[i] != NULL)
	{
		replace_var_env(mini->env, "OLDPWD=", pwd);
		replace_var_env(mini->env, "PWD=", new_pwd);
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
		if (mini->args[1][0] == '/')
			new_pwd = ft_strjoin(pwd, mini->args[1]);
		else
			new_pwd = ft_append_str(pwd, "/",mini->args[1]);
		printf("%s\n", new_pwd);
		if (chdir(new_pwd) == -1)
		{
			write(2, "Error: chdir failed2\n", 21);
			//if (mini->args)
			//	free_args(mini->args);
			free(new_pwd);
			free(pwd);
			free(home);
			return;
		}
		update_env(new_pwd, pwd, mini);
		free(new_pwd);
	}

	free(pwd);
	free(home);
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
