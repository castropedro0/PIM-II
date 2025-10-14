#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logica_pessoa.h"

// Função auxiliar para buscar uma pessoa e retornar o índice
// Retorna o índice (0 a total-1) ou -1 se não encontrar
int buscar_pessoa(Pessoa *pessoas, int total, char *cpf_busca) {
    for (int i = 0; i < total; i++) {
        if (strcmp(pessoas[i].cpf, cpf_busca) == 0) {
            return i;
        }
    }
    return -1;
}

// Função de Inserção (melhorada para incluir o Perfil)
void inserir(Pessoa *pessoas, int *total) {
    if (*total >= MAX) {
        printf("\nERRO: Limite de cadastro atingido.\n");
        return;
    }

    int i = *total;
    printf("\n--- Inserir Pessoa ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", pessoas[i].nome);
    printf("CPF: ");
    scanf(" %[^\n]", pessoas[i].cpf);

    // Verificação de CPF duplicado
    if (buscar_pessoa(pessoas, *total, pessoas[i].cpf) != -1) {
        printf("ERRO: CPF ja cadastrado.\n");
        return;
    }

    printf("Idade: ");
    scanf("%d", &pessoas[i].idade);
    printf("Telefone: ");
    scanf(" %[^\n]", pessoas[i].telefone);
    printf("Endereco: ");
    scanf(" %[^\n]", pessoas[i].endereco);

    // Escolha do Perfil (Novo Requisito)
    int perfil_opcao;
    do {
        printf("\nEscolha o Perfil:\n");
        printf("1. %s\n", ROLE_ADM);
        printf("2. %s\n", ROLE_PROFESSOR);
        printf("3. %s\n", ROLE_ALUNO);
        printf("Opção: ");
        scanf("%d", &perfil_opcao);

        // Estrutura de seleção (switch - Requisito Obrigatório)
        switch (perfil_opcao) {
            case 1: strcpy(pessoas[i].role, ROLE_ADM); break;
            case 2: strcpy(pessoas[i].role, ROLE_PROFESSOR); break;
            case 3: strcpy(pessoas[i].role, ROLE_ALUNO); break;
            default: printf("Opcao de perfil invalida. Tente novamente.\n");
        }
    } while (perfil_opcao < 1 || perfil_opcao > 3);

    (*total)++;
    printf("Pessoa '%s' cadastrada com sucesso como %s!\n", pessoas[i].nome, pessoas[i].role);
}

// Função de Listagem
void listar(Pessoa *pessoas, int total) {
    printf("\n--- Lista de Pessoas ---\n");
    for (int i = 0; i < total; i++) {
        printf("\nPessoa %d:\n", i + 1);
        printf("Nome: %s\n", pessoas[i].nome);
        printf("CPF: %s\n", pessoas[i].cpf);
        printf("Idade: %d\n", pessoas[i].idade);
        printf("Telefone: %s\n", pessoas[i].telefone);
        printf("Endereço: %s\n", pessoas[i].endereco);
        printf("Perfil: %s\n", pessoas[i].role); // Novo campo
    }
    if (total == 0) {
        printf("Nenhuma pessoa cadastrada.\n");
    }
}

// Função de Exclusão (Requer permissão ADM)
void excluir_pessoa(Pessoa *pessoas, int *total, int index) {
    if (index >= 0 && index < *total) {
        printf("\nExcluindo: %s (CPF: %s)\n", pessoas[index].nome, pessoas[index].cpf);

        // Desloca os elementos para preencher o espaço (Requisito Estrutura de Dados)
        for (int i = index; i < *total - 1; i++) {
            pessoas[i] = pessoas[i + 1];
        }
        (*total)--;
        printf("Pessoa excluida com sucesso!\n");
    } else {
        printf("Erro ao excluir: indice invalido.\n");
    }
}

// Funções de Persistência (Requisitos C: Salvar e Ler em Arquivo)

void salvar_dados(Pessoa *pessoas, int total) {
    FILE *arq = fopen(NOME_ARQUIVO, "wb"); // 'wb' = escrita binária
    if (arq == NULL) {
        perror("Erro ao salvar dados");
        return;
    }
    fwrite(&total, sizeof(int), 1, arq); // Salva o total primeiro
    if (fwrite(pessoas, sizeof(Pessoa), total, arq) != total) {
        printf("Aviso: Nem todos os dados foram salvos corretamente.\n");
    } else {
        printf("\nDados salvos com sucesso no arquivo '%s'.\n", NOME_ARQUIVO);
    }
    fclose(arq);
}

int carregar_dados(Pessoa *pessoas) {
    FILE *arq = fopen(NOME_ARQUIVO, "rb"); // 'rb' = leitura binária
    if (arq == NULL) {
        return 0; // Arquivo não existe ou não pode ser aberto
    }

    int total = 0;
    fread(&total, sizeof(int), 1, arq); // Lê o total

    // Garante que o total lido não exceda o tamanho do array
    if (total > MAX) total = MAX;

    fread(pessoas, sizeof(Pessoa), total, arq); // Lê os dados

    printf("\nDados de %d pessoas carregados do arquivo '%s'.\n", total, NOME_ARQUIVO);

    fclose(arq);
    return total;
}

// Funções de Acesso (Login e Menu)

// Simula o login buscando por CPF e retornando o usuário logado
int fazer_login(Pessoa *pessoas, int total, Pessoa *usuario_logado) {
    char cpf_busca[15];
    printf("\n--- Login (Busca por CPF) ---\n");
    printf("Digite o seu CPF: ");
    scanf(" %[^\n]", cpf_busca);

    int index = buscar_pessoa(pessoas, total, cpf_busca);

    // Estrutura de decisão para verificar se o usuário existe
    if (index != -1) {
        // Copia a struct da pessoa encontrada para a variável 'usuario_logado'
        *usuario_logado = pessoas[index];
        printf("\nLogin efetuado com sucesso! Perfil: %s\n", usuario_logado->role);
        return 1; // Sucesso
    } else {
        printf("Erro: CPF nao encontrado. Acesso negado.\n");
        return 0; // Falha
    }
}
