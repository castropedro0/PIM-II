#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logica_atividade.h"
#include "logica_pessoa.h"
// Implementação das funções de Atividades

// RF01: Permitir o gerenciamento de atividades (Inserção)
void inserir_atividade(Atividade *atividades, int *total, const char *professor_cpf) {
    if (*total >= MAX_ATIVIDADES) {
        printf("\nERRO: Limite de atividades atingido.\n");
        return;
    }

    int i = *total;
    printf("\n--- Inserir Nova Atividade ---\n");

    // Geração de ID simples (apenas para este projeto)
    atividades[i].id = *total + 1;

    printf("Título da Atividade: ");
    scanf(" %[^\n]", atividades[i].titulo);
    printf("Descrição: ");
    scanf(" %[^\n]", atividades[i].descricao);

    // Associa a atividade ao professor logado
    strcpy(atividades[i].professor_cpf, professor_cpf);

    (*total)++;
    printf("Atividade '%s' cadastrada com sucesso! (ID: %d)\n", atividades[i].titulo, atividades[i].id);
}

// RF01: Permitir o gerenciamento de atividades (Listagem)
void listar_atividades(Atividade *atividades, int total) {
    printf("\n--- Lista de Todas as Atividades ---\n");
    if (total == 0) {
        printf("Nenhuma atividade cadastrada.\n");
        return;
    }

    // Estrutura de repetição (for - Requisito C)
    for (int i = 0; i < total; i++) {
        printf("\nID: %d\n", atividades[i].id);
        printf("Título: %s\n", atividades[i].titulo);
        printf("Descrição: %s\n", atividades[i].descricao);
        printf("Criada por CPF: %s\n", atividades[i].professor_cpf);
    }
}

// RF02: Consulta de atividades pelos alunos (Restrição de acesso)
void consultar_atividades_aluno(Atividade *atividades, int total) {
    printf("\n--- Minhas Atividades (Aluno) ---\n");
    // Alunos podem ver todas as atividades
    listar_atividades(atividades, total);
}


// Implementação da Persistência (Salvamento Único)
void salvar_dados_sistema(Pessoa *pessoas, int total_pessoas, Atividade *atividades, int total_atividades) {
    FILE *arq = fopen(NOME_ARQUIVO, "wb");
    if (arq == NULL) {
        perror("Erro ao salvar dados do sistema");
        return;
    }

    // Salva o total de Pessoas e as Pessoas
    fwrite(&total_pessoas, sizeof(int), 1, arq);
    fwrite(pessoas, sizeof(Pessoa), total_pessoas, arq);

    // Salva o total de Atividades e as Atividades
    fwrite(&total_atividades, sizeof(int), 1, arq);
    fwrite(atividades, sizeof(Atividade), total_atividades, arq);

    printf("\nDados de %d pessoas e %d atividades salvos com sucesso.\n", total_pessoas, total_atividades);
    fclose(arq);
}

// Implementação da Persistência (Carregamento Único)
void carregar_dados_sistema(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades) {
    FILE *arq = fopen(NOME_ARQUIVO, "rb");
    if (arq == NULL) {
        *total_pessoas = 0;
        *total_atividades = 0;
        return;
    }

    // Carrega Pessoas
    fread(total_pessoas, sizeof(int), 1, arq);
    if (*total_pessoas > MAX) *total_pessoas = MAX;
    fread(pessoas, sizeof(Pessoa), *total_pessoas, arq);

    // Carrega Atividades
    fread(total_atividades, sizeof(int), 1, arq);
    if (*total_atividades > MAX_ATIVIDADES) *total_atividades = MAX_ATIVIDADES;
    fread(atividades, sizeof(Atividade), *total_atividades, arq);

    printf("Dados de %d pessoas e %d atividades carregados.\n", *total_pessoas, *total_atividades);
    fclose(arq);
}
