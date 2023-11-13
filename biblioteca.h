//
// Created by unifwsilva on 12/09/2023.
//

#ifndef PROJETO_TAREFAS_BIBLIOTECA_H
#define PROJETO_TAREFAS_BIBLIOTECA_H

enum filtro{
    PRIORIDADE,
    ESTADO,
    CATEGORIA,
    PRIORIDADE_CATEGORIA
};

enum estadoLoop{
    FUNCIONANDO,
    FECHADO
};

enum estadoTarefa{
    NAO_INICIADO = -1,
    COMPLETO = 0,
    ANDAMENTO = 1
};

enum gerenciarErro{
    OK,
    ERRO
};

struct tarefa{
    int prioridade;
    char descricao[300];
    char categoria[100];
    enum estadoTarefa estadoTarefa;
};

struct estadoPrograma{
    enum estadoLoop loop;
    struct tarefa memoria[100];
    int tamanho;
};

int alterarTarefas(struct estadoPrograma *state);
int atribuirMemoria(struct estadoPrograma *ponteiroEstado);
void menu();
int criarTarefa(struct estadoPrograma *state);
int salvar(struct estadoPrograma state);
int listarTarefas(struct estadoPrograma *state);
int deletarTarefa(struct estadoPrograma *state, int index);
void input(char entrada[], char *saida);
struct tarefa* buscarTarefasPorFiltro(struct estadoPrograma *state, enum filtro filtro, void* escolha, int *tamanhoFiltragem);
int exportarTarefas(struct tarefa *tarefas, int tamanho);
int comparar(const void *a, const void *b);

#endif //PROJETO_TAREFAS_BIBLIOTECA_H
