#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "logica_pessoas.h"
#include "logica_atividades.h"

// Definições de Opções
#define OP_SAIR 0
#define OP_LOGIN 1
#define OP_NOVO_CADASTRO 2
#define OP_LOGOUT 10

// Funções Auxiliares para Menus
int menu_adm(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado, char *perfil_logado);
int menu_professor(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado, char *perfil_logado);
int menu_aluno(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado, char *perfil_logado);

// Funções de Subpáginas
void submenu_gestao_pessoas(Pessoa *pessoas, int *total_pessoas, Pessoa *usuario_logado);
void submenu_gestao_notas(Pessoa *pessoas, int total_pessoas, Atividade *atividades, int total_atividades, Turma *turmas, int total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado);


int main() {
    setlocale(LC_ALL, "");

    Pessoa pessoas[MAX_PESSOAS];
    int total_pessoas = 0;
    Atividade atividades[MAX_ATIVIDADES];
    int total_atividades = 0;
    Turma turmas[MAX_TURMAS];
    int total_turmas = 0;
    Nota notas[MAX_NOTAS];
    int total_notas = 0;
    int codigo_acao = 1;
    int opcao;

    carregar_dados_sistema(pessoas, &total_pessoas, atividades, &total_atividades, turmas, &total_turmas, notas, &total_notas);

    // ADM Master "Gui123"
    if (total_pessoas == 0) {
        Pessoa adm_master = {"ADM Master (Inicial)", "Gui123", "Gui123", 99, "999999999", "Sistema", ROLE_ADM};
        pessoas[0] = adm_master;
        total_pessoas = 1;
        printf("\n[ATENÇÃO] ADM Master 'Gui123' (senha: Gui123) criado para inicialização. Por favor, crie outro ADM após o login.\n");
    }

    Pessoa usuario_logado = {"", "", "", 0, "", "", ""};
    char perfil_logado[15] = "";

    printf("\n\n******************************************\n");
    printf("         SISTEMA ACADÊMICO (PIM II)\n");
    printf("******************************************\n");

    do {
        if (strcmp(perfil_logado, "") == 0) { // Deslogado
            printf("\n--- Menu Principal ---\n");
            printf("1. Fazer Login\n");
            printf("2. Novo Cadastro \n");
            printf("0. Sair e Salvar\n");
            printf("Escolha uma opção: ");

            if (scanf("%d", &opcao) != 1) {
                while (getchar() != '\n');
                opcao = -1;
            }

            switch (opcao) {
                case OP_LOGIN:
                    usuario_logado = fazer_login(pessoas, total_pessoas, perfil_logado);
                    codigo_acao = 1;
                    break;
                case OP_NOVO_CADASTRO:
                    inserir(pessoas, &total_pessoas, 0); // PERMISSÃO 0: APENAS ALUNO (RESOLVIDO)
                    codigo_acao = 1;
                    break;
                case OP_SAIR:
                    codigo_acao = 0;
                    break;
                default:
                    printf("Opção inválida. Tente novamente.\n");
                    codigo_acao = 1;
            }

        } else { // Logado

            if (strcmp(perfil_logado, ROLE_ADM) == 0) {
                codigo_acao = menu_adm(pessoas, &total_pessoas, atividades, &total_atividades, turmas, &total_turmas, notas, &total_notas, &usuario_logado, perfil_logado);
            } else if (strcmp(perfil_logado, ROLE_PROFESSOR) == 0) {
                codigo_acao = menu_professor(pessoas, &total_pessoas, atividades, &total_atividades, turmas, &total_turmas, notas, &total_notas, &usuario_logado, perfil_logado);
            } else if (strcmp(perfil_logado, ROLE_ALUNO) == 0) {
                codigo_acao = menu_aluno(pessoas, &total_pessoas, atividades, &total_atividades, turmas, &total_turmas, notas, &total_notas, &usuario_logado, perfil_logado);
            }

            if (codigo_acao == OP_LOGOUT) {
                strcpy(perfil_logado, "");
                usuario_logado = (Pessoa){"", "", "", 0, "", "", ""};
                codigo_acao = 1; // Volta ao menu deslogado
            } else if (codigo_acao == OP_SAIR) {
                // Sai do loop principal
            } else {
                codigo_acao = 1; // Continua logado
            }
        }
    } while (codigo_acao != OP_SAIR);

    salvar_dados_sistema(pessoas, total_pessoas, atividades, total_atividades, turmas, total_turmas, notas, total_notas);
    printf("\nDados salvos. Sistema encerrado. Tchau!\n");

    return 0;
}

// ----------------------------------------------------
// IMPLEMENTAÇÃO DOS SUBMENUS
// ----------------------------------------------------

