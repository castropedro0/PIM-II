#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h> // Para suporte a acentuação
#include "logica_atividades.h"
#include "logica_pessoas.h"

// Inicialização da biblioteca locale
void __attribute__((constructor)) init_locale_atividades() {
    setlocale(LC_ALL, "");
}

// Auxiliar: Busca uma nota pelo CPF do aluno e título da atividade
int buscar_nota(Nota *notas, int total, char *aluno_cpf, char *atividade_titulo) {
    for (int i = 0; i < total; i++) {
        if (strcmp(notas[i].aluno_cpf, aluno_cpf) == 0 && strcmp(notas[i].atividade_titulo, atividade_titulo) == 0) {
            return i;
        }
    }
    return -1;
}

// --- FUNÇÕES DE NOTAS (LANÇAR E EDITAR SEPARADAS) ---

// NOVO: Permissão: ADM e Professor - Lançar/Criar nota
void inserir_nota(Nota *notas, int *total_notas, Pessoa *usuario_logado) {

    if (strcmp(usuario_logado->role, ROLE_ALUNO) == 0) {
        printf("\n!!! ACESSO NEGADO. Apenas ADM ou Professor podem Lançar notas. !!!\n");
        return;
    }

    if (*total_notas >= MAX_NOTAS) {
        printf("\n!!! ERRO: Limite máximo de notas no sistema atingido. !!!\n");
        return;
    }

    Nota nova_nota;
    printf("\n--- Lançar Nova Nota (ADM/PROFESSOR) ---\n");

    printf("CPF do Aluno que receberá a nota: ");
    scanf("%s", nova_nota.aluno_cpf);

    printf("Título da Atividade: ");
    scanf(" %[^\n]", nova_nota.atividade_titulo);

    if (buscar_nota(notas, *total_notas, nova_nota.aluno_cpf, nova_nota.atividade_titulo) != -1) {
        printf("\n!!! ERRO: Já existe uma nota para esta atividade e aluno. Use a opção EDITAR. !!!\n");
        return;
    }

    printf("Valor da Nota (Ex: 8.5): ");
    if (scanf("%f", &nova_nota.valor) != 1 || nova_nota.valor < 0 || nova_nota.valor > 10) {
        printf("!!! ERRO: Valor da nota inválido. Cancelando operação. !!!\n");
        while (getchar() != '\n');
        return;
    }

    notas[*total_notas] = nova_nota;
    (*total_notas)++;
    printf("\nNota %.2f lançada com sucesso para o aluno %s.\n", nova_nota.valor, nova_nota.aluno_cpf);
}

// AJUSTADO: Permissão: ADM e Professor - Alterar/Modificar nota
void editar_nota(Nota *notas, int total_notas, Pessoa *usuario_logado) {

    if (strcmp(usuario_logado->role, ROLE_ALUNO) == 0) {
        printf("\n!!! ACESSO NEGADO. Apenas ADM ou Professor podem editar notas. !!!\n");
        return;
    }

    char aluno_cpf_busca[15];
    char atividade_titulo_busca[100];
    float nova_nota;

    printf("\n--- Editar/Alterar Nota Existente (ADM/PROFESSOR) ---\n");
    printf("CPF do Aluno: ");
    scanf("%s", aluno_cpf_busca);
    printf("Título da Atividade: ");
    scanf(" %[^\n]", atividade_titulo_busca);

    int index = buscar_nota(notas, total_notas, aluno_cpf_busca, atividade_titulo_busca);

    if (index != -1) {
        printf("Nota atual para Atividade '%s': %.2f\n", atividade_titulo_busca, notas[index].valor);
        printf("Digite a NOVA nota: ");

        if (scanf("%f", &nova_nota) != 1 || nova_nota < 0 || nova_nota > 10) {
            printf("!!! ERRO: Valor da nota inválido. Nenhuma alteração feita. !!!\n");
            while (getchar() != '\n');
            return;
        }

        notas[index].valor = nova_nota;
        printf("Nota atualizada para %.2f com sucesso.\n", nova_nota);

    } else {
        printf("\n!!! ERRO: Nota não encontrada. Use a opção LANÇAR NOTA para criar um registro novo. !!!\n");
    }
}

