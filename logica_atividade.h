#ifndef LOGICA_ATIVIDADES_H
#define LOGICA_ATIVIDADES_H

#include "logica_pessoa.h"
#define MAX_ATIVIDADES 100

// Estrutura de Atividade/Aula (Sem Altera��o)
typedef struct
{
    int id;
    char titulo[100];
    char descricao[256];
    char professor_cpf[15];
} Atividade;

// Prot�tipos das Fun��es para Atividades
void inserir_atividade(Atividade *atividades, int *total, const char *professor_cpf);
void listar_atividades(Atividade *atividades, int total);
void consultar_atividades_aluno(Atividade *atividades, int total);

// Persist�ncia �nica (Fun��es que salvam TODOS os dados do sistema)
void salvar_dados_sistema(Pessoa *pessoas, int total_pessoas, Atividade *atividades, int total_atividades);
void carregar_dados_sistema(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades);

#endif // LOGICA_ATIVIDADES_H
