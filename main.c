#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logica_pessoa.h"
#include "logica_atividade.h"

// Redefini��o de constantes para as op��es do menu
#define OP_SAIR 0
#define OP_LOGIN 1
#define OP_NOVO_CADASTRO 2
#define OP_LISTAR_PESSOAS 10
#define OP_EXCLUIR_PESSOA 11
#define OP_INSERIR_ATIVIDADE 20
#define OP_LISTAR_ATIVIDADES 21
#define OP_CONSULTAR_ATIVIDADES 22
#define OP_LOGOUT 99

int main() {
    Pessoa pessoas[MAX];
    int total_pessoas;

    Atividade atividades[MAX_ATIVIDADES];
    int total_atividades;

    // Carrega todos os dados ao iniciar (RF06)
    carregar_dados_sistema(pessoas, &total_pessoas, atividades, &total_atividades);

    Pessoa usuario_logado = {""};
    int logado = 0;
    int opcao;

    printf("\n============================================\n");
    printf(" SISTEMA ACADEMICO (PIM II)\n");
    printf("============================================\n");

    do {
        if (!logado) {
            // Menu de Login (Deslogado)
            printf("\n--- Menu Principal ---\n");
            printf("%d. Fazer Login\n", OP_LOGIN);
            printf("%d. Novo Cadastro (ADM/PROFESSOR/ALUNO)\n", OP_NOVO_CADASTRO);
            printf("%d. Sair e Salvar\n", OP_SAIR);
            printf("Escolha uma opcao: ");

            if (scanf("%d", &opcao) != 1) {
                while (getchar() != '\n');
                opcao = -1;
            }

            switch (opcao) {
                case OP_LOGIN:
                    logado = fazer_login(pessoas, total_pessoas, &usuario_logado);
                    break;
                case OP_NOVO_CADASTRO:
                    inserir(pessoas, &total_pessoas);
                    break;
                case OP_SAIR:
                    salvar_dados_sistema(pessoas, total_pessoas, atividades, total_atividades);
                    printf("Encerrando o programa...\n");
                    break;
                default:
                    printf("Opcao invalida.\n");
            }

        } else {
            // Menu de Acesso (Logado)
            printf("\n--- Menu de Acesso (%s) - Bem-vindo(a), %s ---\n", usuario_logado.role, usuario_logado.nome);

            // Op��es Comuns a todos
            printf("%d. Listar Pessoas (Todos)\n", OP_LISTAR_PESSOAS);

            // Estrutura de Decis�o (if/else - RNF02) para controle de acesso

            // ------------------ PERFIS: ADM -------------------
            if (strcmp(usuario_logado.role, ROLE_ADM) == 0) {
                printf("--- Opcoes ADM ---\n");
                printf("%d. Excluir Pessoa\n", OP_EXCLUIR_PESSOA);
            }

            // ------------------ PERFIS: PROFESSOR ---------------
            if (strcmp(usuario_logado.role, ROLE_PROFESSOR) == 0) {
                printf("--- Opcoes PROFESSOR ---\n");
                printf("%d. Inserir Nova Atividade\n", OP_INSERIR_ATIVIDADE);
            }

            // ------------------ PERFIS: ALUNO -------------------
            if (strcmp(usuario_logado.role, ROLE_ALUNO) == 0) {
                printf("--- Opcoes ALUNO ---\n");
                printf("%d. Consultar Atividades \n", OP_CONSULTAR_ATIVIDADES);
            }

            printf("%d. Fazer Logout\n", OP_LOGOUT);
            printf("%d. Sair e Salvar\n", OP_SAIR);
            printf("Escolha uma opcao: ");

            if (scanf("%d", &opcao) != 1) {
                while (getchar() != '\n');
                opcao = -1;
            }

            switch (opcao) {
                case OP_LISTAR_PESSOAS:
                    listar(pessoas, total_pessoas);
                    break;

                // --- Fun��es de ADM ---
                case OP_EXCLUIR_PESSOA:
                    if (strcmp(usuario_logado.role, ROLE_ADM) == 0) {
                        char cpf_excluir[15];
                        printf("\nDigite o CPF da pessoa a ser excluida: ");
                        scanf(" %[^\n]", cpf_excluir);
                        int index = buscar_pessoa(pessoas, total_pessoas, cpf_excluir);
                        if (index != -1) {
                            excluir_pessoa(pessoas, &total_pessoas, index);
                        } else {
                            printf("CPF nao encontrado.\n");
                        }
                    } else {
                        printf("Acesso negado.\n");
                    }
                    break;

                // --- Fun��es de PROFESSOR ---
                case OP_INSERIR_ATIVIDADE:
                    if (strcmp(usuario_logado.role, ROLE_PROFESSOR) == 0) {
                        inserir_atividade(atividades, &total_atividades, usuario_logado.cpf);
                    } else {
                        printf("Acesso negado. Apenas Professores podem inserir atividades.\n");
                    }
                    break;

                // --- Fun��es de ALUNO ---
                case OP_CONSULTAR_ATIVIDADES:
                    if (strcmp(usuario_logado.role, ROLE_ALUNO) == 0) {
                        consultar_atividades_aluno(atividades, total_atividades);
                    } else {
                        printf("Acesso negado.\n");
                    }
                    break;

                // --- Fun��es de Sistema ---
                case OP_LOGOUT:
                    logado = 0;
                    printf("Logout realizado com sucesso.\n");
                    break;
                case OP_SAIR:
                    salvar_dados_sistema(pessoas, total_pessoas, atividades, total_atividades);
                    printf("Encerrando o programa...\n");
                    break;
                default:
                    printf("Opcao invalida.\n");
            }
        }
    } while (opcao != OP_SAIR);

    return 0;
}
