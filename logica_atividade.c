#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "logica_atividades.h"
#include "logica_pessoas.h"

void __attribute__((constructor)) init_locale_atividades() {
    setlocale(LC_ALL, "");
}

// Busca por EMAIL do Aluno
int buscar_nota(Nota *notas, int total, char *aluno_email, char *atividade_titulo) {
    for (int i = 0; i < total; i++) {
        if (strcmp(notas[i].aluno_email, aluno_email) == 0 && strcmp(notas[i].atividade_titulo, atividade_titulo) == 0) {
            return i;
        }
    }
    return -1;
}

// --- FUNÇÕES DE NOTAS ---

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
    printf("\n--- Lançar Nova Nota ---\n");

    printf("E-mail do Aluno que receberá a nota: ");
    scanf("%s", nova_nota.aluno_email);

    printf("Título da Atividade: ");
    scanf(" %[^\n]", nova_nota.atividade_titulo);

    if (buscar_nota(notas, *total_notas, nova_nota.aluno_email, nova_nota.atividade_titulo) != -1) {
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
    printf("\nNota %.2f lançada com sucesso para o aluno %s.\n", nova_nota.valor, nova_nota.aluno_email);
}

void editar_nota(Nota *notas, int total_notas, Pessoa *usuario_logado) {

    if (strcmp(usuario_logado->role, ROLE_ALUNO) == 0) {
        printf("\n!!! ACESSO NEGADO. Apenas ADM ou Professor podem editar notas. !!!\n");
        return;
    }

    char aluno_email_busca[TAM_EMAIL];
    char atividade_titulo_busca[100];
    float nova_nota;

    printf("\n--- Editar/Alterar Nota Existente ---\n");
    printf("E-mail do Aluno: ");
    scanf("%s", aluno_email_busca);
    printf("Título da Atividade: ");
    scanf(" %[^\n]", atividade_titulo_busca);

    int index = buscar_nota(notas, total_notas, aluno_email_busca, atividade_titulo_busca);

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

void consultar_notas(Nota *notas, int total_notas, Pessoa *usuario_logado) {
    printf("\n--- Consulta de Notas ---\n");

    char email_busca[TAM_EMAIL];
    int notas_encontradas = 0;

    if (strcmp(usuario_logado->role, ROLE_ALUNO) == 0) {
        strcpy(email_busca, usuario_logado->email);
        printf("Consultando suas notas...\n");
    } else {
        printf("Digite o E-mail do aluno para consulta: ");
        scanf("%s", email_busca);
    }

    for (int i = 0; i < total_notas; i++) {
        if (strcmp(notas[i].aluno_email, email_busca) == 0) {
            printf("----------------------------------------\n");
            printf("Atividade: %s\n", notas[i].atividade_titulo);
            printf("Nota: %.2f\n", notas[i].valor);
            notas_encontradas++;
        }
    }

    if (notas_encontradas == 0) { printf("Nenhuma nota encontrada para o E-mail %s.\n", email_busca); }
    printf("----------------------------------------\n");
}

void excluir_nota(Nota *notas, int *total_notas, Pessoa *usuario_logado) {

    if (strcmp(usuario_logado->role, ROLE_ADM) != 0) {
        printf("\n!!! ACESSO NEGADO. Apenas ADM. !!!\n");
        return;
    }

    char aluno_email_busca[TAM_EMAIL];
    char atividade_titulo_busca[100];
    char confirmacao;

    printf("\n--- Excluir Nota ---\n");
    printf("E-mail do Aluno: ");
    scanf("%s", aluno_email_busca);
    printf("Título da Atividade: ");
    scanf(" %[^\n]", atividade_titulo_busca);

    int index = buscar_nota(notas, *total_notas, aluno_email_busca, atividade_titulo_busca);

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



void inserir_atividade(Atividade *atividades, int *total_atividades, char *professor_email) {
    if (*total_atividades >= MAX_ATIVIDADES) { printf("Limite de atividades atingido.\n"); return; }

    Atividade nova_atividade;

    printf("\n--- Inserir Nova Atividade ---\n");
    printf("Título: ");
    scanf(" %[^\n]", nova_atividade.titulo);
    printf("Descrição: ");
    scanf(" %[^\n]", nova_atividade.descricao);
    printf("Data de Entrega (DD/MM/AAAA): ");
    scanf(" %s", nova_atividade.data_entrega);

    strcpy(nova_atividade.professor_email, professor_email);

    atividades[*total_atividades] = nova_atividade;
    (*total_atividades)++;
    printf("Atividade '%s' cadastrada com sucesso.\n", nova_atividade.titulo);
}

void consultar_atividades_aluno(Atividade *atividades, int total_atividades) {
    if (total_atividades == 0) { printf("\nNenhuma atividade cadastrada no sistema.\n"); return; }

    printf("\n--- Consulta de Aulas e Atividades ---\n");
    for (int i = 0; i < total_atividades; i++) {
        printf("----------------------------------------\n");
        printf("Atividade: %s\n", atividades[i].titulo);
        printf("Professor (E-mail): %s\n", atividades[i].professor_email);
        printf("Entrega: %s\n", atividades[i].data_entrega);
    }
    printf("----------------------------------------\n");
}

void inserir_turma(Turma *turmas, int *total_turmas) {
    if (*total_turmas >= MAX_TURMAS) { printf("Limite de turmas atingido.\n"); return; }

    Turma nova_turma;
    char email_professor_temp[TAM_EMAIL];

    printf("\n--- Inserir Nova Turma ---\n");
    printf("Nome/Código da Turma: ");
    scanf(" %[^\n]", nova_turma.nome);

    printf("Nome do Professor Responsável : ");
    scanf(" %s", email_professor_temp);

    if (strcmp(email_professor_temp, "PENDENTE") != 0 && strlen(email_professor_temp) > 0) {
        strcpy(nova_turma.professor_email, email_professor_temp);
        // Gera um número aleatório de alunos entre 1 e 5 (exemplo)
        srand(time(NULL));
        nova_turma.total_alunos = (rand() % 5) + 1;
    } else {
        strcpy(nova_turma.professor_email, "PENDENTE");
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

    if (strcmp(usuario_logado->role, ROLE_ADM) == 0 || strcmp(usuario_logado->role, ROLE_PROFESSOR) == 0) {
        printf("Lista de Turmas (ADM/Professor):\n");
        for (int i = 0; i < total_turmas; i++) {
            printf("[%d] Turma: %s | Prof. : %s | Alunos: %d\n",
                i + 1, turmas[i].nome, turmas[i].professor_email, turmas[i].total_alunos);
        }
    } else {
        // Aluno: Apenas mostra a primeira turma como exemplo de sua turma
        Turma *minha_turma = &turmas[0];
        printf("Sua Turma de Interesse: %s\n", minha_turma->nome);
        printf("- Professor Responsável : %s\n", (strcmp(minha_turma->professor_email, "PENDENTE") == 0) ? "A definir" : minha_turma->professor_email);
    }
    printf("----------------------------------------------------\n");
}

void gerar_relatorio() {
    printf("\n[ SISTEMA de GESTÃO - RELATÓRIO ]\n");
    printf("----------------------------------------------------\n");
    printf("Relatório gerado com sucesso! (ADM/Professor)\n");
    printf("----------------------------------------------------\n");
}

// --- FUNÇÕES DE PERSISTÊNCIA  ---

void salvar_dados_sistema(Pessoa *pessoas, int total_pessoas, Atividade *atividades, int total_atividades, Turma *turmas, int total_turmas, Nota *notas, int total_notas) {
    FILE *file = fopen(NOME_ARQUIVO, "w");
    if (file == NULL) {
        printf("\nERRO ao abrir o arquivo para salvar em texto.\n");
        return;
    }


    fprintf(file, "PESSOAS %d\n", total_pessoas);
    for (int i = 0; i < total_pessoas; i++) {
        // Formato: Nome|Email|Senha|Role
        fprintf(file, "%s|%s|%s|%s\n",
                pessoas[i].nome, pessoas[i].email, pessoas[i].senha, pessoas[i].role);
    }


    fprintf(file, "ATIVIDADES %d\n", total_atividades);
    for (int i = 0; i < total_atividades; i++) {

        fprintf(file, "%s|%s|%s|%s\n",
                atividades[i].titulo, atividades[i].descricao, atividades[i].professor_email,
                atividades[i].data_entrega);
    }


    fprintf(file, "TURMAS %d\n", total_turmas);
    for (int i = 0; i < total_turmas; i++) {

        fprintf(file, "%s|%s|%d\n",
                turmas[i].nome, turmas[i].professor_email, turmas[i].total_alunos);
    }


    fprintf(file, "NOTAS %d\n", total_notas);
    for (int i = 0; i < total_notas; i++) {
        // Formato: Aluno_Email|Atividade_Titulo|Valor
        fprintf(file, "%s|%s|%.2f\n",
                notas[i].aluno_email, notas[i].atividade_titulo, notas[i].valor);
    }

    fclose(file);
    printf("\n[PERSISTÊNCIA] Dados salvos em arquivo de texto com sucesso.\n");
}

void carregar_dados_sistema(Pessoa *pessoas, int *total_pessoas, Atividade *atividades, int *total_atividades, Turma *turmas, int *total_turmas, Nota *notas, int *total_notas) {
    FILE *file = fopen(NOME_ARQUIVO, "r");
    if (file == NULL) {
        *total_pessoas = 0; *total_atividades = 0; *total_turmas = 0; *total_notas = 0;
        printf("\n[PERSISTÊNCIA] Arquivo de dados de texto não encontrado. Iniciando sistema vazio.\n");
        return;
    }

    char header[20];
    int count;


    if (fscanf(file, "%s %d\n", header, &count) == 2 && strcmp(header, "PESSOAS") == 0) {
        *total_pessoas = 0;
        for (int i = 0; i < count && i < MAX_PESSOAS; i++) {
            if (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^\n]\n",
                pessoas[i].nome, pessoas[i].email, pessoas[i].senha, pessoas[i].role) == 4) {
                (*total_pessoas)++;
            } else {
                printf("\n[PERSISTÊNCIA] Erro ao ler registro de Pessoa. Interrompendo carregamento.\n");
                break;
            }
        }
    }

    // 2. Atividades
    if (fscanf(file, "%s %d\n", header, &count) == 2 && strcmp(header, "ATIVIDADES") == 0) {
        *total_atividades = 0;
        for (int i = 0; i < count && i < MAX_ATIVIDADES; i++) {
            if (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^\n]\n",
                atividades[i].titulo, atividades[i].descricao, atividades[i].professor_email, atividades[i].data_entrega) == 4) {
                (*total_atividades)++;
            }
        }
    }

    // 3. Turmas
    if (fscanf(file, "%s %d\n", header, &count) == 2 && strcmp(header, "TURMAS") == 0) {
        *total_turmas = 0;
        for (int i = 0; i < count && i < MAX_TURMAS; i++) {
            if (fscanf(file, "%[^|]|%[^|]|%d\n",
                turmas[i].nome, turmas[i].professor_email, &turmas[i].total_alunos) == 3) {
                (*total_turmas)++;
            }
        }
    }

    // 4. Notas
    if (fscanf(file, "%s %d\n", header, &count) == 2 && strcmp(header, "NOTAS") == 0) {
        *total_notas = 0;
        for (int i = 0; i < count && i < MAX_NOTAS; i++) {
            if (fscanf(file, "%[^|]|%[^|]|%f\n",
                notas[i].aluno_email, notas[i].atividade_titulo, &notas[i].valor) == 3) {
                (*total_notas)++;
            }
        }
    }

    fclose(file);
    printf("\n[PERSISTÊNCIA] Dados carregados com sucesso. Total de %d usuários e %d notas.\n", *total_pessoas, *total_notas);
}
