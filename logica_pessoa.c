#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include "logica_pessoas.h"

// Garante o idioma em português
void __attribute__((constructor)) init_locale_pessoas() {
    setlocale(LC_ALL, "");
}

// ----------------------------------------------------
// Função para ler a senha ocultando os caracteres com '*'
void ler_senha_oculta(char *senha, int max_len) {
    int i = 0;
    char ch;

    printf("Senha (máximo %d caracteres): ", max_len - 1);

    // O loop continua até que o buffer esteja cheio ou o usuário pressione Enter (\r)
    while (i < max_len - 1) {
        // Usa _getch() para ler o caractere sem ecoar (printar)
        ch = _getch();

        if (ch == '\r' || ch == '\n') { // Enter pressionado (fim da entrada)
            break;
        } else if (ch == 8 || ch == 127) { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Move o cursor para trás, apaga o caractere, move o cursor para trás novamente
            }
        } else {
            senha[i++] = ch;
            printf("*");
        }
    }
    senha[i] = '\0';
    printf("\n");
}
// ----------------------------------------------------

// Busca por EMAIL
int buscar_pessoa(Pessoa *pessoas, int total, char *email_busca) {
    for (int i = 0; i < total; i++) {
        if (strcmp(pessoas[i].email, email_busca) == 0) {
            return i;
        }
    }
    return -1;
}

// Cadastro de Pessoa
void inserir(Pessoa *pessoas, int *total, int permissao_total) {
    if (*total >= MAX_PESSOAS) {
        printf("\n!!! ERRO: LIMITE DE CADASTRO ATINGIDO !!!\n");
        return;
    }

    Pessoa nova_pessoa;
    char email_temp[TAM_EMAIL];

    printf("\n--- Novo Cadastro ---\n");
    printf("Nome: ");
    // Limpa o buffer antes de ler string com espaços
    if (scanf(" %[^\n]", nova_pessoa.nome) != 1) { return; }

    // Usa EMAIL como identificador único
    printf("E-mail (será seu login): ");
    if (scanf("%s", email_temp) != 1) { return; }

    if (buscar_pessoa(pessoas, *total, email_temp) != -1) {
        printf("!!! ERRO: E-mail já cadastrado. Não é possível continuar. !!!\n");
        return;
    }
    strcpy(nova_pessoa.email, email_temp);

    // Senha oculta
    ler_senha_oculta(nova_pessoa.senha, sizeof(nova_pessoa.senha));

    // Lógica de Perfil
    if (permissao_total) {
        // Opções completas para ADM logado
        int escolha_role;
        int role_valida = 0;

        do {
            printf("\nEscolha o Perfil (Role):\n");
            printf("1 - %s\n", ROLE_ADM);
            printf("2 - %s\n", ROLE_PROFESSOR);
            printf("3 - %s\n", ROLE_ALUNO);

            printf("Escolha: ");
            if (scanf("%d", &escolha_role) != 1) {
                while (getchar() != '\n');
                escolha_role = -1;
            }

            switch (escolha_role) {
                case 1: strcpy(nova_pessoa.role, ROLE_ADM); role_valida = 1; break;
                case 2: strcpy(nova_pessoa.role, ROLE_PROFESSOR); role_valida = 1; break;
                case 3: strcpy(nova_pessoa.role, ROLE_ALUNO); role_valida = 1; break;
                default: printf("Opção de perfil inválida. Tente novamente.\n");
            }
        } while (!role_valida);

    } else {
        // Somente ALUNO para cadastro deslogado
        strcpy(nova_pessoa.role, ROLE_ALUNO);
        printf("\nPerfil definido como: %s\n", ROLE_ALUNO);
    }

    pessoas[*total] = nova_pessoa;
    (*total)++;
    printf("\nCadastro de %s como %s realizado com sucesso!\n", nova_pessoa.nome, nova_pessoa.role);
}

// Login por EMAIL (substitui o CPF)
Pessoa fazer_login(Pessoa *pessoas, int total, char *perfil_logado) {
    char email_login[TAM_EMAIL];
    char senha_login[TAM_SENHA];
    int index = -1;

    Pessoa usuario_vazio = {"", "", "", ""};

    printf("\n--- Login ---\n");
    printf("Digite seu E-mail: ");
    if (scanf("%s", email_login) != 1) { return usuario_vazio; }

    // Senha oculta
    ler_senha_oculta(senha_login, sizeof(senha_login));

    index = buscar_pessoa(pessoas, total, email_login);

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
        printf("\nERRO: Usuário não encontrado. E-mail incorreto ou não cadastrado.\n");
        strcpy(perfil_logado, "");
        return usuario_vazio;
    }
}

void listar(Pessoa *pessoas, int total) {
    printf("\n--- Lista de Usuários (%d cadastrados) ---\n", total);
    for (int i = 0; i < total; i++) {
        printf("----------------------------------------\n");
        printf("Nome: %s\n", pessoas[i].nome);
        printf("E-mail (Login): %s\n", pessoas[i].email);
        printf("Perfil: %s\n", pessoas[i].role);
    }
    printf("----------------------------------------\n");
}

void excluir_pessoa(Pessoa *pessoas, int *total) {
    char email_busca[TAM_EMAIL];
    char confirmacao;

    printf("\n--- Excluir Usuário ---\n");
    printf("Digite o E-mail do usuário a ser excluído: ");
    if (scanf("%s", email_busca) != 1) { return; }

    int index = buscar_pessoa(pessoas, *total, email_busca);

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
        printf("Usuário com E-mail %s não encontrado.\n", email_busca);
    }
}


void promover_usuario(Pessoa *pessoas, int total) {
    char email_busca[TAM_EMAIL];
    int novo_perfil_escolha;

    printf("\n--- Promover/Alterar Perfil de Usuário ---\n");
    printf("Digite o E-mail do usuário a ser promovido: ");
    if (scanf("%s", email_busca) != 1) { return; }

    int index = buscar_pessoa(pessoas, total, email_busca);

    if (index != -1) {
        printf("Usuário encontrado: %s (Atual: %s)\n", pessoas[index].nome, pessoas[index].role);

        printf("Escolha o novo Perfil:\n1 - %s\n2 - %s\n3 - %s\nEscolha: ", ROLE_ADM, ROLE_PROFESSOR, ROLE_ALUNO);

        if (scanf("%d", &novo_perfil_escolha) != 1) {
            while (getchar() != '\n');
            novo_perfil_escolha = -1;
        }

        switch (novo_perfil_escolha) {
            case 1: strcpy(pessoas[index].role, ROLE_ADM); break;
            case 2: strcpy(pessoas[index].role, ROLE_PROFESSOR); break;
            case 3: strcpy(pessoas[index].role, ROLE_ALUNO); break;
            default: printf("Opção de perfil inválida. Nenhuma alteração feita.\n"); return;
        }

        printf("Perfil de %s alterado com sucesso para %s.\n", pessoas[index].nome, pessoas[index].role);
    } else {
        printf("Usuário com E-mail %s não encontrado.\n", email_busca);
    }
}
