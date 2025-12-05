#ifndef LOGICA_PESSOAS_H
#define LOGICA_PESSOAS_H

// Constantes
#define MAX_PESSOAS 100
#define TAM_NOME 50
#define TAM_EMAIL 50
#define TAM_SENHA 20
#define TAM_ROLE 15

// Constantes de Perfil (Role)
#define ROLE_ADM "ADM"
#define ROLE_PROFESSOR "PROFESSOR"
#define ROLE_ALUNO "ALUNO"

// Estrutura principal para armazenar dados de uma Pessoa
typedef struct {
    char nome[TAM_NOME];
    char email[TAM_EMAIL];
    char senha[TAM_SENHA];
    char role[TAM_ROLE]; // Perfil: ADM, PROFESSOR ou ALUNO
} Pessoa;

// --- Protótipos das Funções de Gestão de Pessoas ---
int buscar_pessoa(Pessoa *pessoas, int total, char *email_busca); // Busca por EMAIL
// permissao_total: 1 se ADM pode escolher perfil; 0 se é apenas ALUNO
void inserir(Pessoa *pessoas, int *total, int permissao_total);
Pessoa fazer_login(Pessoa *pessoas, int total, char *perfil_logado);
void listar(Pessoa *pessoas, int total);
void excluir_pessoa(Pessoa *pessoas, int *total);
void promover_usuario(Pessoa *pessoas, int total);

// Protótipo da função de leitura de senha oculta (ADAPTADO PARA WINDOWS)
void ler_senha_oculta(char *senha, int max_len);

#endif // LOGICA_PESSOAS_H
