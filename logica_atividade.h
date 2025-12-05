#ifndef LOGICA_ATIVIDADES_H
#define LOGICA_ATIVIDADES_H

#include "logica_pessoas.h"

#define MAX_ATIVIDADES 100
#define MAX_TURMAS 10
#define MAX_NOTAS 500
#define NOME_ARQUIVO "dados_sistema.txt"

// Estruturas - CHAVES ATUALIZADAS PARA EMAIL
typedef struct {
    char aluno_email[TAM_EMAIL];
    char atividade_titulo[100];
    float valor;
} Nota;

typedef struct {
    char nome[50];
    char professor_email[TAM_EMAIL];
    int total_alunos;
} Turma;

typedef struct {
    char titulo[100];
    char descricao[250];
    char professor_email[TAM_EMAIL];
    char data_entrega[11];
} Atividade;

// --- Protótipos das Funções de Notas (ADM/Professor/Aluno) ---
void consultar_notas(Nota *notas, int total_notas, Pessoa *usuario_logado);
void inserir_nota(Nota *notas, int *total_notas, Pessoa *usuario_logado);
void editar_nota(Nota *notas, int total_notas, Pessoa *usuario_logado);
void excluir_nota(Nota *notas, int *total_notas, Pessoa *usuario_logado);

// --- Protótipos de Turmas e Atividades (Implementação completa para evitar erro de Linker) ---
void inserir_atividade(Atividade *atividades, int *total_atividades, char *professor_email);
void consultar_atividades_aluno(Atividade *atividades, int total_atividades);
void inserir_turma(Turma *turmas, int *total_turmas);
void consultar_turma(Turma *turmas, int total_turmas, Pessoa *usuario_logado);
void gerar_relatorio();

// --- Protótipos de Persistência (TXT) ---
void salvar_dados_sistema(Pessoa *pessoas, int total_pessoas, Atividade *atividades, int total_atividades, Turma *turmas, int total_turmas, Nota *notas, int total_notas);
void carregar_dados_sistema(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas);

#endif // LOGICA_ATIVIDADES_H