// Permissão: Todos
void consultar_notas(Nota *notas, int total_notas, Pessoa *usuario_logado) {
    printf("\n--- Consulta de Notas ---\n");

    char cpf_busca[15];
    int notas_encontradas = 0;

    if (strcmp(usuario_logado->role, ROLE_ALUNO) == 0) {
        strcpy(cpf_busca, usuario_logado->cpf);
        printf("Consultando suas notas...\n");
    } else {
        printf("Digite o CPF do aluno para consulta: ");
        scanf("%s", cpf_busca);
    }

    for (int i = 0; i < total_notas; i++) {
        if (strcmp(notas[i].aluno_cpf, cpf_busca) == 0) {
            printf("----------------------------------------\n");
            printf("Atividade: %s\n", notas[i].atividade_titulo);
            printf("Nota: %.2f\n", notas[i].valor);
            notas_encontradas++;
        }
    }

    if (notas_encontradas == 0) { printf("Nenhuma nota encontrada para o CPF %s.\n", cpf_busca); }
    printf("----------------------------------------\n");
}

// Permissão: ADM Exclusivo
void excluir_nota(Nota *notas, int *total_notas, Pessoa *usuario_logado) {

    if (strcmp(usuario_logado->role, ROLE_ADM) != 0) {
        printf("\n!!! ACESSO NEGADO. Apenas ADM. !!!\n");
        return;
    }

    char aluno_cpf_busca[15];
    char atividade_titulo_busca[100];
    char confirmacao;

    printf("\n--- Excluir Nota (ADM EXCLUSIVO) ---\n");
    printf("CPF do Aluno: "); scanf("%s", aluno_cpf_busca);
    printf("Título da Atividade: "); scanf(" %[^\n]", atividade_titulo_busca);

    int index = buscar_nota(notas, *total_notas, aluno_cpf_busca, atividade_titulo_busca);

    if (index != -1) {
        printf("Nota encontrada (%.2f). Confirma exclusão? (S/N): ", notas[index].valor);
        scanf(" %c", &confirmacao);

        if (confirmacao == 'S' || confirmacao == 's') {
            for (int i = index; i < *total_notas - 1; i++) {
                notas[i] = notas[i+1];
            }
            (*total_notas)--;
            printf("Nota excluída com sucesso.\n");
        } else {
            printf("Exclusão cancelada.\n");
        }
    } else {
        printf("Nota não encontrada.\n");
    }
}

// --- FUNÇÕES DE ATIVIDADES E TURMAS ---

void inserir_atividade(Atividade *atividades, int *total, char *professor_cpf) {
    if (*total >= MAX_ATIVIDADES) { printf("Limite de atividades atingido.\n"); return; }

    Atividade nova_atividade;

    printf("\n--- Inserir Nova Atividade (Professor) ---\n");
    printf("Título: "); scanf(" %[^\n]", nova_atividade.titulo);
    printf("Descrição: "); scanf(" %[^\n]", nova_atividade.descricao);
    printf("Data de Entrega (DD/MM/AAAA): "); scanf(" %s", nova_atividade.data_entrega);

    strcpy(nova_atividade.professor_cpf, professor_cpf);

    atividades[*total] = nova_atividade;
    (*total)++;
    printf("Atividade '%s' cadastrada com sucesso.\n", nova_atividade.titulo);
}

void consultar_atividades_aluno(Atividade *atividades, int total) {
    if (total == 0) { printf("\nNenhuma atividade cadastrada no sistema.\n"); return; }

    printf("\n--- Consulta de Aulas e Atividades (Aluno) ---\n");
    for (int i = 0; i < total; i++) {
        printf("----------------------------------------\n");
        printf("Atividade: %s\n", atividades[i].titulo);
        printf("Entrega: %s\n", atividades[i].data_entrega);
    }
    printf("----------------------------------------\n");
}

