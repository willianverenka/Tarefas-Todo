#include <stdio.h>
#include "biblioteca.h"
#include <string.h>


void menu(){
    printf("1. Cadastrar tarefa\n");
    printf("2. Excluir tarefa\n");
    printf("3. Listar tarefas\n");
    printf("4. Sair\n");
    printf("5. Editar\n");
};

int buscarTarefa(struct estadoPrograma *state, int escolha){
    if(state->tamanho == 0 || state->tamanho < escolha || escolha == 0){
        return -1;
    }
    return escolha-1;
}

int criarTarefa(struct estadoPrograma *state){
    if(state->tamanho == 100)
        return ERRO;
    struct tarefa novaTarefa;
    int p = 0; // valor da prioridade
    char c[100]; // vetor p/ categoria
    char d[300]; // vetor p/ descricao
    do{
        printf("Prioridade da tarefa (1-10):\n");
        scanf("%d", &p);
        getchar();
    }while(p <= 0 || p > 10);
    printf("Descricao da tarefa (max 300 caracteres):\n");
    scanf("%299[^\n]%*c", &d);
    printf("Categoria (max 100 caracteres):\n");
    scanf("%99[^\n]%*c", &c);
    novaTarefa.prioridade = p;
    strcpy(novaTarefa.categoria, c);
    strcpy(novaTarefa.descricao, d);
    state->memoria[state->tamanho] = novaTarefa;
    return OK;
}

int atribuirMemoria(struct estadoPrograma *ponteiroEstado){
    FILE *f = fopen("dados.bin", "rb");
    if(f == NULL){
        printf("ERRO:\n");
        printf("Nao foi posivel abrir o arquivo dados.bin");
        return ERRO;
    }
    // carrega o tamanho
    fread(&(ponteiroEstado->tamanho), sizeof(int), 1, f);
    // carrega a array de tarefas
    for(int i = 0; i < ponteiroEstado->tamanho; i++){
        fread(&(ponteiroEstado->memoria[i]), sizeof(struct tarefa), 1, f);
    }
    printf("Tarefas salvas carregadas com sucesso!\n");
    fclose(f);
    return OK;
}

int salvar(struct estadoPrograma state){
    FILE *f = fopen("dados.bin", "wb");
    if(f == NULL){
        printf("ERRO:\n");
        printf("Não foi possível abrir o arquivo dados.bin.\n");
        return ERRO;
    }
    // salva o tamanho no binario
    fwrite(&state.tamanho, sizeof(int), 1, f);
    // salva a array de structs
    for(int i = 0; i < state.tamanho; i++){
        fwrite(&state.memoria[i], sizeof(struct tarefa), 1, f);
    }
    fclose(f);
    printf("Arquivo atualizado com sucesso!\n");
    return OK;
}

int listarTarefas(struct estadoPrograma *state){
    if(state->tamanho == 0) // nao ha tarefas registradas
        return ERRO;
    struct tarefa *tarefas = state->memoria;
    printf("@@@@ TAREFAS @@@\n");
    for(int i = 0; i < state->tamanho; i++){
        printf("\n%d.\n", i+1);
        printf("Prioridade: %d\n", tarefas[i].prioridade);
        printf("Descricao: %s\n", tarefas[i].descricao);
        printf("Categoria: %s\n", tarefas[i].categoria);
    }
    return OK;
}

/* A funcao consiste em rearranjar a array. Ja que ela tem tamanho fixo (100), nao existem elementos
 * "null" ou vazios. O que o programa faz eh mandar o elemento a ser deletado para a direita do vetor, sendo
 * posicao = tamanho-1. Apos isso, diretamente no switch da main, o tamanho eh diminuido em 1. Assim, apesar do elemento
 * deletado ainda existir na array, ele nao eh computado. Quando o usuario criar outra tarefa, as tarefas que nao sao
 * computadas serao sobrescritas, e como o tamanho aumentara, serao lidas.*/
int deletarTarefa(struct estadoPrograma *state, int index){
    int novoIndex = index-1; // o input começa em 1 para melhor listagem de tarefas
    /* caso a tarefa seja a ultima, retorna ok pra nao dar erro no runtime (i+1 seria invalido)
     * como o alvo ja eh o ultimo item da array, entao faz sentido retornar OK e apenas diminuir
     * o tamanho na main*/
    if(novoIndex == state->tamanho-1){
        return OK;
    }
    else if(state->tamanho > 0){
        for(int i = novoIndex; i < state->tamanho - 1; i++){
            state->memoria[i] = state->memoria[i+1];
        }
        return OK;
    }
    else{
        return ERRO;
    }
}

int alterarTarefas(struct estadoPrograma *state){
    if(state->tamanho == 0){
        return ERRO;
    }
    listarTarefas(state);
    int escolha;
    printf("Digite o numero da tarefa a ser alterada:\n");
    scanf("%d", &escolha);
    int indexTarefa = buscarTarefa(state, escolha);
    if(indexTarefa == -1){
        return ERRO;
    }
    int novaPrioridade;
    char novaDescricao[300];
    char novaCategoria[100];
    int novoEstadoTarefa;
    do{
        printf("Digite a nova prioridade (1-10): digite -1 caso não queira alterar\n");
        scanf("%d", &novaPrioridade);
    }while(novaPrioridade < 0 && novaPrioridade != -1 || novaPrioridade > 10);
    printf("Digite a nova descricao: digite / caso não queira alterar\n");
    scanf("%s", novaDescricao);
    printf("Digite a nova categoria: digite / caso não queira alterar\n");
    scanf("%s", novaCategoria);
    do{
        printf("Digite o novo estado da tarefa: digite -1 caso não queria alterar.\n");
        printf("1. Não iniciado 2. Em andamento 3. Completo\n");
        scanf("%d", &novoEstadoTarefa);
    }while(novoEstadoTarefa < 1 && novoEstadoTarefa != -1 || novoEstadoTarefa > 3);
    if(novaPrioridade != -1){
        state->memoria[indexTarefa].prioridade = novaPrioridade;
    }
    if(novaCategoria[0] != '/'){
        strcpy(state->memoria[indexTarefa].categoria, novaCategoria);
    }
    if(novaDescricao[0] != '/'){
        strcpy(state->memoria[indexTarefa].descricao, novaDescricao);
    }
    if(novoEstadoTarefa != -1){
        switch(novoEstadoTarefa){
            case 1:
                state->memoria[indexTarefa].estadoTarefa = NAO_INICIADO;
                break;
            case 2:
                state->memoria[indexTarefa].estadoTarefa = ANDAMENTO;
                break;
            default:
                state->memoria[indexTarefa].estadoTarefa = COMPLETO;
        }
    }
    printf("Alteracoes feitas com sucesso!\n");
    return OK;
}