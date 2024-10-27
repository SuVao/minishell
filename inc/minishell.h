
# ifndef MINISHELL_H
#  define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

typedef struct s_mini
{
	int			ac;
	char		**av;
	char		**env;
	char		*line;
	char		**args;
	char		**path;
	int			shlvl;
	struct s_cmd	*cmd;
}	t_mini;

typedef struct s_cmd
{
	int				builtin;
	int				pipe;
	int				redir;
	int				redir_type;
	int				redir_fd;
	int				dollar;
	int				quote;
	int				double_quote;
	int				index;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_buildin
{
	void	(*ft_cd)(t_mini *mini);
	void	(*ft_pwd)();
	void	(*ft_env)(t_mini *mini);
	void	(*ft_export)(t_mini *mini);
	void	(*ft_unset)(t_mini *mini);
	void	(*ft_exit)(t_mini *mini);
} t_buildin;

void	ft_free(void **args);
void	ft_free_env(char **env);
void	ft_exit(t_mini *mini);
void	ft_pwd();
void	ft_env(t_mini *mini);
void	ft_export(t_mini *mini);
void	free_args(char **args);
void	ft_unset(t_mini *mini);
void	choose_args(t_mini *mini);
void	init_args(t_mini *mini, int ac, char **envp);
void	init_envp(t_mini *mini, char **envp);


#endif
