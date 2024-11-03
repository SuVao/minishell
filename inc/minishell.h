
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
# include <stdbool.h>

# define MAX_TOKENS 300
# define CMD 1
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define REDIR_APPEND 5
# define HEREDOC 6
# define FILE 7
# define BUFFER_SIZE 1024

typedef struct s_mini
{
	int			ac;
	char		**new_tokens;
	char		**exp_tokens;
	char		**env;
	char		*line;
	char		**args;
	int			shlvl;
}	t_mini;

typedef struct s_ast_node
{
	int		type;//	Type of node
	char	*cmd; //used for command nodes
	char	**args; //arguments for command nodes
	char	*file; //in case of redirections
	char	*delimiter; // for here documents
	struct s_ast_node *left;
	struct s_ast_node *right;
} t_ast_node;

typedef struct
{
	char	*exp;
	char	*var_start;
	char	*temp;
	int		i;
	int		d_quote;
	int		s_quote;
	int		len;
	int		start;
}	expand_data;

typedef struct
{
	char **t;
	int	i;
	int	start;
	int	ti;
	int	d_quote;
	int	s_quote;
}	tokenize_data;

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
void 	replace_var_env(char **envp, char *to_found, char *to_replace);
void 	update_env(char *new_pwd, char *pwd,t_mini *mini);
void 	update_env_abs(char *pwd, char *home, t_mini *mini);
char 	*encontra_barra(char *s);
void 	update_env_back_cd(char *new_pwd, char*pwd, t_mini *mini);
void 	ft_cd(t_mini *mini);
char 	*get_var_env(char **env, char *to_find);
char	*path_find(char **envp, char *cmd);
void 	ft_exec(t_mini *mini);
void	free_all1(char **matrix, char *str);
char	**tokenize(char *str);
void 	free_all(char **tokens, char *input);


#endif
