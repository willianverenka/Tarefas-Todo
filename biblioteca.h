//
// Created by unifwsilva on 12/09/2023.
//

#ifndef PROJETO_TAREFAS_BIBLIOTECA_H
#define PROJETO_TAREFAS_BIBLIOTECA_H

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

#endif //PROJETO_TAREFAS_BIBLIOTECA_H
