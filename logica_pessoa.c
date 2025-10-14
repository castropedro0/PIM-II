#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> // Para suporte a acentuação
#include "logica_pessoas.h"

// Inicialização da biblioteca locale (garante acentuação mesmo se não for chamado no main)
void __attribute__((constructor)) init_locale_pessoas() {
    setlocale(LC_ALL, "");
}

// Funcao auxiliar para buscar uma pessoa
int buscar_pessoa(Pessoa *pessoas, int total, char *cpf_busca) {
    for (int i = 0; i < total; i++) {
        if (strcmp(pessoas[i].cpf, cpf_busca) == 0) {
            return i;
        }
    }
    return -1;
}

// Funcao de Inserir Novo Usuário
void inserir(Pessoa *pessoas, int *total) {
    if (*total >= MAX_PESSOAS) {
        printf("\n!!! ERRO: LIMITE DE CADASTRO ATINGIDO !!!\n");
        return;
    }

    Pessoa nova_pessoa;

    printf("\n--- Novo Cadastro ---\n");
    printf("Nome: "); scanf(" %[^\n]", nova_pessoa.nome);
    printf("CPF: "); scanf("%s", nova_pessoa.cpf);

    if (buscar_pessoa(pessoas, *total, nova_pessoa.cpf) != -1) {
        printf("!!! ERRO: CPF já cadastrado. Não é possível continuar. !!!\n");
        return;
    }

    printf("Senha (max 19 caracteres): "); scanf("%s", nova_pessoa.senha);
    printf("Idade: "); scanf("%d", &nova_pessoa.idade);
    printf("Telefone: "); scanf(" %[^\n]", nova_pessoa.telefone);
    printf("Endereço: "); scanf(" %[^\n]", nova_pessoa.endereco);

    int escolha_role;
    int role_valida = 0;

    do {
        printf("\nEscolha o Perfil (Role):\n");
        printf("3 - %s (Padrão para Novos Cadastros)\n", ROLE_ALUNO);

        if (*total == 0) {
            printf("1 - %s (Inicializador do Sistema)\n", ROLE_ADM);
            printf("2 - %s\n", ROLE_PROFESSOR);
        }

        printf("Escolha: ");
        if (scanf("%d", &escolha_role) != 1) {
            while (getchar() != '\n');
            escolha_role = -1;
        }

        switch (escolha_role) {
            case 1:
                if (*total == 0) {
                    strcpy(nova_pessoa.role, ROLE_ADM); role_valida = 1;
                } else { printf("Opção inválida. Use a função de promoção.\n"); }
                break;
            case 2:
                if (*total == 0) {
                    strcpy(nova_pessoa.role, ROLE_PROFESSOR); role_valida = 1;
                } else { printf("Opção inválida. Use a função de promoção.\n"); }
                break;
            case 3:
                strcpy(nova_pessoa.role, ROLE_ALUNO); role_valida = 1;
                break;
            default:
                printf("Opção de perfil inválida. Tente novamente.\n");
        }
    } while (!role_valida);

    pessoas[*total] = nova_pessoa;
    (*total)++;
    printf("\nCadastro de %s como %s realizado com sucesso!\n", nova_pessoa.nome, nova_pessoa.role);
}

// Implementação da função de login
Pessoa fazer_login(Pessoa *pessoas, int total, char *perfil_logado) {
    char cpf_login[15];
    char senha_login[20];
    int index = -1;

    Pessoa usuario_vazio = {"", "", "", 0, "", "", ""};

    printf("\n--- Login ---\n");
    printf("Digite seu CPF: "); scanf("%s", cpf_login);
    printf("Digite sua Senha: "); scanf("%s", senha_login);

    index = buscar_pessoa(pessoas, total, cpf_login);

    if (index != -1) {
        if (strcmp(pessoas[index].senha, senha_login) == 0) {
            strcpy(perfil_logado, pessoas[index].role);
            printf("\nLogin bem-sucedido! Bem-vindo(a), %s.\n", pessoas[index].nome);
            return pessoas[index];
        } else {
            printf("\nERRO: Senha incorreta.\n");
            strcpy(perfil_logado, "");
            return usuario_vazio;
        }
    } else {
        printf("\nERRO: Usuário não encontrado. CPF incorreto ou não cadastrado.\n");
        strcpy(perfil_logado, "");
        return usuario_vazio;
    }
}

// Funcao de listar (ADM)
void listar(Pessoa *pessoas, int total) {
    printf("\n--- Lista de Usuários (%d cadastrados) ---\n", total);
    for (int i = 0; i < total; i++) {
        printf("----------------------------------------\n");
        printf("Nome: %s\n", pessoas[i].nome);
        printf("CPF: %s\n", pessoas[i].cpf);
        printf("Perfil: %s\n", pessoas[i].role);
    }
    printf("----------------------------------------\n");
}

// Funcao de exclusao (ADM)
void excluir_pessoa(Pessoa *pessoas, int *total) {
    char cpf_busca[15];
    char confirmacao;

    printf("\n--- Excluir Usuário (ADM) ---\n");
    printf("Digite o CPF do usuário a ser excluído: "); scanf("%s", cpf_busca);

    int index = buscar_pessoa(pessoas, *total, cpf_busca);

    if (index != -1) {
        printf("Usuário %s (%s) encontrado. Excluir? (S/N): ", pessoas[index].nome, pessoas[index].role);
        scanf(" %c", &confirmacao);

        if (confirmacao == 'S' || confirmacao == 's') {
            for (int i = index; i < *total - 1; i++) {
                pessoas[i] = pessoas[i+1];
            }
            (*total)--;
            printf("Usuário excluído com sucesso.\n");
        } else {
            printf("Operação de exclusão cancelada.\n");
        }
    } else {
        printf("Usuário com CPF %s não encontrado.\n", cpf_busca);
    }
}

// Funcao de promocao (ADM)
void promover_usuario(Pessoa *pessoas, int total) {
    char cpf_busca[15];
    int novo_perfil_escolha;

    printf("\n--- Promover/Alterar Perfil de Usuário (ADM) ---\n");
    printf("Digite o CPF do usuário a ser promovido: "); scanf("%s", cpf_busca);

    int index = buscar_pessoa(pessoas, total, cpf_busca);

    if (index != -1) {
        printf("Usuário encontrado: %s (Atual: %s)\n", pessoas[index].nome, pessoas[index].role);

        printf("Escolha o novo Perfil:\n1 - %s\n2 - %s\n3 - %s\nEscolha: ", ROLE_ADM, ROLE_PROFESSOR, ROLE_ALUNO);

        if (scanf("%d", &novo_perfil_escolha) != 1) {
            while (getchar() != '\n'); novo_perfil_escolha = -1;
        }

        switch (novo_perfil_escolha) {
            case 1: strcpy(pessoas[index].role, ROLE_ADM); break;
            case 2: strcpy(pessoas[index].role, ROLE_PROFESSOR); break;
            case 3: strcpy(pessoas[index].role, ROLE_ALUNO); break;
            default: printf("Opção de perfil inválida. Nenhuma alteração feita.\n"); return;
        }

        printf("Perfil de %s alterado com sucesso para %s.\n", pessoas[index].nome, pessoas[index].role);
    } else {
        printf("Usuário com CPF %s não encontrado.\n", cpf_busca);
    }
}
