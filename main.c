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
        scanf_s(" %c", &input);
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
            default:
                printf("Entrada invalida.\n");
                break;
        }
    }
    return OK;
}
