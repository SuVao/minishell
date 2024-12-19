/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcarepa- <mcarepa-@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:08:56 by pesilva-          #+#    #+#             */
/*   Updated: 2024/12/12 16:46:08 by mcarepa-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <sys/ioctl.h>

# define MAX_TOKENS 7000
# define CMD 1
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define REDIR_APPEND 5
# define HEREDOC 6
# define FILE 7
# define CD 8
# define PWD 9
# define EXPORT 10
# define UNSET 11
# define ENV 12
# define BUFFER_SIZE 1024

typedef struct s_pid
{
	pid_t			pid;
	int				status;
	int				here_doc;
	int				sigint;
	int				exec;
}	t_pid;

typedef struct s_redir
{
	int					type;
	char				*target;
	struct s_redir		*next;
}	t_redir;

typedef struct s_node
{
	char			**tokens;
	char			*cmd;
	char			**args;
	t_redir			*redirs;
	int				infile;
	int				outfile;
	struct s_node	*next;
	struct s_node	*previous;
}	t_node;

typedef struct s_mini
{
	char				**new_tokens;
	char				**exp_tokens;
	char				**env;
	char				*line;
	char				**args;
	char				**env_copy;
	int					shlvl;
	int					is_busy;
	int					exit_status;
	int					pipe_fd[2];
	char				*filenames[100];
	pid_t				pids[1024];
	int					file_count;
	int					num;
	int					nr_pipes;
	t_node				*cur;
	t_redir				*redir;
	struct sigaction	sa;
	struct sigaction	sa_default;
}	t_mini;

typedef struct s_getpath
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;
	char	*env_path;
}	t_getpath;

typedef struct s_expand_data
{
	char	*exp;
	char	*var_start;
	char	*temp;
	int		i;
	int		d_quote;
	int		s_quote;
	int		len;
	int		start;
}	t_expand_data;

typedef struct s_tokenize_data
{
	char	**t;
	int		i;
	int		start;
	int		ti;
	int		d_quote;
	int		s_quote;
}	t_tokenize_data;

typedef struct s_pt_data
{
	int			pipe_index;
	char		**left_slice;
	char		**right_slice;
}	t_pt_data;

//BUILTINS
int				is_builtins(t_node *node, t_mini *mini, int *status);

//ENV
void			ft_env(t_mini *mini);
void			init_envp(t_mini *mini, char **envp);
char			*encontra_barra(char *s);
char			*get_var_env(char **env, char *to_find);
void			init_myown_envp(t_mini *mini);

//CD
void			update_env_back_cd(char *new_pwd, char*pwd, t_mini *mini);
void			update_env(char *new_pwd, char *pwd, t_mini *mini);
void			update_env_abs(char *pwd, char *home, t_mini *mini);
void			replace_var_env(char **envp, char *to_found, char *to_replace);
void			ft_cd(t_node *node, t_mini *mini, int *status);
char			*ft_encontra_o_home(void);
void			ft_cd_go_back(char *pwd, t_mini *mini, int status);
void			ft_cd_no_args(char *home, char *pwd, t_mini *mini, int status);
void			ft_cd_with_minus(char *pwd, t_mini *mini, int status);
void			ft_cd_with_dash(t_node *node, t_mini *mini, char *home, \
				int *status);

//ECHO
void			ft_print_echo(t_node *node, int i);
void			ft_echo(t_node *node, t_mini *mini);
int				is_echo(t_node *cur, t_mini *mini);

//EXPORT
void			ft_export(t_node *node, t_mini *mini);
void			ft_zero_env(char **env);
int				var_accept(t_node *node);
int				already_exist(char *arg, t_mini *mini);
void			ft_replace_var(char **env, char *tofound, char *to_replace);
int				is_var(char *s);

//UNSET
void			ft_unset(t_mini *mini, t_node *node);
int				check_var(char *str);

//EXIT
void			ft_exit(t_mini *mini);

//PWD
void			ft_pwd(t_mini *mini);

//SIGNALS
void			handling_signals(int sig, siginfo_t *info, void *context);
void			ft_sig_init(t_mini *mini);
void			handling_sig_children(void);

//MAIN
void			init_args(t_mini *mini, int ac, char **envp);
void			shell_looping(t_mini *mini);

//PARSE
char			**tokenize(char *str);
char			**expand_vars(char **new_tokens, char **env_copy, t_mini *mini);
char			*expand_var(char *str, char **env_copy, t_mini *mini);
char			**slice_tokens(char **tokens, int i, int end);
void			parse_tokens(char **tokens, t_node **sliced_tokens_list);
int				find_pipe(char **tokens);
bool			check_for_unclosed_quotes(char *input);
int				semicolon_checker(char *str);
bool			checker_quotes(char *input);
void			final_sliced_list(t_node **sliced_tokens_list);
void			fill_node(t_node **sliced_tokens_list, t_mini *mini);

//REDIR
t_redir			*new_redirection(int type, char *target);

//EXEC
void			execute(t_node *sliced_tokens_list, t_mini *mini);
char			*path_find(char **envp, char *cmd);
int				open_redirs(t_node *node, t_mini *mini);
char			*get_path(char *cmd, char **env);
void			add_redirection(t_node *node, int type, char *target);
char			**filter(char **tokens);
void			expand_args(char **args, char **env_copy, t_mini *mini);
int				get_redir_type(char *token);
void			redir_cases(t_mini *mini);
//UTILS
void			ft_check_cmd(t_node *cur, t_mini *mini);
void			free_mini_things(t_node *node, t_mini *mini);
void			free_all(char **tokens, char *input);
void			free_args(char **args);
void			free_2_all(char **args1, char **args2);
void			ft_free_env(char **env);
void			cleanup_heredoc_files(t_mini *mini);
void			free_sliced(char **sliced, int count);
void			free_ast(t_node **sliced_tokens_list);
void			ft_close_all_fds(t_mini *mini);
void			ft_close(int fd);

//UTILS
int				here_doc(t_mini *mini, char *str);
void			heredoc_last(t_node **sliced_tokens_list);
t_pid			*main_pid(void);
void			add_redir_in(t_mini *mini);
void			add_redir_out(t_mini *mini);
void			add_redir_app(t_mini *mini);
bool			if_stats_input(t_mini *mini);

#endif