void submenu_gestao_pessoas(Pessoa *pessoas, int *total_pessoas, Pessoa *usuario_logado) {
    int opcao;
    do {
        printf("\n--- Submenu: Gestão de Pessoas (ADM) ---\n");
        printf("1. Listar Todas as Pessoas\n");
        printf("2. Excluir Pessoa\n");
        printf("3. Promover/Alterar Perfil de Usuário\n");
        printf("4. Inserir Novo Usuário\n"); // MOVIMENTADO
        printf("0. Voltar ao Menu ADM\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) { while (getchar() != '\n'); opcao = -1; }

        switch (opcao) {
            case 1: listar(pessoas, *total_pessoas); break;
            case 2: excluir_pessoa(pessoas, total_pessoas); break;
            case 3: promover_usuario(pessoas, *total_pessoas); break;
            case 4: inserir(pessoas, total_pessoas, 1); break;
            case 0: break;
            default: printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

void submenu_gestao_notas(Pessoa *pessoas, int total_pessoas, Atividade *atividades, int total_atividades, Turma *turmas, int total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado) {
    int opcao;
    char *role = usuario_logado->role;
    int is_adm_prof = (strcmp(role, ROLE_ADM) == 0 || strcmp(role, ROLE_PROFESSOR) == 0);
    int is_adm = (strcmp(role, ROLE_ADM) == 0);

    do {
        printf("\n--- Submenu: Gestão de Notas (%s) ---\n", role);

        if (is_adm_prof) {
            printf("2. Lançar Nota \n");
            printf("3. Editar Nota \n");
            if (is_adm) {
                printf("4. Excluir Nota \n");
            }
        }

        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) { while (getchar() != '\n'); opcao = -1; }

        if (opcao == 1) {
            consultar_notas(notas, *total_notas, usuario_logado);
        } else if (opcao == 2 && is_adm_prof) {
            inserir_nota(notas, total_notas, usuario_logado);
        } else if (opcao == 3 && is_adm_prof) {
            editar_nota(notas, *total_notas, usuario_logado);
        } else if (opcao == 4 && is_adm) {
            excluir_nota(notas, total_notas, usuario_logado);
        } else if (opcao == 0) {
            break;
        } else {
            printf("Opção inválida ou acesso negado. Tente novamente.\n");
        }
    } while (opcao != 0);
}


// ----------------------------------------------------
// IMPLEMENTAÇÃO DOS MENUS PRINCIPAIS POR PERFIL
// ----------------------------------------------------

int menu_adm(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado, char *perfil_logado) {
    int opcao;
    printf("\n--- Menu ADM - Bem-vindo(a), %s ---\n", usuario_logado->nome);

    while (1) {
        printf("1. Gerenciar Pessoas \n");
        printf("2. Gerenciar Notas \n");
        printf("3. Inserir Nova Turma\n");
        printf("4. Consultar Turma\n");
        printf("5. Gerar Relatório\n");
        printf("10. Fazer Logout\n");
        printf("0. Sair e Salvar\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) { while (getchar() != '\n'); opcao = -1; }

        switch (opcao) {
            case 1: submenu_gestao_pessoas(pessoas, total_pessoas, usuario_logado); break;
            case 2: submenu_gestao_notas(pessoas, *total_pessoas, atividades, *total_atividades, turmas, *total_turmas, notas, total_notas, usuario_logado); break;
            case 3: inserir_turma(turmas, total_turmas); break;
            case 4: consultar_turma(turmas, *total_turmas, usuario_logado); break;
            case 5: gerar_relatorio(); break;
            case OP_LOGOUT: return OP_LOGOUT;
            case OP_SAIR: return OP_SAIR;
            default: printf("Opção inválida. Tente novamente.\n");
        }
    }
}

int menu_professor(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado, char *perfil_logado) {
    int opcao;
    printf("\n--- Menu PROFESSOR - Bem-vindo(a), %s ---\n", usuario_logado->nome);

    while (1) {
        printf("1. Gerenciar Notas \n");
        printf("2. Inserir Nova Atividade\n");
        printf("3. Consultar Turma\n");
        printf("4. Gerar Relatório\n");
        printf("10. Fazer Logout\n");
        printf("0. Sair e Salvar\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) { while (getchar() != '\n'); opcao = -1; }

        switch (opcao) {
            case 1: submenu_gestao_notas(pessoas, *total_pessoas, atividades, *total_atividades, turmas, *total_turmas, notas, total_notas, usuario_logado); break;
            case 2: inserir_atividade(atividades, total_atividades, usuario_logado->cpf); break;
            case 3: consultar_turma(turmas, *total_turmas, usuario_logado); break;
            case 4: gerar_relatorio(); break;
            case OP_LOGOUT: return OP_LOGOUT;
            case OP_SAIR: return OP_SAIR;
            default: printf("Opção inválida. Tente novamente.\n");
        }
    }
}

int menu_aluno(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas, Pessoa *usuario_logado, char *perfil_logado) {
    int opcao;
    printf("\n--- Menu ALUNO - Bem-vindo(a), %s ---\n", usuario_logado->nome);

    while (1) {
        printf("1. Consultar Notas\n");
        printf("2. Consultar Aulas e Atividades\n");
        printf("3. Consultar Turma\n");
        printf("10. Fazer Logout\n");
        printf("0. Sair e Salvar\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) { while (getchar() != '\n'); opcao = -1; }

        switch (opcao) {
            case 1: consultar_notas(notas, *total_notas, usuario_logado); break;
            case 2: consultar_atividades_aluno(atividades, *total_atividades); break;
            case 3: consultar_turma(turmas, *total_turmas, usuario_logado); break;
            case OP_LOGOUT: return OP_LOGOUT;
            case OP_SAIR: return OP_SAIR;
            default: printf("Opção inválida. Tente novamente.\n");
        }
    }
}
