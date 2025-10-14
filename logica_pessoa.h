#ifndef LOGICA_PESSOAS_H
#define LOGICA_PESSOAS_H

// Capacidade máxima de usuários
#define MAX_PESSOAS 100

// Constantes de Perfil (Role)
#define ROLE_ADM "ADM"
#define ROLE_PROFESSOR "PROFESSOR"
#define ROLE_ALUNO "ALUNO"

// Estrutura principal para armazenar dados de uma Pessoa
typedef struct {
    char nome[50];
    char cpf[15];
    char senha[20];
    int idade;
    char telefone[15];
    char endereco[100];
    char role[15]; // Perfil: ADM, PROFESSOR ou ALUNO
} Pessoa;

// --- Protótipos das Funções de Gestão de Pessoas ---
int buscar_pessoa(Pessoa *pessoas, int total, char *cpf_busca);
void inserir(Pessoa *pessoas, int *total);
Pessoa fazer_login(Pessoa *pessoas, int total, char *perfil_logado);
void listar(Pessoa *pessoas, int total);
void excluir_pessoa(Pessoa *pessoas, int *total);
void promover_usuario(Pessoa *pessoas, int total);

#endif // LOGICA_PESSOAS_H