void inserir_turma(Turma *turmas, int *total_turmas) {
    if (*total_turmas >= MAX_TURMAS) { printf("Limite de turmas atingido.\n"); return; }

    Turma nova_turma;
    char cpf_professor_temp[15];

    printf("\n--- Inserir Nova Turma (ADM) ---\n");
    printf("Nome/Código da Turma: "); scanf(" %[^\n]", nova_turma.nome);

    printf("CPF do Professor Responsável (Pendente se vazio): "); scanf(" %s", cpf_professor_temp);

    if (strcmp(cpf_professor_temp, "PENDENTE") != 0 && strlen(cpf_professor_temp) > 0) {
        strcpy(nova_turma.professor_cpf, cpf_professor_temp);
        srand(time(NULL));
        nova_turma.total_alunos = (rand() % 5) + 1;
    } else {
        strcpy(nova_turma.professor_cpf, "PENDENTE");
        nova_turma.total_alunos = 0;
    }

    turmas[*total_turmas] = nova_turma;
    (*total_turmas)++;
    printf("Turma '%s' cadastrada.\n", nova_turma.nome);
}

void consultar_turma(Turma *turmas, int total_turmas, Pessoa *usuario_logado) {
    printf("\n[ SISTEMA DE GESTÃO - Consulta de Turma ]\n");
    printf("----------------------------------------------------\n");

    if (total_turmas == 0) { printf("Nenhuma turma foi cadastrada.\n"); return; }

    if (strcmp(usuario_logado->role, ROLE_ADM) == 0) {
        printf("Lista de Turmas (ADM):\n");
        for (int i = 0; i < total_turmas; i++) {
            printf("[%d] Turma: %s | Prof.: %s | Alunos: %d\n",
                i + 1, turmas[i].nome, turmas[i].professor_cpf, turmas[i].total_alunos);
        }
    } else {
        Turma *minha_turma = &turmas[0]; // Simula a busca pela turma do usuário
        printf("Sua Turma de Interesse: %s\n", minha_turma->nome);
        printf("- Professor Responsável: %s\n", (strcmp(minha_turma->professor_cpf, "PENDENTE") == 0) ? "A definir" : minha_turma->professor_cpf);
    }
    printf("----------------------------------------------------\n");
}

void gerar_relatorio() {
    printf("\n[ SISTEMA de GESTÃO - RELATÓRIO ]\n");
    printf("----------------------------------------------------\n");
    printf("Relatório básico gerado com sucesso! (ADM/Professor)\n");
    printf("----------------------------------------------------\n");
}


// --- FUNÇÕES DE PERSISTÊNCIA ---

void salvar_dados_sistema(Pessoa *pessoas, int total_pessoas, Atividade *atividades, int total_atividades, Turma *turmas, int total_turmas, Nota *notas, int total_notas) {
    FILE *file = fopen(NOME_ARQUIVO, "wb");
    if (file == NULL) { printf("\nERRO ao abrir o arquivo para salvar.\n"); return; }

    fwrite(&total_pessoas, sizeof(int), 1, file);
    fwrite(pessoas, sizeof(Pessoa), total_pessoas, file);

    fwrite(&total_atividades, sizeof(int), 1, file);
    fwrite(atividades, sizeof(Atividade), total_atividades, file);

    fwrite(&total_turmas, sizeof(int), 1, file);
    fwrite(turmas, sizeof(Turma), total_turmas, file);

    fwrite(&total_notas, sizeof(int), 1, file);
    fwrite(notas, sizeof(Nota), total_notas, file);

    fclose(file);
    printf("\n[PERSISTÊNCIA] Dados salvos com sucesso.\n");
}

void carregar_dados_sistema(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas) {
    FILE *file = fopen(NOME_ARQUIVO, "rb");
    if (file == NULL) {
        *total_pessoas = 0; *total_atividades = 0; *total_turmas = 0; *total_notas = 0;
        printf("\n[PERSISTÊNCIA] Arquivo de dados não encontrado. Iniciando sistema vazio.\n");
        return;
    }

    fread(total_pessoas, sizeof(int), 1, file);
    fread(pessoas, sizeof(Pessoa), *total_pessoas, file);

    fread(total_atividades, sizeof(int), 1, file);
    fread(atividades, sizeof(Atividade), *total_atividades, file);

    fread(total_turmas, sizeof(int), 1, file);
    fread(turmas, sizeof(Turma), *total_turmas, file);

    fread(total_notas, sizeof(int), 1, file);
    fread(notas, sizeof(Nota), *total_notas, file);

    fclose(file);
    printf("\n[PERSISTÊNCIA] Dados carregados.\n");
}
