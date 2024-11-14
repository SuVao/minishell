
#include "../inc/minishell.h"

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


bool	if_stats_input(t_mini *mini)
{	//false para break //true para continue
	if (!mini->line)
	{
		printf("exit\n");
		ft_exit(mini);
		return (false);
	}
	if (*mini->line)
	{
		add_history(mini->line);
		if (ft_strcmp(mini->line, "exit") == 0)
		{
			printf("exit\n");
			ft_exit(mini);
			return (false);
		}
	}
	return (true);
}

void	init_mini(t_mini *mini)
{
	mini->new_tokens = NULL;
	mini->exp_tokens = NULL;
	mini->env = NULL;
	mini->line = NULL;
	mini->args = NULL;
	mini->shlvl = 0;
}

/* void print_redirections(t_redirection *redir)
{
    while (redir != NULL)
    {
        // Print the redirection type and target file
        if (redir->type == REDIR_IN)
            printf("Redirection: INPUT from %s\n", redir->target);
        else if (redir->type == REDIR_OUT)
            printf("Redirection: OUTPUT to %s\n", redir->target);
        else if (redir->type == REDIR_APPEND)
            printf("Redirection: APPEND to %s\n", redir->target);
        else if (redir->type == HEREDOC)
            printf("Redirection: HEREDOC to %s\n", redir->target);

        // Move to the next redirection
        redir = redir->next;
    }
}

// Function to print an individual AST node
void print_ast_node(t_ast_node *node)
{
    if (node == NULL)
        return;

    // Print the current node type
    if (node->type == PIPE)
    {
        printf("PIPE\n");
    }
    else if (node->type == CMD)
    {
        printf("CMD: %s\n", node->cmd);
        if (node->args != NULL)
        {
            printf("Arguments: ");
            for (int i = 0; node->args[i] != NULL; i++)
            {
                printf("%s ", node->args[i]);
            }
            printf("\n");
        }
    }

    // Print redirection information if present
    if (node->redirs != NULL)
    {
        print_redirections(node->redirs);
    }

    // Recursively print left and right children if they exist
    if (node->left != NULL)
    {
        printf("Left child of %s:\n", node->cmd);
        print_ast_node(node->left);
    }
    if (node->right != NULL)
    {
        printf("Right child of %s:\n", node->cmd);
        print_ast_node(node->right);
    }
}

// Wrapper function to start the printing from the root of the AST
void print_ast(t_ast_node *root)
{
    print_ast_node(root);
} */
void free_token_list(t_token_node *head) {
    t_token_node *current = head;
    t_token_node *next;

    while (current != NULL) {
        next = current->next;  // Save the next node

        // Free the tokens array
        for (int i = 0; current->tokens[i] != NULL; i++) {
            free(current->tokens[i]);
			current->tokens[i] = NULL;  // Free each string in the tokens array
        }
        free(current->tokens);
		current->tokens = NULL;  // Finally, free the array of tokens itself

        free(current);
		current = NULL;  // Free the current node
        current = next;  // Move to the next node
    }
}

void	shell_looping(t_mini *mini, t_ast_node *ast_root)
{
	t_token_node *sliced_tokens_list;
	(void)ast_root;
	while (1)
	{
		mini->line = readline("minishell> ");
		if (!if_stats_input(mini))
			break;
		if (!checker_quotes(mini->line))
			continue;
		mini->new_tokens = tokenize(mini->line);
		if (!mini->new_tokens)
		{
    		free(mini->line);
    		write(2, "Error in tokenize", 18);
    		continue;
		}
		mini->exp_tokens = expand_vars(mini->new_tokens, mini->env);
		if (!mini->exp_tokens)
		{
			free_all(mini->new_tokens, mini->line);
			continue;
		}
		sliced_tokens_list = NULL;
		ast_root = parse_tokens(mini->exp_tokens, &sliced_tokens_list);
		/* print_ast(ast_root); */

		execute_ast(ast_root, mini);
		free_token_list(sliced_tokens_list);
		free_ast(ast_root);
		free_all(mini->exp_tokens, mini->line);
	}
}


int main(int ac, char **av, char **env)
{
	t_ast_node *ast_root;
	t_mini		*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (0);
	(void)ac;
	(void)av;
	init_mini(mini);
	ast_root = NULL;
	if (!env)
   		init_myown_envp(mini);
    else
   		init_envp(mini, env);
	shell_looping(mini, ast_root);
	ft_close_all_fds(mini);
	free_ast(ast_root);
	free(mini);
	return (0);
}
