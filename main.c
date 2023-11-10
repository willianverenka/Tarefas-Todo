#include <stdio.h>
#include "biblioteca.h"
#include "stdlib.h"

int main() {
    /* declara a array (memoria),
* o tamanho da array e o estado do loop */
    struct estadoPrograma state;
    state.loop = FUNCIONANDO;
    FILE *f;
    f = fopen("dados.bin", "rb");
    if(f){
        atribuirMemoria(&state); //carrega os dados do estado do programa (array de tarefas, tamanho)
        fclose(f);
    }
    else{
        fclose(f);
        f = fopen("dados.bin", "wb");
        int t = 0;
        fwrite(&t, sizeof(int), 1, f);
        fclose(f);
        state.tamanho = t;
        printf("Arquivo nao encontrado!\ndados.bin foi criado com sucesso.\n");
    }
    char input = '-';
    int inputDeletar = 0;
    int resultado;
    while(state.loop == FUNCIONANDO){
        menu();
        scanf(" %c", &input);
        getchar();
        switch(input){
            case '1':
                if(criarTarefa(&state) == OK){
                    ++state.tamanho;
                }
                else{ // a funcao retorna 1 (ERRO) caso existam 100 tarefas
                    printf("ERRO:\nNao ha memoria disponivel.\n");
                }
                break;
            case '2':
                if(state.tamanho == 0){
                    printf("Nao ha tarefas a serem excluidas.\n");
                    break;
                }
                listarTarefas(&state);
                do{
                    printf("Digite a posicao na lista do item a ser deletado (1-%d).\n", state.tamanho);
                    resultado = scanf("%d", &inputDeletar);
                }while(resultado != 1 || inputDeletar > state.tamanho || inputDeletar < 0);
                if(deletarTarefa(&state, inputDeletar) == OK){
                    printf("Tarefa deletada com sucesso.\n");
                    --state.tamanho;
                }
                else{
                    printf("Erro ao deletar tarefa.\n");
                }
                break;
            case '3':
                if(listarTarefas(&state)==ERRO){
                    printf("Nao ha tarefas para serem lidas.\n");
                }
                printf("Pressione qualquer tecla pra continuar...\n");
                getchar();
                break;
            case '4':
                if(salvar(state) == ERRO){
                    char inputSalvar = '-';
                    do{
                        printf("Erro ao salvar o arquivo.\nGostaria de sair mesmo assim? (S/N)\n");
                        scanf("%c", &inputSalvar);
                    }while(inputSalvar != 'S' && inputSalvar != 'N' && inputSalvar != 's' && inputSalvar != 'n');
                    if(inputSalvar == 'S' || inputSalvar == 's'){
                        printf("SAINDO\n");
                        state.loop = FECHADO;
                        break;
                    }
                    break;
                }
                else{
                    printf("SAINDO\n");
                    state.loop = FECHADO;
                    break;
                }
            case '5':
                alterarTarefas(&state);
                break;
            case '6':
            enum filtro filtro;
                int escolha;
                do{
                    printf("Por qual propriedade deseja filtrar a lista?\n");
                    printf("1. Prioridade 2. Estado 3. Categoria\n");
                    scanf("%d", &escolha);
                }while(escolha < 1 || escolha > 3);
                switch(escolha){
                    case 1:
                        int prioridade;
                        do{
                            printf("Qual prioridade deseja filtrar? (1-10)\n");
                            scanf("%d", &prioridade);
                        }while(prioridade < 1 || prioridade > 10);
                        filtrarTarefas(&state, PRIORIDADE, prioridade);
                        break;
                    case 2:
                        enum estadoTarefa estado;
                        int estadoInt;
                        do{
                            printf("Qual estado deseja filtrar?\n");
                            printf("1. Nao iniciado 2. Completo 3. Andamento");
                            scanf("%d", &prioridade);
                        }while(estadoInt < 1 || estadoInt > 3);
                        estado = estadoInt - 2;
                        filtrarTarefas(&state, ESTADO, estado);
                        break;
                    case 3:
                        char categoria[100];
                        printf("Qual categoria deseja filtrar?\n");
                        filtrarTarefas(&state, ESTADO, categoria);
                        break;
                }
                filtro = escolha;
                filtrarTarefas(&state);
            default:
                printf("Entrada invalida.\n");
                break;
        }
    }
    return OK;
}
