#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // Para suporte a acentuação
#include "logica_pessoas.h"
#include "logica_atividades.h"

// Definições de Constantes para as Opções do Menu
#define OP_SAIR 0
#define OP_LOGIN 1
#define OP_NOVO_CADASTRO 2

// Opções ADM
#define OP_LISTAR_PESSOAS 10
#define OP_EXCLUIR_PESSOA 11
#define OP_PROMOVER_USUARIO 12
#define OP_INSERIR_TURMA 13
#define OP_EXCLUIR_NOTA 14

// Opções Professor/ADM de Notas
#define OP_INSERIR_ATIVIDADE 20
#define OP_INSERIR_NOTA 21 // LANÇAR/CRIAR
#define OP_EDITAR_NOTA 22  // EDITAR/ALTERAR

// Opções Comuns
#define OP_CONSULTAR_ATIVIDADES 23 // Aluno
#define OP_CONSULTAR_NOTA 24       // Todos
#define OP_GERAR_RELATORIO 30      // ADM e Professor
#define OP_CONSULTAR_TURMA 31      // Todos
#define OP_LOGOUT 99

int main() {
    // ----------------------------------------------------
    // INICIALIZAÇÃO DA BLIBLIOTECA LOCALE
    setlocale(LC_ALL, "");
    // ----------------------------------------------------

    Pessoa pessoas[MAX_PESSOAS];
    int total_pessoas = 0;

    Atividade atividades[MAX_ATIVIDADES];
    int total_atividades = 0;

    Turma turmas[MAX_TURMAS];
    int total_turmas = 0;

    Nota notas[MAX_NOTAS];
    int total_notas = 0;

    carregar_dados_sistema(pessoas, &total_pessoas, atividades, &total_atividades, turmas, &total_turmas, notas, &total_notas);

    Pessoa usuario_logado = {"", "", "", 0, "", "", ""};
    char perfil_logado[15] = "";
    int opcao;

    printf("\n\n******************************************\n");
    printf("         SISTEMA ACADÊMICO (PIM II)\n");
    printf("******************************************\n");

    do {
        printf("\n");
        if (strcmp(perfil_logado, "") == 0) { // Deslogado
            printf("--- Menu Principal (Deslogado) ---\n");
            printf("%d. Fazer Login\n", OP_LOGIN);
            printf("%d. Novo Cadastro (APENAS ALUNO)\n", OP_NOVO_CADASTRO);
            printf("%d. Sair e Salvar\n", OP_SAIR);
            printf("Escolha uma opção: ");

            if (scanf("%d", &opcao) != 1) {
                while (getchar() != '\n');
                opcao = -1;
            }

            switch (opcao) {
                case OP_LOGIN:
                    usuario_logado = fazer_login(pessoas, total_pessoas, perfil_logado);
                    break;
                case OP_NOVO_CADASTRO:
                    inserir(pessoas, &total_pessoas);
                    break;
                case OP_SAIR:
                    break;
                default:
                    printf("Opção inválida. Tente novamente.\n");
            }

        } else { // Logado
            printf("\n--- Menu de Acesso (%s) - Bem-vindo(a), %s ---\n", perfil_logado, usuario_logado.nome);

            // Menu ADM
            if (strcmp(perfil_logado, ROLE_ADM) == 0) {
                printf("--- Opções ADM ---\n");
                printf("%d. Listar Pessoas\n", OP_LISTAR_PESSOAS);
                printf("%d. Inserir Nova Turma\n", OP_INSERIR_TURMA);
                printf("%d. Excluir Pessoa\n", OP_EXCLUIR_PESSOA);
                printf("%d. Promover/Alterar Perfil de Usuário\n", OP_PROMOVER_USUARIO);
                printf("%d. Gerar Relatório\n", OP_GERAR_RELATORIO);
                printf("%d. Consultar Notas\n", OP_CONSULTAR_NOTA);
                printf("%d. LANÇAR Nota \n", OP_INSERIR_NOTA);
                printf("%d. EDITAR Nota \n", OP_EDITAR_NOTA);
                printf("%d. Excluir Nota \n", OP_EXCLUIR_NOTA);
            }

            // Menu PROFESSOR
            if (strcmp(perfil_logado, ROLE_PROFESSOR) == 0) {
                printf("--- Opções PROFESSOR ---\n");
                printf("%d. Inserir Nova Atividade\n", OP_INSERIR_ATIVIDADE);
                printf("%d. LANÇAR Nota \n", OP_INSERIR_NOTA);
                printf("%d. EDITAR Nota \n", OP_EDITAR_NOTA);
                printf("%d. Gerar Relatório\n", OP_GERAR_RELATORIO);
            }

            // Menu ALUNO
            if (strcmp(perfil_logado, ROLE_ALUNO) == 0) {
                printf("--- Opções ALUNO ---\n");
                printf("%d. Consultar Aulas e Atividades\n", OP_CONSULTAR_ATIVIDADES);
                printf("%d. Consultar Notas\n", OP_CONSULTAR_NOTA);
            }

            // Opções Comuns
            printf("--- Opções Comuns ---\n");
            printf("%d. Consultar Turma\n", OP_CONSULTAR_TURMA);

            printf("%d. Fazer Logout\n", OP_LOGOUT);
            printf("%d. Sair e Salvar\n", OP_SAIR);
            printf("Escolha uma opção: ");

            if (scanf("%d", &opcao) != 1) {
                while (getchar() != '\n');
                opcao = -1;
            }

            // Lógica do SWITCH/CASE (Permissões)
            switch (opcao) {
                // ... (ADM Pessoas) ...
                case OP_LISTAR_PESSOAS:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0) { listar(pessoas, total_pessoas); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;
                case OP_EXCLUIR_PESSOA:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0) { excluir_pessoa(pessoas, &total_pessoas); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;
                case OP_PROMOVER_USUARIO:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0) { promover_usuario(pessoas, total_pessoas); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;

                // Funções de Notas (Separadas)
                case OP_INSERIR_NOTA:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0 || strcmp(perfil_logado, ROLE_PROFESSOR) == 0) {
                        inserir_nota(notas, &total_notas, &usuario_logado);
                    } else {
                        printf("!!! ACESSO NEGADO. Apenas ADM ou Professor. !!!\n");
                    }
                    break;
                case OP_EDITAR_NOTA:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0 || strcmp(perfil_logado, ROLE_PROFESSOR) == 0) {
                        editar_nota(notas, total_notas, &usuario_logado);
                    } else {
                        printf("!!! ACESSO NEGADO. Apenas ADM ou Professor. !!!\n");
                    }
                    break;
                case OP_EXCLUIR_NOTA:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0) { excluir_nota(notas, &total_notas, &usuario_logado); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;

                // Funções de Turmas/Atividades
                case OP_INSERIR_TURMA:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0) { inserir_turma(turmas, &total_turmas); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;
                case OP_INSERIR_ATIVIDADE:
                    if (strcmp(perfil_logado, ROLE_PROFESSOR) == 0) { inserir_atividade(atividades, &total_atividades, usuario_logado.cpf); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;
                case OP_CONSULTAR_ATIVIDADES:
                    if (strcmp(perfil_logado, ROLE_ALUNO) == 0) { consultar_atividades_aluno(atividades, total_atividades); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;
                case OP_GERAR_RELATORIO:
                    if (strcmp(perfil_logado, ROLE_ADM) == 0 || strcmp(perfil_logado, ROLE_PROFESSOR) == 0) { gerar_relatorio(); } else { printf("!!! ACESSO NEGADO. !!!\n"); }
                    break;

                // Funções Comuns
                case OP_CONSULTAR_NOTA:
                    consultar_notas(notas, total_notas, &usuario_logado);
                    break;
                case OP_CONSULTAR_TURMA:
                    consultar_turma(turmas, total_turmas, &usuario_logado);
                    break;

                case OP_LOGOUT:
                    strcpy(perfil_logado, "");
                    break;
                case OP_SAIR:
                    break;
                default:
                    printf("Opção inválida. Tente novamente.\n");
            }
        }
    } while (opcao != OP_SAIR);

    salvar_dados_sistema(pessoas, total_pessoas, atividades, total_atividades, turmas, total_turmas, notas, total_notas);
    printf("\nDados salvos. Sistema encerrado. Tchau!\n");

    return 0;
}
