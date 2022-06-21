#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define elementos 10000000

int numberOfPartitions = 0;

typedef struct Cliente
{
    int CodCliente;
    char Nome[50];
    char DataNascimento[8];

} Cliente;

typedef struct No
{
    Cliente cliente;
    short int congelado;
    No * dir;
    No * esq;    
} No;


void salvaCliente(FILE * arquivo, Cliente * aux){
    fwrite(&aux->CodCliente, sizeof(int), 1, arquivo);
    fwrite(aux->Nome, sizeof(aux->Nome), 1, arquivo);
    fwrite(aux->DataNascimento, sizeof(aux->DataNascimento), 1, arquivo);
}

int randomNumber(int max, int min){
    int aux = 0;
    aux = (rand() % (max - min + 1)) + min;
    return aux;
}

void trocaElementos(int * vet, int a, int b){
    int aux = vet[a];
    vet[a] = vet[b];
    vet[b] = aux;
}

void embaralhaVetor(int * vet, int iter, int tam){
    printf("========Embaralhando vetor========\n");
    int a, b;
    int per = 0;
    for(int i = 0; i < iter; i++){
        per = (int)((float) iter * 0.10); 
        if( i % per == 0)
            printf("Ja foi: %.0f %%\n", (float)i / (float)iter * 100.0);
        a = randomNumber(tam-1,0);
        b = randomNumber(tam-1,0);
        trocaElementos(vet, a, b);
    }
}



Cliente* criaCliente(int Cod, char * nome, char * data){

    Cliente * aux = malloc(sizeof(Cliente));
    aux->CodCliente = Cod;
    strcpy(aux->Nome, nome);
    strcpy(aux->DataNascimento, data);
    return aux;

}


Cliente * lerCliente(int cod, FILE * arquivo){
    Cliente * aux = malloc(sizeof(Cliente));
    rewind(arquivo);

    while (aux->CodCliente != cod)
    {
        if(0 >= fread(&aux->CodCliente, sizeof(int), 1, arquivo)){
            free(aux);
            break;
        }
        if(aux->CodCliente == cod){
            fread(&aux->Nome, sizeof(char), 50 , arquivo);
            fread(&aux->DataNascimento, sizeof(char), 8 , arquivo);
            return aux;
        }
        fseek(arquivo, sizeof(char) * 50, SEEK_CUR);
        fseek(arquivo, sizeof(char) * 8, SEEK_CUR);
    }
    
}

void imprimeCliente(Cliente * cliente){
    if(cliente == NULL){
        printf("Cliente inexistente!\n");
        return;
    }
    printf("\n===========Cliente=========\n\n");
    printf("Codigo: %i\n", cliente->CodCliente);
    printf("Nome: %s\n", cliente->Nome);
    printf("Data: %s\n\n", cliente->DataNascimento);
    printf("=============================\n\n");
}


void cadastraCliente(int cod, char * nome, char * data, FILE * arquivo){

    int aux;
    while (0 < fread(&aux, sizeof(int), 1, arquivo))
    {
        fseek(arquivo, sizeof(char) * 50, SEEK_CUR);
        fseek(arquivo, sizeof(char) * 8, SEEK_CUR);
    }
    
    Cliente * cliente = criaCliente(cod, nome, data);
    salvaCliente(arquivo, cliente);
    free(cliente);

}

void escreveCliente(FILE * arquivo, int cod, char * nome, char * data){
    Cliente * aux = criaCliente(cod, nome, data);
    salvaCliente(arquivo, aux);
}

Cliente * leCliente(FILE * arquivo){

    Cliente * aux = malloc(sizeof(Cliente));
    if (0 >= fread(&aux->CodCliente, sizeof(int), 1, arquivo))
    {
        return NULL;
    }
    fread(&aux->Nome, sizeof(char), 50 , arquivo);
    fread(&aux->DataNascimento, sizeof(char), 8 , arquivo);
    return aux;
}

void escreveNoDisco(FILE * arquivo, int qntd){
    int i = 0;
    int per = 0;
    int * vet;
    vet = malloc(sizeof(int) * qntd);
    for(i = 0; i < qntd; i++){
        vet[i] = i;
    }
    embaralhaVetor(vet, 50000000, qntd);
    printf("========Escrevendo no disco========\n");
    
    for (i = 0; i < qntd; i++)
    {   
        per = (int)((float) qntd * 0.10); 
        if( i % per == 0)
            printf("Ja foi: %.0f %%\n", (float)i / (float)qntd * 100.0);
        escreveCliente(arquivo, vet[i], "Gustavo Santos", "12062000");
    }
    free(vet);
    
}

FILE * createNewPartition(){
    numberOfPartitions++;
    char path[70];
    sprintf(path, "Partitions/Partition_%d.dat", numberOfPartitions);
    return fopen(path, "ab+");
}

void insereComPrioridade(No * raiz, No x){
    No * pos = raiz;
    while(pos->dir != NULL){
        if(pos->cliente.CodCliente < x.cliente.CodCliente){

        }
    }
}

void classificacaoExterna(FILE * arquivo, int m){
    No * raiz;
    Cliente * aux;
    aux = leCliente(arquivo);
    while (aux != NULL)
    {
        //le cliente
        aux = leCliente(arquivo);

    }
    
}

int main () {

    
    
    return 0;
}