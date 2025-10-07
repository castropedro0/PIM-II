#ifndef LOGICA_PESSOAS_H
#define LOGICA_PESSOAS_H

#include <stdio.h>

#define MAX 100
// Nome único para salvar Pessoas e Atividades
#define NOME_ARQUIVO "dados_sistema.dat"

// Definição dos perfis
#define ROLE_ADM "ADM"
#define ROLE_PROFESSOR "PROFESSOR"
#define ROLE_ALUNO "ALUNO"

// Estrutura de Pessoa (Sem Alteração)
typedef struct {
    char nome[50];
    char cpf[15];
    int idade;
    char telefone[15];
    char endereco[100];
    char role[15];
} Pessoa;

// Protótipos das Funções (Sem Alteração)
void inserir(Pessoa *pessoas, int *total);
void listar(Pessoa *pessoas, int total);
int buscar_pessoa(Pessoa *pessoas, int total, char *cpf_busca);
void excluir_pessoa(Pessoa *pessoas, int *total, int index);
int fazer_login(Pessoa *pessoas, int total, Pessoa *usuario_logado);

#endif // LOGICA_PESSOAS_H
