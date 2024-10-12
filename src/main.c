
#include "../inc/minishell.h"

#define MAX_COMMAND_LENGTH 1024

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	(void)envp;
	char command[MAX_COMMAND_LENGTH];
    char cwd[1024];  // Para armazenar o diretório atual

    char *input;

    while (1) {
        // Obtém o diretório de trabalho atual
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
            return 1;
        }

        // Exibe o prompt com o diretório atual
        printf("%s> ", cwd);

        while ((input = readline("\0")) != NULL)
        {
        	if (*input)
         	{
          		add_history(input);
		 		break;
		 	}
          	free(input);
        }

        // Remove a nova linha no final do comando
        command[strcspn(command, "\n")] = 0;

        // Se o usuário digitar "exit", sair do shell
        if (strcmp(command, "exit") == 0)
        {
            printf("Saindo...\n");
            break;
        }

        // Executa o comando usando o sistema padrão (system)
        if (system(command) == -1) {
            perror("Erro ao executar o comando");
        }
    }

    return 0;
}
